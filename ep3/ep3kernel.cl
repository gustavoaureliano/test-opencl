#define rand2166() (rand2166f(-1, &r))
#define srand2166(seed) (rand2166f((seed), &r))

#define AA 16807
#define MM 2147483647

int rand2166f(int seed, long *r);

__kernel void myKernel(__global uchar *pixels, __global int *B) {
	int i = get_global_id(0);
	long r;
	srand2166(2166);
	for (int j = 0; j < 10; j++) {
		printf("pixel[%d]: %d - rand: %d", i, pixels[i], rand2166());
	}
}

int rand2166f(int seed, long *r) {
  if (seed >= 0) {
    *r = seed;
    return 0;
  }
  *r = (*r * AA) % MM;
  return *r;
}
