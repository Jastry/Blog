#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>



#if 1 //
int main()
{
    int fd[2] = {0};
    pid_t id = -1;
    size_t total_size = 0;
    int ret = pipe(fd);
    char buf[1024] = {1};
    if (ret <0) {
        perror("pipe");
        return 1;
    }
    
    id = fork();
    if (id > 0) {   //father write
        ssize_t write_size = 0;
        printf("Im father\n");
        close(fd[0]);
        while (1) {
            write_size = write(fd[1], buf, sizeof (buf));
            if (write_size < 0) {
                perror("write");
                return 0;
            }
            total_size += write_size;
            printf("current pipe size is %lu\n", total_size);
        }
        wait(NULL);
    } else if (id == 0) {
        ssize_t read_size = 0;
        printf("Im child\n");
        close(fd[1]);
        sleep(2);
        read_size = read(fd[0], buf, sizeof(buf));
        printf("read from pipe size is %ld\n", read_size);
        sleep(1);
        printf("child done\n");
        return 0;
    }
    return 0;
}
#endif


#if 0 //test pipe size
int main()
{
    int fd[2] = {0};
    pid_t id = -1;
    size_t total_size = 0;
    int ret = pipe(fd);
    char buf[1024] = {1};
    if (ret <0) {
        perror("pipe");
        return 1;
    }
    
    id = fork();
    if (id > 0) {   //father write
        ssize_t write_size = 0;
        printf("Im father\n");
        close(fd[0]);
        while (1) {
            write_size = write(fd[1], buf, sizeof (buf));
            if (write_size < 0) {
                perror("write");
                return 0;
            }
            total_size += write_size;
            printf("current pipe size is %lu\n", total_size);
        }
        wait(NULL);
    } else if (id == 0) {
        ssize_t read_size = 0;
        printf("Im child\n");
        close(fd[1]);
        sleep(3);
        printf("child done\n");
        return 0;
    }
    return 0;
}
#endif //test pipe size
