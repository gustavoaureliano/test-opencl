#include <stdint.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define rand2166() (rand2166f(-1))
#define srand2166(seed) (rand2166f((seed)))

#define AA 16807
#define MM 2147483647

int rand2166f(int seed);

void printpgm(uint8_t* pixels, int cols, int rows, uint8_t max_value);

int main(int argc, char* argv[]) {
	if (argc <= 1) {
		fprintf(stderr, "Too few arguments!\n");
		return 1;
	}
	char* filepath = argv[1];
	FILE* fp = fopen(filepath, "r");

	if (fp == NULL) {
		fprintf(stderr, "Failed to open '%s': %s\n" , filepath, strerror(errno));
		return 1;
	}

	// fseek(fp, 0L, SEEK_END);
	// long filesize = ftell(fp);
	// fseek(fp, 0L, SEEK_SET);

	// char* filedata = (char*)malloc(sizeof(char)*filesize);
	
	// printf("filesize: %ld\n", filesize);

	char fileheader[3];
	fgets(fileheader, sizeof(fileheader), fp);

	if (strncmp(fileheader, "P2", 2) != 0) {
		fprintf(stderr, "Wrong file type! It should be a pgm image");
	}

	int cols, rows = 0;
	uint8_t max_value = 0;
	if (fscanf(fp, "%d %d %hhu", &cols, &rows, &max_value) < 3) {
		fprintf(stderr, "Corrupted file!");
	}
	// printf("cols: %d, rows: %d\n", cols, rows);
	// printf("max value: %d\n", max_value);

	int num_pixels = cols * rows;
	uint8_t* pixels = (uint8_t*) malloc(sizeof(uint8_t) * num_pixels);

	for (int i = 0; i < num_pixels; i++) {
		uint8_t num;
		if (fscanf(fp, "%hhu", &num) < 1) {
			fprintf(stderr, "Something went wrong while reading the file!");
			return 1;
		}
		pixels[i] = num;
	}
	
	printpgm(pixels, cols, rows, max_value);
	return 0;
}

void printpgm(uint8_t* pixels, int cols, int rows, uint8_t max_value) {
	printf("P2\n");
	printf("%d %d\n", cols, rows);
	int num_pixels = cols * rows;
	printf("%d", max_value);
	for (int i = 0; i < num_pixels; i++) {
		if (i % cols == 0) {
			printf("\n");
		} else {
			printf(" ");
		}
		printf("%03d", pixels[i]);
	}
	printf("\n");
}


int rand2166f(int seed) {
  static long r;
  if (seed >= 0) {
    r = seed;
    return 0;
  }
  r = (r * AA) % MM;
  return r;
}
