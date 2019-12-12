#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
	char p1[20], a1[20], p2[20], a2[20], p3[20], a3[20];

	int curr_arg = 1, pora = 0;
	for (int i = 1; i < argc; i = i + 1) {
		if (strcmp(argv[i], "X") == 0) {
			curr_arg = curr_arg + 1;
			pora = 0;
		} else if (strcmp(argv[i], "S") == 0 || strcmp(argv[i], "E") == 0) {
			i = i;
		} else {
			if (curr_arg == 1) {
				if (pora == 0) {
					strcpy(p1, argv[i]);
					pora = pora + 1;
				} else {
					strcpy(a1, argv[i]);
				}
			} else if (curr_arg == 2) {
				if (pora == 0) {
					strcpy(p2, argv[i]);
					pora = pora + 1;
				} else {
					strcpy(a2, argv[i]);
				}
			} else if (curr_arg == 3) {
				if (pora == 0) {
					strcpy(p3, argv[i]);
					pora = pora + 1;
				} else {
					strcpy(a3, argv[i]);
				}
			} else {
				printf("Too many arguments\n");
			}
		}
	}		

	printf("%s %s | %s %s | %s %s\n", p1, a1, p2, a2, p3, a3);

	int fd[2], fe[2];
	pipe(fd);
	pipe(fe);

	int pid = fork(); // create child 1
	if (pid == 0) {
		char * arg1[] = {p1, a1, NULL}; // collect child 1 arguments
		close(fd[0]); // close read of pipe 1
		dup2(fd[1], fileno(stdout)); // dup pipe 1 write to stdout
		execve(p1, arg1, NULL);
		close(fd[1]);
		return 0;
	}

	int pid2 = fork();
	if (pid2 == 0) {
		char * arg2[] = {p2, a2, NULL}; // collect child 2 arguments
		close(fd[1]); // close write of pipe 1
		close(fe[0]); // close read of pipe 2
		dup2(fd[0], fileno(stdin)); // dup pipe 1 read to STDIN
		dup2(fe[1], fileno(stdout)); // dup pipe 2 write to STDOUT
		execve(p2, arg2, NULL); // exec child 2 w arguments
		close(fd[0]);
		close(fe[1]);
		return 0;
	}

	close(fd[0]); close(fd[1]); // close pipe 1
	int pid3 = fork();
	if (pid3 == 0) {
		char * arg3[] = {p3, NULL}; // collect child 3 arguments
		close(fe[1]); // close write of pipe 2
		dup2(fe[0], fileno(stdin)); // dup pipe 2 read to STDIN
		execve(p3, arg3, NULL); // exec child 3 w arguments
		close(fe[1]);
		return 0;
	}

	close(fe[0]); close(fe[1]); // close pipe 2

	// wait for all children
	waitpid(pid);
	waitpid(pid2);
	waitpid(pid3);

	return 0;
}
