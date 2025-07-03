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
uint8_t* readpgm(char* filepath, int* cols, int* rows, uint8_t* max_value);
uint8_t* encode(uint8_t* pixels, int key, int cols, int rows, uint8_t max_value);
uint8_t* decode(uint8_t* pixels, int key, int cols, int rows, uint8_t max_value);

int main(int argc, char* argv[]) {
	if (argc <= 2) {
		fprintf(stderr, "Too few arguments!\n");
		return 1;
	}
	char* filepath = argv[1];
	int option = atoi(argv[2]);
	int key = atoi(argv[3]); // i'm not really verifying  if it's a number
	int cols, rows = 0;
	uint8_t max_value = 0;
	uint8_t* pixels = readpgm(filepath, &cols, &rows, &max_value);
	uint8_t* result = NULL;
	switch (option) {
		case 0:
			result = encode(pixels, key, cols, rows, max_value);
		break;
		case 1:
			result = decode(pixels, key, cols, rows, max_value);
		break;
		case 2:
			// decode brute force
		break;
		default:
			fprintf(stderr, "Invalid option!\n");
			return 1;
		break;
	}
	if (result == NULL) {
		fprintf(stderr, "Something went wrong!\n");
		return 1;
	}
	printpgm(result, cols, rows, max_value);
	return 0;
}

uint8_t* encode(uint8_t* pixels, int key, int cols, int rows, uint8_t max_value) {
	int num_pixels = cols * rows;
	srand2166(key);
	uint8_t* encoded = (uint8_t*) malloc(sizeof(uint8_t) * num_pixels);
	for (int i = 0; i < num_pixels; i++) {
		encoded[i] = (pixels[i] + (rand2166() % (max_value + 1))) % (max_value + 1);
	}
	return encoded;
}

uint8_t* decode(uint8_t* pixels, int key, int cols, int rows, uint8_t max_value) {
	int num_pixels = cols * rows;
	srand2166(key);
	uint8_t* decoded = (uint8_t*) malloc(sizeof(uint8_t) * num_pixels);
	for (int i = 0; i < num_pixels; i++) {
		decoded[i] = (pixels[i] - (rand2166() % (max_value + 1)) + (max_value +1)) % (max_value + 1);
	}
	return decoded;
}

uint8_t* readpgm(char* filepath, int* cols, int* rows, uint8_t* max_value) {
	FILE* fp = fopen(filepath, "r");

	if (fp == NULL) {
		fprintf(stderr, "Failed to open '%s': %s\n" , filepath, strerror(errno));
		exit(1);
	}

	char fileheader[3];
	fgets(fileheader, sizeof(fileheader), fp);

	if (strncmp(fileheader, "P2", 2) != 0) {
		fprintf(stderr, "Wrong file type! It should be a pgm image\n");
		exit(1);
	}

	if (fscanf(fp, "%d %d %hhu", cols, rows, max_value) < 3) {
		fprintf(stderr, "Corrupted file!\n");
		exit(1);
	}

	int num_pixels = ( *cols ) * ( *rows );
	uint8_t* pixels = (uint8_t*) malloc(sizeof(uint8_t) * num_pixels);

	for (int i = 0; i < num_pixels; i++) {
		uint8_t num;
		if (fscanf(fp, "%hhu", &num) < 1) {
			fprintf(stderr, "Something went wrong while reading the file!\n");
			exit(1);
		}
		pixels[i] = num;
	}
	return pixels;
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
