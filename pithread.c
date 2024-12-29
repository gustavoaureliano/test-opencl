#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

double soma, eps;
pthread_mutex_t mutex;

void soma1(void* arg) {
	double parcela = 1, somaL = 0;
	long int div = 1;
	while (parcela > eps) {
		/*printf("par: %f / %d = %f ", 1.0, div, 1.0 / div);*/ 
		parcela = 1.0 / div;
		somaL += parcela;
		div += 32;
		/*printf("parcela: %.16f\n", parcela);*/
	}
	pthread_mutex_lock(&mutex);
	soma += somaL;
	pthread_mutex_unlock(&mutex);
}

void soma2(void* arg) {
	double parcela = 1, somaL = 0;
	long int div = 3;
	while (parcela > eps) {
		/*printf("par: %f / %d = %f ", -1.0, div, -1.0 / div);*/
		parcela = -1.0 / div;
		somaL += parcela;
		div += 32;
		parcela *= -1.0;
		/*printf("parcela: %.16f\n", parcela);*/
	}
	pthread_mutex_lock(&mutex);
	soma += somaL;
	pthread_mutex_unlock(&mutex);
}

void soma3(void* arg) {
	double parcela = 1, somaL = 0;
	long int div = 5;
	while (parcela > eps) {
		/*printf("par: %f / %d = %f ", 1.0, div, 1.0 / div);*/ 
		parcela = 1.0 / div;
		somaL += parcela;
		div += 32;
		/*printf("parcela: %.16f\n", parcela);*/
	}
	pthread_mutex_lock(&mutex);
	soma += somaL;
	pthread_mutex_unlock(&mutex);
}

void soma4(void* arg) {
	double parcela = 1, somaL = 0;
	long int div = 7;
	while (parcela > eps) {
		/*printf("par: %f / %d = %f ", -1.0, div, -1.0 / div);*/
		parcela = -1.0 / div;
		somaL += parcela;
		div += 32;
		parcela *= -1.0;
		/*printf("parcela: %.16f\n", parcela);*/
	}
	pthread_mutex_lock(&mutex);
	soma += somaL;
	pthread_mutex_unlock(&mutex);
}

void soma5(void* arg) {
	double parcela = 1, somaL = 0;
	long int div = 9;
	while (parcela > eps) {
		/*printf("par: %f / %d = %f ", 1.0, div, 1.0 / div);*/ 
		parcela = 1.0 / div;
		somaL += parcela;
		div += 32;
		/*printf("parcela: %.16f\n", parcela);*/
	}
	pthread_mutex_lock(&mutex);
	soma += somaL;
	pthread_mutex_unlock(&mutex);
}

void soma6(void* arg) {
	double parcela = 1, somaL = 0;
	long int div = 11;
	while (parcela > eps) {
		/*printf("par: %f / %d = %f ", -1.0, div, -1.0 / div);*/
		parcela = -1.0 / div;
		somaL += parcela;
		div += 32;
		parcela *= -1.0;
		/*printf("parcela: %.16f\n", parcela);*/
	}
	pthread_mutex_lock(&mutex);
	soma += somaL;
	pthread_mutex_unlock(&mutex);
}

void soma7(void* arg) {
	double parcela = 1, somaL = 0;
	long int div = 13;
	while (parcela > eps) {
		/*printf("par: %f / %d = %f ", 1.0, div, 1.0 / div);*/ 
		parcela = 1.0 / div;
		somaL += parcela;
		div += 32;
		/*printf("parcela: %.16f\n", parcela);*/
	}
	pthread_mutex_lock(&mutex);
	soma += somaL;
	pthread_mutex_unlock(&mutex);
}

void soma8(void* arg) {
	double parcela = 1, somaL = 0;
	long int div = 15;
	while (parcela > eps) {
		/*printf("par: %f / %d = %f ", -1.0, div, -1.0 / div);*/
		parcela = -1.0 / div;
		somaL += parcela;
		div += 32;
		parcela *= -1.0;
		/*printf("parcela: %.16f\n", parcela);*/
	}
	pthread_mutex_lock(&mutex);
	soma += somaL;
	pthread_mutex_unlock(&mutex);
}

void soma9(void* arg) {
	double parcela = 1, somaL = 0;
	long int div = 17;
	while (parcela > eps) {
		/*printf("par: %f / %d = %f ", 1.0, div, 1.0 / div);*/ 
		parcela = 1.0 / div;
		somaL += parcela;
		div += 32;
		/*printf("parcela: %.16f\n", parcela);*/
	}
	pthread_mutex_lock(&mutex);
	soma += somaL;
	pthread_mutex_unlock(&mutex);
}

void soma10(void* arg) {
	double parcela = 1, somaL = 0;
	long int div = 19;
	while (parcela > eps) {
		/*printf("par: %f / %d = %f ", -1.0, div, -1.0 / div);*/
		parcela = -1.0 / div;
		somaL += parcela;
		div += 32;
		parcela *= -1.0;
		/*printf("parcela: %.16f\n", parcela);*/
	}
	pthread_mutex_lock(&mutex);
	soma += somaL;
	pthread_mutex_unlock(&mutex);
}

void soma11(void* arg) {
	double parcela = 1, somaL = 0;
	long int div = 21;
	while (parcela > eps) {
		/*printf("par: %f / %d = %f ", 1.0, div, 1.0 / div);*/ 
		parcela = 1.0 / div;
		somaL += parcela;
		div += 32;
		/*printf("parcela: %.16f\n", parcela);*/
	}
	pthread_mutex_lock(&mutex);
	soma += somaL;
	pthread_mutex_unlock(&mutex);
}

void soma12(void* arg) {
	double parcela = 1, somaL = 0;
	long int div = 23;
	while (parcela > eps) {
		/*printf("par: %f / %d = %f ", -1.0, div, -1.0 / div);*/
		parcela = -1.0 / div;
		somaL += parcela;
		div += 32;
		parcela *= -1.0;
		/*printf("parcela: %.16f\n", parcela);*/
	}
	pthread_mutex_lock(&mutex);
	soma += somaL;
	pthread_mutex_unlock(&mutex);
}

void soma13(void* arg) {
	double parcela = 1, somaL = 0;
	long int div = 25;
	while (parcela > eps) {
		/*printf("par: %f / %d = %f ", 1.0, div, 1.0 / div);*/ 
		parcela = 1.0 / div;
		somaL += parcela;
		div += 32;
		/*printf("parcela: %.16f\n", parcela);*/
	}
	pthread_mutex_lock(&mutex);
	soma += somaL;
	pthread_mutex_unlock(&mutex);
}

void soma14(void* arg) {
	double parcela = 1, somaL = 0;
	long int div = 27;
	while (parcela > eps) {
		/*printf("par: %f / %d = %f ", -1.0, div, -1.0 / div);*/
		parcela = -1.0 / div;
		somaL += parcela;
		div += 32;
		parcela *= -1.0;
		/*printf("parcela: %.16f\n", parcela);*/
	}
	pthread_mutex_lock(&mutex);
	soma += somaL;
	pthread_mutex_unlock(&mutex);
}

void soma15(void* arg) {
	double parcela = 1, somaL = 0;
	long int div = 29;
	while (parcela > eps) {
		/*printf("par: %f / %d = %f ", 1.0, div, 1.0 / div);*/ 
		parcela = 1.0 / div;
		somaL += parcela;
		div += 32;
		/*printf("parcela: %.16f\n", parcela);*/
	}
	pthread_mutex_lock(&mutex);
	soma += somaL;
	pthread_mutex_unlock(&mutex);
}

void soma16(void* arg) {
	double parcela = 1, somaL = 0;
	long int div = 31;
	while (parcela > eps) {
		/*printf("par: %f / %d = %f ", -1.0, div, -1.0 / div);*/
		parcela = -1.0 / div;
		somaL += parcela;
		div += 32;
		parcela *= -1.0;
		/*printf("parcela: %.16f\n", parcela);*/
	}
	pthread_mutex_lock(&mutex);
	soma += somaL;
	pthread_mutex_unlock(&mutex);
}

int main(int argc, char* argv[]) {
	pthread_t a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p;
	double pi;
	soma = 0;
	eps = atof(argv[1]);
	printf("%.16f\n", eps);
	pthread_create(&a, NULL, (void*)&soma1, NULL);
	pthread_create(&b, NULL, (void*)&soma2, NULL);
	pthread_create(&c, NULL, (void*)&soma3, NULL);
	pthread_create(&d, NULL, (void*)&soma4, NULL);
	pthread_create(&e, NULL, (void*)&soma5, NULL);
	pthread_create(&f, NULL, (void*)&soma6, NULL);
	pthread_create(&g, NULL, (void*)&soma7, NULL);
	pthread_create(&h, NULL, (void*)&soma8, NULL);
	pthread_create(&i, NULL, (void*)&soma9, NULL);
	pthread_create(&j, NULL, (void*)&soma10, NULL);
	pthread_create(&k, NULL, (void*)&soma11, NULL);
	pthread_create(&l, NULL, (void*)&soma12, NULL);
	pthread_create(&m, NULL, (void*)&soma13, NULL);
	pthread_create(&n, NULL, (void*)&soma14, NULL);
	pthread_create(&o, NULL, (void*)&soma15, NULL);
	pthread_create(&p, NULL, (void*)&soma16, NULL);
	pthread_join(a, NULL);
	pthread_join(b, NULL);
	pthread_join(c, NULL);
	pthread_join(d, NULL);
	pthread_join(e, NULL);
	pthread_join(f, NULL);
	pthread_join(g, NULL);
	pthread_join(h, NULL);
	pthread_join(i, NULL);
	pthread_join(j, NULL);
	pthread_join(k, NULL);
	pthread_join(l, NULL);
	pthread_join(m, NULL);
	pthread_join(n, NULL);
	pthread_join(o, NULL);
	pthread_join(p, NULL);
	pi = soma * 4;
	printf("%.16f\n", pi);
	return 0;
}
