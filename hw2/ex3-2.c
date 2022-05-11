#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main(void) {
	int fd, n;
	char buf[256];
	struct stat bufff;
	mode_t mode;
	int per;
	mode = S_IRUSR | S_IWUSR;
	fd = open("data.txt", O_CREAT | O_WRONLY | O_TRUNC, mode);
	if (fd == -1) {
		perror("Open data.txt");
		exit(1);
	}
	stat("data.txt", & bufff);
	if ((int)S_ISREG(bufff.st_mode)==-1) {
		write(1, "This is not regular file\n", 30);
		return 0;

	}
	if (access("data.txt", F_OK)!= -1) {
		if ((bufff.st_mode & S_IRGRP)!=0 || (bufff.st_mode & S_IROTH)!=0 || (bufff.st_mode & S_IWGRP)!=0 || (bufff.st_mode & S_IWOTH)!=0) {
			write(1, "data.txt must be protected\n", 30);
			return 0;
		}
		while (1) {
			fchmod(fd, S_IRUSR | S_IWUSR);
			write(1, ">> ", 3);
			n = read(0, buf, 255);
			buf[n] = '\0';
			if (n > 0) {
				if (write(fd, buf, n) != n) {
					perror("Write error");
				}
			}
			else if (n == -1) {
				perror("Read error");
			}
			if (n == 1 && buf[0] == 'q') {
				write(1, "Terminate\n", 10);
				break;
			}
			write(1, buf, n);
		}

		close(fd);

	}
	return 0;
}