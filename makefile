# makefile -- rules to build OSP workshop D
# to use simply type "make"
# this will build the server and client and launcher script
# note, this is a configuration file for the MAKE utility
# do not try to run it directly
# if typing up the file, the indented lines need to be indented
# with TABS not spaces.
CC=gcc
CFLAGS=-g -Wall

all: cp1

clean:
	rm -f *.o cp1

cp1: cp1.c
	${CC} ${CFLAGS} -o cp1 cp1.c
