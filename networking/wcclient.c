// Sends words from STDIN to server, prints ouput. Finish on EOF.

#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

int main(int arc, char *argv[]) {
	struct sockaddr_in addr;
	int sock = 0;
	char buffer[1024] = {0}, w[1024] = {0};

	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		printf("wcclient: Socket err.\n");
		exit(-1);
	}

	addr.sin_family = AF_INET;
	addr.sin_port = htons(atoi(argv[2]));

	if (inet_pton(AF_INET, argv[1], &addr.sin_addr) <= 0) {
		printf("wcclient: Addr err.\n");
		exit(-1);
	}

	if (connect(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
		printf("wcclient: Connection err.\n");
		exit(-1);
	}

	while (scanf("%s", buffer) != EOF) {
		strcat(buffer, " ");
		send(sock, buffer, strlen(buffer), 0);
		memset(buffer, 0, 1024);
		read(sock, buffer, 1024);
		printf("%s\n", buffer);
	}

	send(sock, "^", 1, 0);
	memset(buffer, 0, 1024);
	read(sock, buffer, 1024);
	printf("%s\n", buffer);
	exit(0);
	close(sock);

	return 0;
}
