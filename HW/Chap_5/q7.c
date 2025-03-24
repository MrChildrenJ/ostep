/*
Q7. Write a program that creates a child process, and then in the child closes standard output (STDOUT FILENO).
What happens if the child calls printf() to print some output after closing the descriptor?

Ans: No output
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>


int main(int argc, const char* argv[]) {
    int rc = fork();

    if (rc < 0) {
        fprintf(stderr, "fork failed\n");
        exit(1);
    }

    else if (rc == 0) {		// child
      close(STDOUT_FILENO);
      printf("hello I'm child\n");
    }

    else {
      wait(NULL);
      printf("goodbye I'm parent (pid: %d)\n", getpid());
    }
    return 0;
}