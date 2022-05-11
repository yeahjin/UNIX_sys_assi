#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>


int main(void) {
    DIR *dir;
    struct dirent* diret;
    char title[10] = "data.txt";
    int i = 1;
    int fd, n;
    if ((dir = opendir(".")) == NULL) {
        perror("opendir: currnet site");
        exit(1);
    }
    while (i) {
        while ((diret = readdir(dir))) {
            if (strcmp(diret->d_name, title) == 0) {

                char buf[256];

                fd = open("data.txt", O_RDONLY);
                if (fd == -1) {
                    perror("Open data.txt");
                    exit(1);
                }

                while (1) {
                    n = read(fd, buf, 255);
                    buf[n] = '\0';
                    if (n == -1) {
                        perror("Read error");
                    }
                    else if (n == 0) continue;
                    write(1, "Recv>> ", 7);
                    write(1, buf, n);
                    if (n == 1 && buf[0] == 'q') {
                        write(1, "Terminate\n", 10);
                        i = 0;
                        break;
                    }
                }
                
                break;
            }
            else {
                write(1, "it's not data.txt\n", 18);
                continue;
            }
        }
    rewinddir(dir);
    }
    close(fd);
    closedir(dir);
    return 0;
}