#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <string.h>

#define DEVICE "/dev/lkmchardev"

#define IOCTL_BASE 'M'
#define SET_MODE _IOW(IOCTL_BASE, 0, int)
#define GET_MODE _IOR(IOCTL_BASE, 1, int)

int main()
{
    int fd, mode;
    char write_buf[256], read_buf[256];

    fd = open(DEVICE, O_RDWR);
    if (fd < 0) {
        perror("open");
        return -1;
    }

    printf("Enter string: ");
    fgets(write_buf, sizeof(write_buf), stdin);
    write_buf[strcspn(write_buf, "\n")] = 0;

    write(fd, write_buf, strlen(write_buf));

    printf("Mode (0-normal,1-upper,2-reverse): ");
    scanf("%d", &mode);

    ioctl(fd, SET_MODE, &mode);

    lseek(fd, 0, SEEK_SET);

    int n = read(fd, read_buf, sizeof(read_buf)-1);
    read_buf[n] = '\0';

    printf("Output: %s\n", read_buf);

    close(fd);
    return 0;
}
