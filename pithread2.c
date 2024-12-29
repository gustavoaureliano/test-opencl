#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

double soma, eps;
int nthreads;
pthread_mutex_t mutex;

void soma1(void* arg) {
	double parcela = 1, somaL = 0;
	long int div = *((int*)arg);
	while (parcela > eps) {
		/*printf("par: %f / %d = %f ", 1.0, div, 1.0 / div); */
		parcela = 1.0 / div;
		somaL += parcela;
		div += nthreads * 2;
		/*printf("parcela: %.16f\n", parcela);*/
	}
	pthread_mutex_lock(&mutex);
	soma += somaL;
	pthread_mutex_unlock(&mutex);
}

void soma2(void* arg) {
	double parcela = 1, somaL = 0;
	long int div = *((int*)arg);
	while (parcela > eps) {
		/*printf("par: %f / %d = %f ", -1.0, div, -1.0 / div);*/
		parcela = -1.0 / div;
		somaL += parcela;
		div += nthreads * 2;
		parcela *= -1.0;
		/*printf("parcela: %.16f\n", parcela);*/
	}
	pthread_mutex_lock(&mutex);
	soma += somaL;
	pthread_mutex_unlock(&mutex);
}

int main(int argc, char* argv[]) {
	double pi;
	int i;
	int div = 1;
	nthreads = atoi(argv[2]);
	int divs[nthreads]; 
	pthread_t threads[nthreads];
	soma = 0;
	eps = atof(argv[1]);
	printf("%.16f\n", eps);
	/*
	pthread_create(&a, NULL, (void*)&soma1, NULL);
	pthread_create(&b, NULL, (void*)&soma2, NULL);
	pthread_join(a, NULL);
	pthread_join(b, NULL);
	*/
	for(i = 0; i < nthreads; i++) {
		divs[i] = div;
		div += 2;
	}
	for(i = 0; i < nthreads; i++) {
		pthread_t thread;
		threads[i] = thread;
		if (i % 2 == 0) {
			pthread_create(&threads[i], NULL, (void*)&soma1, (void*)&divs[i]);
		} else {
			pthread_create(&threads[i], NULL, (void*)&soma2, (void*)&divs[i]);
		}
	}
	for(i = 0; i < nthreads; i++) {
		pthread_join(threads[i], NULL);
	}
	pi = soma * 4;
	printf("%.16f\n", pi);
	return 0;
}
