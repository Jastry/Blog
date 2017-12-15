#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

const char *pathname = "./mkfifo";
int main()
{
    int fd = open(pathname, O_WRONLY);
    char buf[1024] = {0};
    ssize_t write_size = -1;
    ssize_t read_size = -1;
    if (fd < 0) {
        perror("open");
        return 1;
    }

    while (1) {
        bzero(buf, sizeof(buf));
        read_size = read(0, buf, sizeof(buf) - 1);
        if (read_size < 0) {
            perror("read");
            return 2;
        } else if (read_size > 0) {
            buf[read_size] = 0;
            write_size = write(fd, buf, strlen(buf));
            if (write_size < 0) {
                perror("write");
                return 3;
            } else if (write_size > 0) {
                printf("client send %ld bytes ,message is %s\n",write_size, buf);
            }
        }

    }
    close(fd);
    return 0;
}
