//
//  Q1.c
//  Chap 05
//
//  Created by JJ Huang on 1/21/25.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int main1(int argc, const char* argv[]) {
    int x = 100;
    int rc = fork();
    if (rc < 0) {
        fprintf(stderr, "fork failed\n");
        exit(1);
    }
    
    else if (rc == 0) {
        x += 66;
        printf("I'm child %d, x = %d\n", (int) getpid(), x);
    }
    
    else {
        x += 88;
        printf("I'm parent %d, x = %d\n", (int) getpid(), x);
    }
    
    return 0;
}
