#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>


int main() {
    char userid[10];
    char password[10];
    printf("User id:\n");
    scanf("%s", userid);
    printf("Password:\n");
    scanf("%s", password);
    int fd[2];
    int pid;

    if (pipe(fd) == -1) {
        perror("pipe");
        exit(1);
    }
    if ((pid = fork()) == -1) {
        perror("fork");
        exit(1);
    } else if (pid == 0) {
        close(fd[1]);
        dup2(fd[0], fileno(stdin));
        execlp("validate", "validate", NULL);
        perror("exec");
        exit(1);
    } else {

        close(fd[0]);
        write(fd[1], userid, 10);
        write(fd[1], password, 10);
        close(fd[1]);
        int status;

        if ((wait(&status)) == -1) {
            perror("wait");
            exit(1);
        } else {
            if (WIFEXITED(status)) {
                if (WEXITSTATUS(status) == 0) {
                    printf("Password verified\n");
                } else {
                    printf("Either password or user id is incorrect\n");
                }
            }
        }
    }
    return 0;
}