#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

int children_running;

void sigchld_handler(int s) {
    int status;

    if ((wait(&status)) == -1) {
        perror("wait");
    } else if (WIFEXITED(status)) {
        printf("child exited with status %d\n", WEXITSTATUS(status));
    } else {
        printf("child exited abnormally\n");
    }
    children_running--;
}

void do_child(int cnum) {
    printf("Child %d\n", cnum);
    sleep(2 * cnum);
    exit(cnum);
}

int main(int argc, char **argv) {
    int i;
    pid_t pid;
    struct sigaction sa;
    int numkids;

    if (argc != 2) {
        fprintf(stderr, "Usage: sigchld <num children>\n");
        exit(1);
    }
    numkids = atoi(argv[1]);

    /* Turn off buffering */
    setbuf(stdout, NULL);

    /* install signal handler */
    sa.sa_handler = sigchld_handler; /* reap all dead processes */
    sigemptyset(&sa.sa_mask);
    //sa.sa_flags = SA_RESTART | SA_NOCLDSTOP;
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        perror("sigaction"); exit(1);
    }

    /* create some children */
    for (i = 0; i < numkids; i++) {
        if ((pid = fork()) == -1) {
            perror("fork"); exit(1);

        } else if (pid == 0) {
            /* child */
            printf("child %d created\n", i);
            do_child(i);
        }
        children_running++;
    }

    /* compute for a while */
    i = 0;
    while (children_running > 0) {
        i++;
        if (i % 10000000 == 0)
            putchar('.');
    }
    printf("\ndone computing\n");

    return 0;
}

