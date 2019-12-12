#include <stdio.h>

int main() {
	char word[30];
	int count = 0;

	// Get words until no more are sent.
	while (fgets(word, sizeof word, stdin) != 0) {
		count = count + 1;
		printf("%d, %s", count, word);
	}
	printf("WordCount=%d\n", count);

	return 0;
}
