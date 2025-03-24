/*
Q4. Write a program that calls fork() and then calls some form of exec() to run the program /bin/ls.
See if you can try all of the variants of exec(), including (on Linux) execl(), execle(), execlp(),
execv(), execvp(), and execvpe().
Why do you think there are so many variants of the same basic call?

Q5. Now write a program that uses wait() to wait for the child process to finish in the parent.
What does wait() return? What happens if you use wait() in the child?

Ans: wait() returns the pid of the child process that has terminated, which the parent can use to know which child has finished.
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

    else if (rc == 0) {                    // child
        fprintf(stdout, "hello I'm child\n");
        char* args[] = {"ls", NULL};
        if (execv("/bin/ls", args) == -1) {
          perror("execv failed");
          exit(1);
        }
    }

    else {
      // kernel will save the child's pid in the process table
      int status;
      // parent call wait() to get the pid of the child, then save it to "status"
      pid_t child_pid = wait(&status);

      if (child_pid == -1)    perror("wait failed");
      else                    printf("child pid = %d, status = %d\n", child_pid, status);

      printf("goodbye I'm parent\n");
    }
    return 0;
}