#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>

int main(int argc, char *argv[]) {
    // syscall, open and create, should return integer > 0
    int fd = open("/tmp/file", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    assert(fd >= 0);

    char buffer[20];
    sprintf(buffer, "hello world\n");   // formatted output to a string (character array) i/o stdout
    
    // syscall, return the number of bytes written, and if an error occurs, it returns -1.
    int rc = write(fd, buffer, strlen(buffer));
    assert(rc == (strlen(buffer)));

    // fsync() forces the buffered changes to be written to the physical disk or other storage media, 
    // ensuring data persistence even in the event of a system crash or power failure
    fsync(fd);
    close(fd);
    return 0;
}