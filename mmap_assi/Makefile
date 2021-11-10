CC=gcc
CFLAGS=
OBJS=client_mmap.o sort.o
LIBS =
all : svr sort cli

svr: sorter_mmap.o
	$(CC) $(CFLAGS) -o svr sorter_mmap.o $(LIBS)

sort: sort.o
	$(CC) $(CFLAGS) -o sort sort.o $(LIBS)

cli: client_mmap.o
	$(CC) $(CFLAGS) -o cli client_mmap.o $(LIBS)
sorter_mmap.o: sorter_mmap.c
	$(CC) $(CFLAGS) -c sorter_mmap.c
sort.o: sort.c
	$(CC) $(CFLAGS) -c sort.c

client_mmap.o: client_mmap.c
	$(CC) $(CFLAGS) -c client_mmap.c

clean:
	rm -f $(OBJS) sort cli core*
