__kernel void myKernel(__global uchar *pixels, __global int *B) {
	int i = get_global_id(0);
	printf("hello");
	printf("i: %d", i);
	printf("pixel[%d]: %d", i, pixels[i]);
}
