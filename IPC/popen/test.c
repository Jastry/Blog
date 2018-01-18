#include "Popen.h"
//#define PAGER "${PAGER:-more}" /* environment variable, or default*/
#define PAGER "${date}" /* environment variable, or default*/

int main(int argc, char *argv[])
{
    const int MAX_LINE = 1024;
    char line[MAX_LINE];
    FILE *fpin, *fpout;

    if (argc != 2) {
        printf("usage : %s [pathname]\n", argv[0]);
        return 1;
    }
    if (NULL == (fpin = fopen(argv[1], "r"))) {
        perror("open");
        return 2;
    }

    if (NULL == (fpout = Popen("date", "r"))) {
        perror("popen");
        return 3;
    }
    ssize_t read_size = fread(line, sizeof(line), 1, fpout);
    line[read_size - 1] = 0;
    printf("%s\n", line);
    /* copy argv[1] to page*/
   // while (fgets(line, MAX_LINE, fpin) != NULL) {
   //     if (fputs(line, fpout) == EOF) {
   //         perror("fputs error to pipe\n");
   //         return 4;
   //     }
   // }
   // if (ferror(fpin)) {
   //     perror("fgets error");
   //     return 5;
   // }
    if (Pclose(fpout) == -1) {
        perror("pclose");
        return 6;
    }
    return 0;
}
