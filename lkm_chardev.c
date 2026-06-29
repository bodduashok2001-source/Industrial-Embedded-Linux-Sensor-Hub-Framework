#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>
#include <linux/kthread.h>
#include <linux/delay.h>
#include <linux/string.h>
#include <linux/wait.h>
#include <linux/poll.h>
#include <linux/interrupt.h>
#include <linux/workqueue.h>
#include <linux/platform_device.h>

#define DEVICE_NAME "lkmchardev"
#define CLASS_NAME  "lkm_class"
#define BUF_SIZE 256

#define IOCTL_BASE 'M'
#define SET_MODE _IOW(IOCTL_BASE, 0, int)
#define GET_MODE _IOR(IOCTL_BASE, 1, int)

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Ashok");
MODULE_DESCRIPTION("Character Device Driver with IOCTL");

static dev_t dev_num;
static struct cdev my_cdev;
static struct class *my_class;
static struct device *my_device;
static struct task_struct *sensor_thread;
static int sensor_value = 25;
static wait_queue_head_t sensor_wq;
static struct work_struct sensor_work;
static struct mutex rb_lock;


/* ================= RING BUFFER ================= */

#define MAX_MSG_SIZE 256
#define RING_SIZE 8

struct ring_buffer {
    char data[RING_SIZE][MAX_MSG_SIZE];
    int len[RING_SIZE];

    int head;
    int tail;
};

static struct ring_buffer rb = {
    .head = 0,
    .tail = 0,
};

static int mode = 0; // 0-normal, 1-uppercase, 2-reverse

static int rb_is_empty(void)
{
    return rb.head == rb.tail;
}

static int rb_is_full(void)
{
    return ((rb.head + 1) % RING_SIZE) == rb.tail;
}

static void sensor_work_fn(struct work_struct *work)
{
    int slot;
    char msg[MAX_MSG_SIZE];
    int len;
	
	mutex_lock(&rb_lock);

    if (rb_is_full())
	{
		mutex_unlock(&rb_lock);
		return;
	}

    slot = rb.head;

    len = snprintf(msg,
                   MAX_MSG_SIZE,
                   "TEMP:%d",
                   sensor_value++);

    memcpy(rb.data[slot], msg, len);

    rb.len[slot] = len;

    rb.head = (rb.head + 1) % RING_SIZE;

    wake_up_interruptible(&sensor_wq);

    printk(KERN_INFO
           "workqueue stored slot=%d value=%s\n",
           slot,
           msg);
	
	mutex_unlock(&rb_lock);
}

static irqreturn_t sensor_irq_handler(
        int irq,
        void *dev_id)
{
    printk(KERN_INFO
           "sensor irq occurred\n");

    schedule_work(&sensor_work);

    return IRQ_HANDLED;
}

/* ================= Thread ================= */

static int sensor_thread_fn(void *data)
{
    while (!kthread_should_stop())
    {
        sensor_irq_handler(0, NULL);

        msleep(2000);
    }

    return 0;
}

/* ================= FILE OPERATIONS ================= */

static int my_open(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "lkmchardev: opened\n");
    return 0;
}

static int my_release(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "lkmchardev: closed\n");
    return 0;
}

/* WRITE */
static ssize_t my_write(struct file *file,
                        const char __user *buf,
                        size_t len,
                        loff_t *off)
{
    int slot;
	
	mutex_lock(&rb_lock);

    if (rb_is_full())
    {
        printk(KERN_ERR "lkmchardev: ring buffer full\n");
		mutex_unlock(&rb_lock);
        return -ENOMEM;
    }

    if (len > MAX_MSG_SIZE)
        len = MAX_MSG_SIZE;

    slot = rb.head;

    if (copy_from_user(rb.data[slot], buf, len))
	{
		mutex_unlock(&rb_lock);
        return -EFAULT;
	}

    rb.len[slot] = len;

    rb.head = (rb.head + 1) % RING_SIZE;

    printk(KERN_INFO "stored message in slot=%d\n", slot);
	
	mutex_unlock(&rb_lock);

    return len;
}

/* READ */
static ssize_t my_read(struct file *file,
                       char __user *buf,
                       size_t len,
                       loff_t *off)
{
    int slot;
    int msg_len;
    char temp[MAX_MSG_SIZE];
    int i;

    wait_event_interruptible(
        sensor_wq,
        !rb_is_empty());
	
	mutex_lock(&rb_lock);

    slot = rb.tail;

    msg_len = rb.len[slot];

    memcpy(temp, rb.data[slot], msg_len);

    if (mode == 1)
    {
        for (i = 0; i < msg_len; i++)
        {
            if (temp[i] >= 'a' && temp[i] <= 'z')
                temp[i] -= 32;
        }
    }
    else if (mode == 2)
    {
        for (i = 0; i < msg_len / 2; i++)
        {
            char t = temp[i];
            temp[i] = temp[msg_len - i - 1];
            temp[msg_len - i - 1] = t;
        }
    }

    if (len > msg_len)
        len = msg_len;

    if (copy_to_user(buf, temp, len))
	{
		mutex_unlock(&rb_lock);
		return -EFAULT;
	}

    rb.tail = (rb.tail + 1) % RING_SIZE;

    printk(KERN_INFO "read message from slot=%d\n", slot);
	
	mutex_unlock(&rb_lock);

    return len;
}

/* IOCTL */
static long my_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    int val;

    switch (cmd) {
        case SET_MODE:
            if (copy_from_user(&val, (int __user *)arg, sizeof(val)))
                return -EFAULT;

            if (val < 0 || val > 2)
                return -EINVAL;

            mode = val;
            printk(KERN_INFO "lkmchardev: mode set to %d\n", mode);
            break;

        case GET_MODE:
            if (copy_to_user((int __user *)arg, &mode, sizeof(mode)))
                return -EFAULT;
            break;

        default:
            return -EINVAL;
    }
    return 0;
}

static __poll_t my_poll(struct file *file,
                        poll_table *wait)
{
    __poll_t mask = 0;

    poll_wait(file,
              &sensor_wq,
              wait);

    if (!rb_is_empty())
        mask |= POLLIN | POLLRDNORM;

    return mask;
}

/* File operations structure */
static struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = my_open,
    .release = my_release,
    .read = my_read,
    .write = my_write,
    .unlocked_ioctl = my_ioctl,
	.poll = my_poll,
};

/* =================== PROBE ====================== */

static int sensor_probe(struct platform_device *pdev)
{
    printk(KERN_INFO "Platform Driver: probe() called\n");
	
	if (alloc_chrdev_region(&dev_num, 0, 1, DEVICE_NAME) < 0)
	return -1;

    cdev_init(&my_cdev, &fops);

    if (cdev_add(&my_cdev, dev_num, 1) < 0)
        return -1;

    my_class = class_create(THIS_MODULE, CLASS_NAME);
    my_device = device_create(my_class, NULL, dev_num, NULL, DEVICE_NAME);
	
	sensor_thread = kthread_run(
				sensor_thread_fn,
				NULL,
				"sensor_thread");
					
	init_waitqueue_head(&sensor_wq);
	
	mutex_init(&rb_lock);
	
	INIT_WORK(&sensor_work,
          sensor_work_fn);

    printk(KERN_INFO "lkmchardev: created /dev/lkmchardev\n");

    return 0;
}

static int sensor_remove(struct platform_device *pdev)
{
	device_destroy(my_class, dev_num);
    class_destroy(my_class);
    cdev_del(&my_cdev);
    unregister_chrdev_region(dev_num, 1);
	if (sensor_thread)
    kthread_stop(sensor_thread);

    printk(KERN_INFO "lkmchardev: removed\n");
	
    printk(KERN_INFO "Platform Driver: remove() called\n");

    return 0;
}


static struct platform_driver sensor_driver = {
    .probe = sensor_probe,
    .remove = sensor_remove,
    .driver = {
        .name = "sensor_demo",
    },
};

/* ================= INIT & EXIT ================= */

static struct platform_device *sensor_pdev;

static int __init my_init(void)
{
	sensor_pdev =
    platform_device_register_simple(
        "sensor_demo",
        -1,
        NULL,
        0);
	
	platform_driver_register(&sensor_driver);
	
    return 0;
}

static void __exit my_exit(void)
{	
	platform_driver_unregister(&sensor_driver);
	platform_device_unregister(sensor_pdev);
}

module_init(my_init);
module_exit(my_exit);
