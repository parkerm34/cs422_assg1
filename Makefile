CC=gcc

LDFLAGS=-lpthread 

CFLAGS=-Wall -g -std=gnu99 -o

sortSeq.o: sortSeq.c
	$(CC) sortSeq.c $(LDFLAGS) $(CFLAGS) -c

sortProcess: sortProcess.c
	$(CC) $(CFLAGS) sortProcess sortProcess.c $(LDFLAGS)

sortThread: sortThread.c
	$(CC) $(CFLAGS) sortThread sortThread.c $(LDFLAGS)

sortSeq: sortSeq.c
	$(CC) $(CFLAGS) sortSeq sortSeq.c $(LDFLAGS)

testsuite: testsuite.c
	$(CC) $(CFLAGS) testsuite testsuite.c

clean: 
	rm -rf sortSeq.o sortSeq testsuite sortProcess sortThread tests/*.out

all:
	$(CC) $(CFLAGS) sortSeq sortSeq.c $(LDFLAGS)
	$(CC) $(CFLAGS) testsuite testsuite.c
	$(CC) $(CFLAGS) sortThread sortThread.c $(LDFLAGS)
	$(CC) $(CFLAGS) sortProcess sortProcess.c $(LDFLAGS)
