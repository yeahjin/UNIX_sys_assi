#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <sys/mman.h>
#include <sys/wait.h>
void regularfilefunc(char *filetitle){ // regular file input function
    int fd, n;
    DIR* dir;
    char buf[256];
    struct stat bufff;
    mode_t mode;
    struct dirent* dent;
    int size;
    mode = S_IRWXU|S_IRWXG|S_IRWXO;

    fd = open(filetitle, O_CREAT | O_RDWR | O_TRUNC, mode);
    if (fd == -1) {
        perror("Open file");
        exit(1);
    }
    while (1) {
        stat(filetitle, &bufff);
        size = (int)bufff.st_size;
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
        if (n == 5 && buf[0] == '%' && buf[1] == 's' && buf[2] == 'o' && buf[3] == 'r' && buf[4] == 't') {
            write(1, "Terminate\n", 10);
            break;
        }
        write(1, buf, n);
    }

}
int main(int argc, char *argv[])
{
    int a;
    extern char *optarg;
    extern int optind;
    char* val;
    char *val1;
    char* val2;
    pid_t pid;
    char *childargv[3];
    val1 = "data.mmap";
    val2 = "4098";
    while ((a = getopt(argc, argv, "t:f:s:")) != -1) // argument split
    {

        switch (a)
        {
        case 't': // file option(regular, fifo, dirctory...)
            val = "r.map";
            break;
        case 'f': // file path
            val1 = optarg;
            if (val1 == NULL){
                val1 = getenv("SORT_COMM");
                if (val1 == NULL){
                    val1 = "data.mmap";
                }
            }
            break;
        case 's': // file size
            val2 = optarg;
            if (val2 == NULL){
                val2 =  getenv("SORT_COMM_BUF");
                if (val2 == NULL){
                    val2 = "4096";
                }
            }
            break;
        }
    }
    regularfilefunc(val1);
    int status;
    // fork & exec
    switch (pid = fork())
    {
    case -1:
        perror("fork");
        exit(1);
        break;

    case 0:
        printf("child process -My PID: %d\n", (int)getpid());
        childargv[0] = val1;
        childargv[1] = val2;
        childargv[3] = NULL;
        // call the "./client_mmap"
        if (execv("./client_mmap", childargv) == -1)
        {
            perror("execve");
            exit(1);
        }
        break;

    default:
        // process synchronization
        while(wait(&status)!= pid)
        continue;
        printf("Parent process - My PID: %d\n", (int)getpid());
    }
    return 0;
}
