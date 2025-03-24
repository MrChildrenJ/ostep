//
//  Q2.c
//  Chap 05
//
//  Created by JJ Huang on 1/21/25.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>

int main2(int argc, const char* argv[]) {
    int fd;
    int rc;
    
    fd = open("q2.txt", O_CREAT | O_WRONLY | O_TRUNC, 0644);
    // O_CREAT：If file doesn't exist, create it
    // O_WRONLY：Write only
    // O_TRUNC: If exists, clear it
    // S_IRUSR (00400) → 擁有者可讀
    // S_IWUSR (00200) → 擁有者可寫
    // S_IXUSR (00100) → 擁有者可執行
    // S_IRWXU (00700) → 可讀 (r)、可寫 (w)、可執行 (x)
    /*
     0        // 八進位前綴
     6 = 110  // 擁有者 (user) 可讀寫 (rw-)
     4 = 100  // 群組 (group) 只讀 (r--)
     4 = 100  // 其他人 (others) 只讀 (r--)
     */
    
    if (fd < 0) {
        perror("open failed");
        exit(1);
    }
    
    rc = fork();
    
    if (rc < 0) {
        perror("fork failed");
        exit(1);
    }
    
    const char* msg = (rc == 0) ? "Child is writing\n" : "Parent is writing\n";
    
    for (int i = 0; i < 1; i++) {
        write(fd, msg, strlen(msg));
        usleep(500);
    }
    
    close(fd);
    return 0;
}
