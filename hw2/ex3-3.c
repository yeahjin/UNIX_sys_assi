#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>


int main(void) {
	DIR* dir;
	struct dirent* diret;
	char title[10] = "data.txt";
	struct stat bufff;
	int i = 1;
	int fd, n;
	stat("data.txt", &bufff);
	int nowtime = bufff.st_mtime; //시간이 바뀌면 출력하도록 한다.p
	printf("now time :%d\n", nowtime);
	if ((dir = opendir(".")) == NULL) {
		perror("opendir: currnet site");
		exit(1);
	}
	if ((int)S_ISREG(bufff.st_mode) == -1) {
        write(1, "This is not regular file\n", 30);
        return 0;

	}
	if (access("data.txt", F_OK) != -1) {

		if ((bufff.st_mode & S_IRGRP) != 0 || (bufff.st_mode & S_IROTH) != 0 || (bufff.st_mode & S_IWGRP) != 0 || (bufff.st_mode & S_IWOTH) != 0) {
            write(1, "data.txt must be protected\n", 30);
            return 0;
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
							stat("data.txt", &bufff);
							int rightnowtime = bufff.st_mtime;
							if (rightnowtime == nowtime) {
								continue;
							}
                  
                            n = read(fd, buf, 255);
                            buf[n] = '\0';
                            if (n == -1) {
                                perror("Read error");
                            }
                            else if (n == 0) {
                                continue;
                            }
                       
                            write(1, "Recv>> ", 7);
                            write(1, buf, n);
                            if (n == 1 && buf[0] == 'q') {
                                write(1, "Terminate\n", 10);
                                i = 0;
                                return 0;
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
		}

close(fd);
closedir(dir);
return 0;
}