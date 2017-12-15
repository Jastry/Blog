#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

const char *filename = "./mkfifo";
int main()
{
    int fd = open(filename, O_RDONLY);
    char buf[1024] = {0};
    ssize_t read_size = -1;
    if (fd < 0) {
        perror("open");
        return 1;
    }
    while (1) {
        bzero(buf, sizeof(buf));
        read_size = read(fd, buf, sizeof(buf) - 1);
        if (read_size < 0) {
            perror("read");
            //continue;
            return 2;
        } else if (read_size > 0) {
            buf[read_size - 1] = 0;
            printf("server say : %s\n", buf);
        } else {
            printf("read done\n");
            close(fd);
        }
    }
    return 0;
}
