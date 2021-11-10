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
    printf("sorter\n");
    char *filename = argv[1];
    FILE *fp;
    int size = atoi(argv[2]);
    int fd;
    char* addr;
    printf("%s,%d\n",filename, size);
    // carry file information
    struct stat statbuf;
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
    // deep copy (addr -> addrcpy)
    char* addrcpy = malloc(size);
    memcpy(addrcpy,addr,size);
    // cut part of using "\n"
    int i = 0;
    char *p = strtok(addrcpy,"\n");
    char *array[size];
    while (p != NULL){
        array[i++] = p;
        p = strtok(NULL,"\n");
    }
    // count until just before NULL 
    int end;
    for (i = 0; i < size; ++i){
        if (array[i] == NULL){
            break;}
        end = i;
    }
    // string sort
	for(int i=0; i<end; i++ )
	{
		for(int j=0; j<end-i; j++ )
		{
            if (array[j] == NULL) break;
			if( strcmp( array[j], array[j+1]) > 0 )
			{
				char* tmp = array[j];
				array[j] = array[j+1];
				array[j+1] = tmp ;
			}
		}
	}
    // output print
    char addr2[size];
    for (i = 0; i < end+1; ++i){;
        //printf("%s\n",array[i]);
        strcat(addr2,array[i]);
        strcat(addr2,"\n");
    }
    close(fd);
    strcpy(addr,addr2);
    printf("%s",addr);

    //memcpy(addr,addr2)
    return 0;
}