#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
	double eps, soma = 0, parcela = 1, pi, num = -1.0;
	long int div = 1;
	eps = atof(argv[1]);
	/*scanf("%lf", &eps);*/
	printf("%.16f\n", eps);
	while (parcela > eps) {
		num *= -1.0;
		/*printf("par: %f / %d = %f ", num, div, (-1.0 * num) / div);*/
		parcela = num / div;
		soma += parcela;
		div += 2;
		if (parcela < 0) {
			parcela *= -1;
		}
		/*printf("parcela: %.16f\n", parcela);*/
	}
	pi = soma * 4;
	printf("%.16f\n", pi);
	return 0;
}
