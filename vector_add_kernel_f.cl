
inline void atomic_add_f(volatile __global float* addr, const float val) {
	float old = val; while((old=atomic_xchg(addr, atomic_xchg(addr, 0.0f)+old))!=0.0f);
}

__kernel 
void vector_add(__global const float *A, __global const float *B, __global float *C) {

 
    // Get the index of the current element to be processed
    int i = get_global_id(0);
 
    // Do the operation
    float sum = A[i] + B[i];
	//atomicAdd_g_f(C, sum);
	//float old = sum; while((old=atomic_xchg(C, atomic_xchg(C, 0.0f)+old))!=0.0f);
	atomic_add_f(C, sum);
	//float old = sum; while((old=atomic_xchg(C, atomic_xchg(C, 0.0f)+old))!=0.0f);
	//*C += sum;
}
