CC=gcc
CFLAGS=-I.

all: cp1 cp2
	chmod +x cp1 cp2
clean:
	rm -rf *.o cp1 cp2
cp1: cp1.c
	${CC} ${CFLAGS} -o cp1 cp1.c 

cp2: cp2.c
	${CC} ${CFLAGS} -o cp2 cp2.c