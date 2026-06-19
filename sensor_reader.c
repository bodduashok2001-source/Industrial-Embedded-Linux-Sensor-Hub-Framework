#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define DEVICE "/dev/lkmchardev"

int main()
{
    int fd;
    char buffer[256];
    int n;

    fd = open(DEVICE, O_RDONLY);

    if (fd < 0)
    {
        perror("open");
        return -1;
    }

    while (1)
    {
        n = read(fd, buffer, sizeof(buffer) - 1);

        if (n > 0)
        {
            buffer[n] = '\0';

            printf("Received: %s\n", buffer);
        }

        sleep(1);
    }

    close(fd);

    return 0;
}
