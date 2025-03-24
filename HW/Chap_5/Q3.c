#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>

int main3(int argc, const char* argv[]) {
    int rc = fork();
    
    if (rc < 0) {
        fprintf(stderr, "fork failed\n");
        exit(1);
    } else if (rc == 0) {
        printf("hello I'm child\n");
    } else {
        sleep(1000);
        printf("goodbye I'm parent\n");
    }
    
    
    return 0;
}
