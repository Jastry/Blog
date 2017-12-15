#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>


#if 0
const char *pathname = "./mkfifo";
int main()
{
    umask(000);
    int ret = mkfifo(pathname, 0666);
    return 0;
}

#endif

#if 1
int main()
{
    int fd[2] = {0};
    int ret = pipe(fd);
    pid_t id = -1;
    if (ret < 0) {
        perror("pipe");
        return 1;
    }
    char buf[1024] = {0};
    id = fork();
    if (id < 0) {
        perror("fork");
        return 3;
    } else if (id > 0) {
        close(fd[0]);
        printf("father say : ");
        fflush(stdout);
        while (1) {
            bzero(buf, sizeof(buf));
            ssize_t size = read(0, buf, sizeof(buf -1 ));
            if (size > 0)
                write(fd[1], buf, strlen(buf));
        }
    } else {
        close(fd[1]);
        while (1) {
            ssize_t size = read(fd[0], buf, sizeof (buf));
            if (size > 0) {
                buf[size - 1] = 0;
                printf("child echo father : %s\n", buf);
            } 
        }
    } 
    return 0;
}


#endif //pipe
