#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
	char *p1[3][10];
	int curr_arg = 0, curr_prog = 0;

	for (int i = 2; i < argc; i += 1) {
		if (strcmp(argv[i], "X") == 0) {
			curr_arg = 0;
			curr_prog += 1;
			p1[curr_prog][curr_arg] = NULL;
		} else if (strcmp(argv[i], "E") == 0) {
			p1[curr_prog][curr_arg] = NULL;
			break;
		} else {
			p1[curr_prog][curr_arg] = malloc(1024 * sizeof(char));
			strcpy(p1[curr_prog][curr_arg], argv[i]);
			curr_arg += 1;
		}
	}

	int fd[2], fe[2];
	pipe(fd);
	pipe(fe);

	int pid = fork(); // create child 1
	if (pid == 0) {
		close(fd[0]); // close read of pipe 1
		dup2(fd[1], fileno(stdout)); // dup pipe 1 write to stdout
		execv(p1[0][0], p1[0]);
		close(fd[1]);
		return 0;
	}

	int pid2 = fork();
	if (pid2 == 0) {
		close(fd[1]); // close write of pipe 1
		close(fe[0]); // close read of pipe 2
		dup2(fd[0], fileno(stdin)); // dup pipe 1 read to STDIN
		dup2(fe[1], fileno(stdout)); // dup pipe 2 write to STDOUT
		execv(p1[1][0], p1[1]); // exec child 2 w arguments
		close(fd[0]);
		close(fe[1]);
		return 0;
	}

	close(fd[0]); close(fd[1]); // close pipe 1
	int pid3 = fork();
	if (pid3 == 0) {
		close(fe[1]); // close write of pipe 2
		dup2(fe[0], fileno(stdin)); // dup pipe 2 read to STDIN
		execv(p1[2][0], p1[2]); // exec child 3 w arguments
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
