#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char **argv) {

	int i;
	int n;
	int num_kids;

	if (argc != 2) {
		fprintf(stderr, "Usage: forkloop <numkids>\n");
		exit(1);
	}

	num_kids = atoi(argv[1]);

	printf("Parent\t Child\n");

	for (i = 0; i < num_kids; i++) {
		n = fork();
		if (n < 0) {
			perror("fork");
			exit(1);
		}
		printf("pid = %d, i = %d\n", getpid(), i);

		// printf("%5d -> %d\n", getppid(), getpid());

	}

	return 0;
}
