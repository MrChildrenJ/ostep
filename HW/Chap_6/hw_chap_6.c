#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sched.h>
#include <pthread.h>
#include <time.h>
#include <sys/wait.h>
#include <linux/sched.h>

void measure_time(void) {
  struct timeval start, end;
  long long elapsed_time;
  int iterations = 1000000;

  gettimeofday(&start, NULL);

  for (int i = 0; i < iterations; i++) {
    gettimeofday(&end, NULL);
  }

  elapsed_time = (end.tv_sec - start.tv_sec) * 1000000LL + (end.tv_usec - start.tv_usec);

  printf("Total time for %d gettimeofday() calls: %lld microseconds\n", iterations, elapsed_time);
  printf("Average time per call: %.3f nanoseconds\n", (double)elapsed_time * 1000 / iterations);
}

void measure_syscall_cost() {
  struct timeval start, end;
  long long elapsed_time;
  int iterations = 1000000;
  int fd;
  char buf[1];

  fd = open("/dev/null", O_RDONLY);
  gettimeofday(&start, NULL);

  for (int i = 0; i < iterations; i++) {
    read(fd, buf, 0);  // We don't really need to read a byte.
  }

  gettimeofday(&end, NULL);
  close(fd);
  elapsed_time = (end.tv_sec - start.tv_sec) * 1000000LL + (end.tv_usec - start.tv_usec);

  printf("Total time for %d read() calls: %lld microseconds\n", iterations, elapsed_time);
  printf("Average time per call: %.3f nanoseconds\n", (double)elapsed_time * 1000 / iterations);
}

/**
 * int sched_setaffinity(pid_t pid, size_t cpusetsize, const cpu_set_t *mask);
 * @brief Sets the CPU affinity of a process
 *
 * @details This function allows binding a process to specific CPU cores,
 *          which restricts the process to run only on the specified cores.
 *          CPU affinity is useful for performance optimization, resource isolation,
 *          and consistent benchmarking.
 *
 * @param pid Process ID of the target process. If set to 0, it refers to the
 *            current process.
 * @param cpusetsize Size of the CPU set, typically sizeof(cpu_set_t).
 * @param mask Pointer to a cpu_set_t structure that specifies which CPUs
 *             the process may run on. This is a bitmask where each bit
 *             represents a CPU core.
 *
 * @return Returns 0 on success. On error, -1 is returned, and errno is set
 *         to indicate the error.
 *
 * @note Before calling this function, initialize the cpu_set_t with CPU_ZERO()
 *       and set the desired CPUs using CPU_SET().
 *
 * @warning Improper use may lead to poor system performance if critical
 *          processes are restricted inappropriately.
 *
 * @see sched_getaffinity(), CPU_ZERO(), CPU_SET(), CPU_CLR(), CPU_ISSET()
 */
void measure_context_switch() {
  struct timeval start, end;
  long long elapsed_time;
  int iterations = 1000000;
  int pipe1[2], pipe2[2];        // pipe1: parent -> child, pipe2: child -> parent
  pid_t pid;
  char buf[1] = {'A'};
  int i;
  cpu_set_t set;

  CPU_ZERO(&set);
  CPU_SET(0, &set);
  if (sched_setaffinity(0, sizeof(set), &set) < 0) {
    perror("Failed to set CPU affinity");
    return;
  }

  if (pipe(pipe1) < 0 || pipe(pipe2) < 0) {
    perror("Failed to create pipes");
    return;
  }

  printf("\nMeasuring context switch cost with %d iterations...\n", iterations);

  pid = fork();
  if (pid < 0) {
    perror("Failed to fork");
    return;
  }
  if (pid == 0) {
    // Lock child to the same CPU as the parent
    if (sched_setaffinity(0, sizeof(set), &set) < 0) {
      perror("Child: Failed to set CPU affinity");
      exit(1);
    }

    // 關閉後，pipe1只剩下讀，pipe2只剩下寫，所以child要讀只能從pipe1[0]，要寫只能從pipe2[1]
    close(pipe1[1]);
    close(pipe2[0]);

    for (i = 0; i < iterations; i++) {
      if (read(pipe1[0], buf, 1) < 0) {
        perror("Child: Failed to read from pipe");
        exit(1);
      }
      if (write(pipe2[1], buf, 1) < 0) {
        perror("Child: Failed to write to pipe");
        exit(1);
      }
    }

    // Release resources; avoid memory leak; to tell read end there is no more data to read; send EOF
    close(pipe1[0]);
    close(pipe2[1]);
    exit(0);
  }
  else {  // Parent
    // close read of pipe1, close write of pipe2 -> parent can only read from pipe2, write to pipe1
    close(pipe1[0]);
    close(pipe2[1]);

    gettimeofday(&start, NULL);

    for (i = 0; i < iterations; i++) {
      // Write to child
      if (write(pipe1[1], buf, 1) < 0) {
        perror("Parent: Failed to write to pipe");
        break;
      }

      // Read from child
      if (read(pipe2[0], buf, 1) < 0) {
        perror("Parent: Failed to read from pipe");
        break;
      }
    }

    gettimeofday(&end, NULL);

    close(pipe1[1]);
    close(pipe2[0]);

    // 等待子進程結束
    waitpid(pid, NULL, 0);

    // 計算時間
    elapsed_time = (end.tv_sec - start.tv_sec) * 1000000LL + (end.tv_usec - start.tv_usec);
    printf("Total time for %d context switches: %lld microseconds\n", iterations * 2, elapsed_time);
    printf("Average time per context switch: %.3f nanoseconds\n", (double)elapsed_time * 1000 / (iterations * 2));
  }
}

int main() {
  measure_time();
  measure_syscall_cost();
  measure_context_switch();
  return 0;
}
