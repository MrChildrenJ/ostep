/*
Q6. Write a slight modification of the previous program, this time us- ing waitpid() instead of wait().
When would waitpid() be useful?
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
      pid_t child_pid = rc;
      int status;

      pid_t waited_pid = waitpid(child_pid, &status, 0);

      if (waited_pid == -1)    perror("wait failed");
      else                     printf("child pid = %d, status = %d\n", child_pid, status);

      printf("goodbye I'm parent (pid: %d)\n", getpid());
    }
    return 0;
}