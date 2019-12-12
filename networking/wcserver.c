// Takes words from client and outputs word count.

#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
	struct sockaddr_in addr;
	int fd, sock, addrlen = sizeof(addr);
	char buffer[1024] = {0};

	if ((fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
		printf("wcserver: Socket err.\n");
		exit(-1);
	}

	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(atoi(argv[1]));

	if (bind(fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
		printf("wcserver: Bind err.\n");
		exit(-1);
	}

	if (listen(fd, 3) < 0) {
		printf("wcserver: Listen err.\n");
		exit(-1);
	}

	char word[1024] = {0}, w2[1024] = {0}, intw[1024] = {0};

	// Loop to accept multiple clients.
	while (1) {
		int count = 0;
		if ((sock = accept(fd, (struct sockaddr *)&addr, (socklen_t*)&addrlen)) < 0) {
			printf("wcserver: Accept err.\n");
			exit(-1);
		}

		// Clear buffers to avoid sending output from previous client.
		memset(buffer, 0, 1024);
		memset(word, 0, 1024);

		while (1) {
			read(sock, buffer, 1);
			// On ^, send final WordCount= & close socket.
			if (buffer[0] == '^') {
				memset(word, 0, 1024);
				strcpy(word, "WordCount=");
				sprintf(intw, "%d", count);
				strcat(word, intw);
				send(sock, word, sizeof(word), 0);
				close(sock);
				break;
			// At end of word, send count, word.
			} else if (buffer[0] == ' ') {
				memset(w2, 0, 1024);
				count += 1;
				sprintf(intw, "%d", count);
				strcpy(w2, intw);
				strcat(w2, ", ");
				strcat(w2, word);
				send(sock, w2, sizeof(w2), 0);
				memset(word, 0, 1024);
			// Otherwise append character to current word.
			} else
				strcat(word, buffer);
		}
	}

	return 0;
}
