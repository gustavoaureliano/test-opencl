#include <stdio.h>

int main() {
	char* filepath = "charkernel.cl";
	FILE* file = fopen(filepath, "r");
	char fileSource[10000];
	size_t fileSize = fread(fileSource, sizeof(char), sizeof(fileSource), file);
	printf("%s", fileSource);
	printf("size: (%zu) - end of source!", fileSize);
	int count = 0;
	for (int i = 0; i < fileSize; i++) {
		if (fileSource[i] == 'e') {
			count++;
			printf("%c - true\n", fileSource[i]);
		}
	}
	printf("output: %d\n", count);
	return 0;
}
