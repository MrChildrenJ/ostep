/*
Q8. Write a program that creates two children, and connects the standard output of one to the standard input
	of the other, using the pipe() system call.

Note: int dup2(int oldfd, int newfd);	// duplicate oldfd to newfd!
	  int pipe(int fd[2]);				// [read, write]
 */

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(int argc, const char* argv[]) {
    int fds[2];			// fds[0] -> read, fds[1] -> write

    if (pipe(fds) == -1) {
      perror("pipe failed");
      exit(1);
    }

    // Parent get input
    char buffer[256];
    printf("Enter a string: ");
    fflush(stdout);
    fgets(buffer, 256, stdin);

    int rc1 = fork();
    if (rc1 < 0) {
        fprintf(stderr, "fork failed\n");
        exit(1);
    }
    if (rc1 == 0) {
      close(fds[0]);					// close child 1's read end, otherwise we will never send EOF
      dup2(fds[1], STDOUT_FILENO);		// Redirect stdout to the pipe
      close(fds[1]);

      printf("%s\n", buffer);
      fflush(stdout);
      exit(0);
    }

    int rc2 = fork();
    if (rc2 < 0) {
        fprintf(stderr, "fork failed\n");
        exit(1);
    }
    if (rc2 == 0) {
      close(fds[1]);					// if fds[1] (the write end of the pipe) remains open in child 2,
                                       	// the pipe will never signal EOF (End-of-File),
      									// which can cause fgets() or read() to block indefinitely.
      dup2(fds[0], STDIN_FILENO);		// Redirect stdin to the pipe
      close(fds[0]);

      char recv_buffer[256];
      fgets(recv_buffer, 256, stdin);
      printf("Child 2 receives: %s\n", recv_buffer);
      exit(0);
    }

    close(fds[0]);
    close(fds[1]);

    wait(NULL);
    wait(NULL);
    printf("Goodbye from parent!\n");

    return 0;
}