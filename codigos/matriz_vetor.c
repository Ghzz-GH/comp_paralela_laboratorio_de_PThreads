/* matriz_vetor.c */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

long N;
int num_threads;
double **M;
double *V;
double *R;

void* worker_matriz_vetor(void* arg) {
    long rank = (long)arg;
    long linhas_por_thread = N / num_threads;
    long inicio = rank * linhas_por_thread;
    long fim = (rank == num_threads - 1) ? N : inicio + linhas_por_thread;

    for (long i = inicio; i < fim; i++) {
        double soma = 0.0;
        for (long j = 0; j < N; j++) {
            soma += M[i][j] * V[j];
        }
        R[i] = soma;
    }

    return NULL;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Uso: %s <N> <num_threads>\n", argv[0]);
        return 1;
    }

    N = atol(argv[1]);
    num_threads = atoi(argv[2]);

    // Alocação dinâmica da matriz M, vetor V e vetor resultante R
    M = (double**)malloc(N * sizeof(double*));
    for (long i = 0; i < N; i++) {
        M[i] = (double*)malloc(N * sizeof(double));
    }
    V = (double*)malloc(N * sizeof(double));
    R = (double*)malloc(N * sizeof(double));

    // Inicialização da matriz e do vetor
    for (long i = 0; i < N; i++) {
        V[i] = 1.0;
        for (long j = 0; j < N; j++) {
            M[i][j] = 1.0;
        }
    }

    pthread_t* thread_handles = malloc(num_threads * sizeof(pthread_t));

    struct timespec inicio, fim;
    clock_gettime(CLOCK_MONOTONIC, &inicio);

    for (long thread = 0; thread < num_threads; thread++) {
        pthread_create(&thread_handles[thread], NULL, worker_matriz_vetor, (void*)thread);
    }

    for (long thread = 0; thread < num_threads; thread++) {
        pthread_join(thread_handles[thread], NULL);
    }

    clock_gettime(CLOCK_MONOTONIC, &fim);
    double tempo = (fim.tv_sec - inicio.tv_sec) + (fim.tv_nsec - inicio.tv_nsec) / 1e9;

    printf("[Matriz-Vetor] N=%ld | Threads=%d | Tempo: %.4f s\n", N, num_threads, tempo);

    // Libertação de memória
    for (long i = 0; i < N; i++) {
        free(M[i]);
    }
    free(M);
    free(V);
    free(R);
    free(thread_handles);

    return 0;
}