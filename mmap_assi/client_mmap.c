#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <sys/mman.h>

int main(int argc, char *argv[]){
    char* filename = argv[0];
    int size= atoi(argv[1]); // change char pointer type to integer type
    int fd;
    char* addr;
    struct stat statbuf;
    // carry file information
    if (stat(filename,&statbuf) == -1){
        perror("stat");
        exit(1);
    }
    // opne file using file descriptor
    if((fd = open(filename,O_RDWR))==-1){
        perror("open");
        exit(1);
    }
    // share mmap
    addr = mmap(NULL, size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, (off_t)0);
    if (addr == MAP_FAILED){
        perror("mmap");
        exit(1);
    }
    // mmap synchronization
    while (1){
        // The first character of this string is a newline character '%'.
        if (addr[0] == '%') break;
        msync(addr,size,MS_SYNC);
    }
    // output
    printf("%s",addr);
    // unmapping
    if (munmap(addr,size)==-1){
        perror("munmap");
        exit(1);
    }

    close(fd);
    return 0;
}