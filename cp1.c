#include        <stdio.h>
#include        <unistd.h>
#include        <fcntl.h>
#include 		<stdlib.h>
#include 		<stdbool.h>
#include 		<sys/types.h>
#include 		<sys/stat.h>
#include 		<errno.h>

#define BUFFERSIZE      4096
#define COPYMODE        0644

void oops(char *, char *);
bool is_same_file(char *src, char *dest);

main(int ac, char *av[])
{
	int     in_fd, out_fd, n_chars;
	char    buf[BUFFERSIZE];
	
	if ( ac != 3 ){
		fprintf( stderr, "usage: %s source destination\n", *av);
		exit(1);
	}
	/*
	 * Don't copy in case the destination is same as source file.
	 */
	if(is_same_file(av[1], av[2])) {
		fprintf(stderr, "'%s' and '%s' are the same file\n", av[1], av[2]);
		exit(1);
	}
	
	if ( (in_fd=open(av[1], O_RDONLY)) == -1 )
		oops("Cannot open ", av[1]);
	
	if ( (out_fd=creat( av[2], COPYMODE)) == -1 )
		oops( "Cannot creat", av[2]);
	
	while ( (n_chars = read(in_fd , buf, BUFFERSIZE)) > 0 )
		if ( write( out_fd, buf, n_chars ) != n_chars )
			oops("Write error to ", av[2]);
	if ( n_chars == -1 )
		oops("Read error from ", av[1]);
	
	
	if ( close(in_fd) == -1 || close(out_fd) == -1 )
		oops("Error closing files","");
}

void oops(char *s1, char *s2)
{
	fprintf(stderr,"Error: %s ", s1);
	perror(s2);
	exit(1);
}

/*
 * Compare two file by file descriptor to make sure it same file or not.
 * f1 : file descriptor of the file1.
 * f2 : file descriptor of the file2.
 * return true if f1 and f2 is the same file false if not same.
 */
bool is_same_file(char *src, char *dest) 
{
	struct stat s_stat; /*source stat */
	struct stat d_stat; /*destination stat */
	
	/*
	 * Get ﬁles status to compare
	 */
	if (lstat(src, &s_stat) < 0) {
		if (errno != ENOENT) {
			oops("can't stat '%s'", src);
		}
	}
	
	if (lstat(dest, &d_stat) < 0) {
		if (errno != ENOENT) {
			oops("can't stat '%s'", dest);
		}
	}
	/* two ﬁles are the same if they are on 
	 * the same device and have the same i-node number.
	 */
	if (s_stat.st_dev == d_stat.st_dev
		&& s_stat.st_ino == d_stat.st_ino) {
			return true;
	}

	/*
	 * Two files is not same
	 */
	return false;
}
