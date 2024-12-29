inline void atomic_add_f(volatile __global float* addr, const float val) {
	float old = val; while((old=atomic_xchg(addr, atomic_xchg(addr, 0.0f)+old))!=0.0f);
}

float calcSum(float eps, int num, int div, int addDiv) {
	float sum = 0;
	float parcela = 1;
	while (parcela > eps) {
		parcela = (float)num/div;
		sum += parcela;
		div += addDiv;
		parcela *= num;
	}
	return sum;
}

__kernel 
void myKernel(__global unsigned long* addSum, __global int *nums, __global int *divs, __global float *eps, __global float *pi) {
	unsigned long tid = get_global_id(0);
	float sum = 0;
	sum = calcSum(*eps, nums[tid], divs[tid], *addSum);
	//*pi = divs[3];
	//atomic_add(pi, 5);
	atomic_add_f(pi, sum);
}
