__kernel 
void vector_add(__global const int *A, __global const int *B, __global int *C) {

 
    // Get the index of the current element to be processed
    int i = get_global_id(0);
 
    // Do the operation
    float sum = A[i] + B[i];
	atomic_add(C, sum);
	//*C += sum;
}
