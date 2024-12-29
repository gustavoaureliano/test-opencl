#include <stdio.h>
#include <stdlib.h>

float calcPi(float eps) {
	float sum = 0;
	float num = -1;
	long int div = 1;
	float parcela = 1;
	while (parcela > eps) {
		num *= -1.0;
		parcela = num/div;
		sum += parcela;
		div += 2;
		if (parcela < 0) {
			parcela *= -1;
		}
	}
	return 4 * sum;
}

float calcSum(float eps, int num, int div, int addDiv) {
	float sum = 0;
	float parcela = 1;
	//printf("*** ");
	while (parcela > eps) {
		//num *= -1.0;
		//printf("(%.0f) ", (div-1)/2);
		//printf("par: %f / %f = %f ", 1.0*num, div, 1.0 / div);
		parcela = (float)num/div;
		sum += parcela;
		div += addDiv;
		//printf("parcela: %.16f\n", parcela);
		parcela *= num;
	}
	//printf("sum: %f\n", sum);
	return sum;
}

void atomicAdd(float* num1, float* num2) {
	*num1 = *num1 + *num2;
}

void kernel(int globalId, int addSum, int num, int div, float eps, float* pi) {
	//printf("kernel\n");
	float sum = 0;
	float parcela = 1;
	sum = calcSum(eps, num, div, addSum);
	//printf("(pi) %.6f + (sum) %.6f = ", *pi, sum);
	atomicAdd(pi, &sum);
	//printf("%.6f\n", *pi);
}

int main(int argc, char* argv[]) {
	printf("argc: %d\n", argc);
	if (argc <= 1) {
		printf("1 argument nedeed\n");
		return 0;
	}
	float eps = atof(argv[1]);
	printf("eps: %.16f\n", eps);
	//float pi = calcPi(eps);

	float pi = 0;
	float div = 1;
	int num = 1;
	int total = 1000000;
	int divs[total];
	int nums[total];

	for (int i = 0; i < total; i++) {
		divs[i] = div;
		nums[i] = num;
		div += 2;
		num *= -1;
	}

	for (int i = 0; i < total; i++) {
		kernel(i, total*2, nums[i], divs[i], eps, &pi);
	}

	printf("π: %.16f\n", 4*pi);
}
