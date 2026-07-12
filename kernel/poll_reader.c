#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>
#include <string.h>

#define DEVICE "/dev/lkmchardev"

int main()
{
    int fd;
    char buffer[256];

    struct pollfd pfd;

    fd = open(DEVICE, O_RDONLY);

    if (fd < 0)
    {
        perror("open");
        return -1;
    }

    pfd.fd = fd;
    pfd.events = POLLIN;

    while (1)
    {
        printf("Waiting for sensor event...\n");

        int ret = poll(&pfd,
                       1,
                       5000);

        if (ret < 0)
        {
            perror("poll");
            break;
        }

        if (ret == 0)
        {
            printf("Timeout\n");
            continue;
        }

        if (pfd.revents & POLLIN)
        {
            int n;

            n = read(fd,
                     buffer,
                     sizeof(buffer) - 1);

            if (n > 0)
            {
                buffer[n] = '\0';

                printf("Received: %s\n",
                       buffer);
            }
        }
    }

    close(fd);

    return 0;
}
