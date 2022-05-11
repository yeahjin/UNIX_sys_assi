#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>

void regularfilefunc(char* filetitle) {
	int fd, n;
	DIR* dir;
	char buf[256];
	struct stat bufff;
	mode_t mode;
	int per;
	int i = 0;
	struct dirent* dent;
	int size;
	mode = S_IRUSR | S_IWUSR;

	if ((dir = opendir(".")) == NULL) {
		perror("opendir: .");
		exit(1);
	}

	while ((dent = readdir(dir))) {
		if (strcmp(dent->d_name, filetitle) == 0) {
			stat(filetitle, &bufff);
			if ((int)S_ISREG(bufff.st_mode) == -1) {
				write(1, "This is not regular file\n", 30);
				exit(1);
			}
			if ((bufff.st_mode & S_IRGRP) != 0 || (bufff.st_mode & S_IROTH) != 0 || (bufff.st_mode & S_IWGRP) != 0 || (bufff.st_mode & S_IWOTH) != 0) {
				write(1, "data.txt must be protected\n", 30);
				exit(1);
			}
			break;
		}
	}

	while (1) {
		stat(filetitle, &bufff);
		size = (int)bufff.st_size;
		if (size == 0) {
			fd = open(filetitle, O_RDONLY, mode);
			if (fd == -1) {
				perror("Open error!");
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
					break;
				}
			}
		}
		if (n == 1 && buf[0] == 'q') {
			write(1, "Terminate\n", 10);
			break;
		}
	}
	close(fd);
	closedir(dir);


}

void fifofilefunc(char* filetitle) {
	int fd, n;
	char buf[256];
	struct stat bufff;
	DIR* dir;
	mode_t mode;
	int per;
	int size;
	int i;
	struct dirent* dent;
	mode = S_IRUSR | S_IWUSR;
	int nowtime = bufff.st_mtime;
	if ((dir = opendir(".")) == NULL) {
		perror("opendir : current dir");
		exit(1);
	}

	while ((dent = readdir(dir))) {
		if (strcmp(dent->d_name, filetitle) == 0) {
			stat(filetitle, &bufff);
			if ((int)S_ISFIFO(bufff.st_mode) == -1) {
				write(1, "This is not fifo file\n", 30);
				exit(1);
			}
			if ((bufff.st_mode & S_IRGRP) != 0 || (bufff.st_mode & S_IROTH) != 0 || (bufff.st_mode & S_IWGRP) != 0 || (bufff.st_mode & S_IWOTH) != 0) {
				write(1, "fifo file must be protected\n", 30);
				exit(1);
			}
			break;
		}
		else {
			if (mkfifo(filetitle, mode) == -1) {
				perror("mkfifo");
				exit(1);
			}
		}
	}

	while (1) {
		stat(filetitle, &bufff);
		size = (int)bufff.st_size;
		if (size == 0) {
			fd = open(filetitle, O_RDONLY, mode);
			if (fd == -1) {
				perror("Open error!");
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
					break;
				}
			}
		}
		if (n == 1 && buf[0] == 'q') {
			write(1, "Terminate\n", 10);
			break;
		}
	}

	close(fd);
	closedir(dir);

}

int main(int argc, char* argv[]) {
	int a;
	extern char* optarg;
	extern int optind;
	char* val;
	int b = 1;

	val = getenv("COM_FILE");

	while ((a = getopt(argc, argv, "t:f:")) != -1) {
		switch (a)
		{
		case 't':
			if (strcmp(optarg, "f") == 0) {
				printf("fifo mode\n");
				b = 2;
			}
			else if (strcmp(optarg, "r") == 0) {
				printf("regular mode\n");
				b = 1;
			}
			break;
		case 'f':
			val = optarg;
			break;
		}
	}

	if (b == 1) {
		if (val == NULL) {
			val = "data.txt";
		}
		regularfilefunc(val);
	}
	else if (b == 2) {
		if (val == NULL) {
			val = "data.fifo";
		}
		fifofilefunc(val);
	}

	return 0;
}