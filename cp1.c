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
void sync_file_permission(char *src, char *dest);

int main(int ac, char *av[])
{
    int     in_fd, out_fd, n_chars;
    char    buf[BUFFERSIZE];

    if ( ac != 3 ) {
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

    //change file permission
    sync_file_permission(av[1], av[2]);

    return 0;
}

void oops(char *s1, char *s2)
{
    fprintf(stderr,"Error: %s ", s1);
    perror(s2);
    exit(1);
}

/*
 * Compare two file to make sure it same file or not.
 * src : file name or full path of source file.
 * dest : file name or full path of destination file.
 * return true if src and dest is the same file and fale if not.
 */
bool is_same_file(char *src, char *dest)
{
    struct stat s_stat; /*source stat */
    struct stat d_stat; /*destination stat */
    int f1 = 0;
    int f2 = 0;

    /*
     * Open src and dest in read only mode to get file status
     */
    if ( (f1=open(src, O_RDONLY)) == -1 )
        oops("Cannot open ", src);

    /*
     * Get ﬁles status to compare
     */
    if (fstat(f1, &s_stat) < 0) {
        if (errno != ENOENT) {
            oops("can't stat '%s'", src);
        }
    }

    if ( (f2=open(dest, O_RDONLY)) == -1 ) {
        //fprintf(stderr,"Info: '%s' not existed\n", dest);
        if(stat(dest, &d_stat) < 0) {
            if (errno != ENOENT) {
                oops("can't stat '%s'", dest);
            }
        }

    } else {
        if (fstat(f2, &d_stat) < 0) {
            if (errno != ENOENT) {
                oops("can't stat '%s'", dest);
            }
        }
    }

    /*
     * Close two file after get status
     */
    if(f1 != 0 && f1 != -1) {
        if ( close(f1) == -1 )
            oops("Error closing files","");
    }
    if(f2 !=0 && f2 != -1) {
        if ( close(f2) == -1 )
            oops("Error closing files","");
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

/*
 * Copy file owner and permission.
 * src pointer to the file path of the source file
 * dest pointer to the file path of the destination file
 * return Null
 */
void sync_file_permission(char *src, char *dest)
{
    struct stat s_stat; /*source stat */
    struct stat d_stat; /*destination stat */

    /*
     * Get source file status.
     */
    if (stat(src, &s_stat) < 0) {
        if (errno != ENOENT) {
            oops("can't stat '%s'", src);
        }
    }

    /*
     * Get destination file status.
     */
    if (stat(dest, &d_stat) < 0) {
        if (errno != ENOENT) {
            oops("can't stat '%s'", dest);
        }
    }

    /*
     * Copy file's owner.
     */
    if (chown(dest, s_stat.st_uid, s_stat.st_gid) < 0) {
        s_stat.st_mode &= ~(S_ISUID | S_ISGID);
        oops("can't preserve ownership of '%s'", dest);
    }

    /*
     * Copy file's permission.
     */
    if (chmod(dest, s_stat.st_mode) < 0)
        oops("can't preserve permissions of '%s'", dest);

    //return 0;
}