__kernel 
void myKernel(__global int *A, __global int *B) {
	int i = get_global_id(0);
	B[i] = A[i] + 2;
}
