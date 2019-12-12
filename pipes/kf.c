#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
	FILE *fp = fopen(argv[1], "r");

	char word[30];
	char keys[10][30];

	int i = 0;
	while (fscanf(fp, "%29[0-z]%*[^0-z]", keys[i]) == 1 && i < 10)
		i = i + 1;
	fclose(fp);

	while (scanf("%s", word) == 1)
		for (int i = 0; i < 10; i = i + 1)
			if (strcasecmp(word, keys[i]) == 0)
				printf("%s\n", word);

	return 0;
}
