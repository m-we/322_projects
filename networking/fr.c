// Reads words in from a file and sends to STDOUT.

#include <stdio.h>

int main(int argc, char *argv[]) {
	FILE *fp = fopen(argv[1], "r");

	char word[1024];
	while (fscanf(fp, "%1023[0-z]%*[^0-z]", word) == 1)
		printf("%s\n", word);
	fclose(fp);

	return 0;
}
