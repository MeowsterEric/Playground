CC = gcc
PORT=30001
CFLAGS = -DPORT=$(PORT) -g -Wall

all: fileserver fileclient 

fileserver: fileserver.o wrapsock.o writen.o readn.o
	${CC} ${CFLAGS} -o $@ fileserver.o  wrapsock.o writen.o readn.o

fileclient: fileclient.o wrapsock.o writen.o readn.o
	${CC} ${CFLAGS} -o $@ fileclient.o wrapsock.o writen.o readn.o

.c.o:
	${CC} ${CFLAGS}  -c $<

# The - in front of rm tells make to ignore errors and keep going.
clean:
	-rm *.o 

realclean:
	make clean
	rm fileserver fileclient

