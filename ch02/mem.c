#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "common.h"

int main(int argc, char *argv[]) {
    int *p = malloc(sizeof(int));

    if (argc == 1)  *p = 0;
    else if (argc == 2) *p = atoi(argv[1]);
    else { 
        fprintf(stderr, "usage: mem (<value>)\n"); 
        exit(1); 
    } 
    
    assert(p != NULL);
    printf("(%d) addr pointed to by p: %p\n", (int) getpid(), p);

    // *p = atoi(argv[1]); // assign value to addr stored in p
    while (1) {
        Spin(1);
        *p = *p + 1;
        printf("(%d) value of p: %d\n", getpid(), *p);
    }
    
    return 0;
}