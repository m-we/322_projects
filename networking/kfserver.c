// Reads in keys from a file, checks if client input matches these.

#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
	// Read in keys from file.
	FILE *fp = fopen(argv[1], "r");
	char word[1024];
	char keys[10][1024];
	int i = 0;
	while (fscanf(fp, "%1023[0-z]%*[^0-z]", keys[i]) == 1 && i < 10)
		i += 1;
	fclose(fp);

	struct sockaddr_in addr;
	int fd, sock, addrlen = sizeof(addr);
	char buffer[1024] = {0};

	if ((fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
		printf("kfserver: Sock err.\n");
		exit(-1);
	}

	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(atoi(argv[2]));

	if (bind(fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
		printf("kfserver: Bind err.\n");
		exit(-1);
	}

	if (listen(fd, 3) < 0) {
		printf("kfserver: Listen err.\n");
		exit(-1);
	}

	while (1) {
		if ((sock = accept(fd, (struct sockaddr *)&addr, (socklen_t*)&addrlen)) < 0) {
			printf("kfserver: Accept err.\n");
			exit(-1);
		}

		while (1) {
			read(sock, buffer, 1);
			if (buffer[0] == '~') {
				close(sock);
				break;
			} else if (buffer[0] == ' ') {
				int match = 0;
				for (int j = 0; j <= i; j++) {
					if (strcasecmp(keys[j], word) == 0) {
						send(sock, word, sizeof(word), 0);
						match = 1;
					}
				}
				if (match == 0)
					send(sock, "&", 1, 0);
				memset(word, 0, 1024);
			} else
				strcat(word, buffer);
		}
	}

	return 0;
}

