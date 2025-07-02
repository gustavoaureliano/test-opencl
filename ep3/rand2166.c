/*
 * $ rand2166 20 2166
 * 36403962
 * 1956033586
 * [...]
 */

#include <stdio.h>
#include <stdlib.h>

#define rand2166() (rand2166f(-1))
#define srand2166(seed) (rand2166f((seed)))

int rand2166f(int seed);

int main(int argc, char *argv[])
{
  int N = atoi(argv[1]);
  int seed = atoi(argv[2]);

  srand2166(seed);

  while (N-- > 0)
    printf("%d\n", rand2166());
  
  return 0;
}

#define AA 16807
#define MM 2147483647

int rand2166f(int seed) {
  static long r;
  if (seed >= 0) {
    r = seed;
    return 0;
  }
  r = (r * AA) % MM;
  return r;
}
