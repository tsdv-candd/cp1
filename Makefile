CC=gcc
CFLAGS=-Wall

all: cp1
clean:
	rm -rf *.o cp1
cp1: cp1.c
	${CC} ${CFLAGS} -o cp1 cp1.c 