#include <stdio.h>

int main(int argc, char *argv[]) {
	FILE *fp = fopen(argv[1], "r");

	char word[30];
	while (fscanf(fp, "%29[0-z]%*[^0-z]", word) == 1)
		printf("%s\n", word);
	fclose(fp);

	return 0;
}
