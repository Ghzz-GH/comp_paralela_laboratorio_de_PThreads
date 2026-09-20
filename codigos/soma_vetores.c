/* soma_vetores.c */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

long long N;
int num_threads;
double *A, *B, *C;

void* worker_soma(void* arg) {
    long rank = (long)arg;
    long long elementos_por_thread = N / num_threads;
    long long inicio = rank * elementos_por_thread;
    long long fim = (rank == num_threads - 1) ? N : inicio + elementos_por_thread;

    for (long long i = inicio; i < fim; i++) {
        C[i] = A[i] + B[i];
    }

    return NULL;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Uso: %s <N> <num_threads>\n", argv[0]);
        return 1;
    }

    N = atoll(argv[1]);
    num_threads = atoi(argv[2]);

    A = (double*)malloc(N * sizeof(double));
    B = (double*)malloc(N * sizeof(double));
    C = (double*)malloc(N * sizeof(double));

    for (long long i = 0; i < N; i++) {
        A[i] = 1.05;
        B[i] = 2.05;
    }

    pthread_t* thread_handles = malloc(num_threads * sizeof(pthread_t));

    struct timespec inicio, fim;
    clock_gettime(CLOCK_MONOTONIC, &inicio);

    for (long thread = 0; thread < num_threads; thread++) {
        pthread_create(&thread_handles[thread], NULL, worker_soma, (void*)thread);
    }

    for (long thread = 0; thread < num_threads; thread++) {
        pthread_join(thread_handles[thread], NULL);
    }

    clock_gettime(CLOCK_MONOTONIC, &fim);
    double tempo = (fim.tv_sec - inicio.tv_sec) + (fim.tv_nsec - inicio.tv_nsec) / 1e9;

    printf("[Soma Vetores] N=%lld | Threads=%d | Tempo: %.4f s\n", N, num_threads, tempo);

    free(A);
    free(B);
    free(C);
    free(thread_handles);

    return 0;
}