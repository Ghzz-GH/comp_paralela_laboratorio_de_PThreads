/* monte_carlo_pi.c */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

long long total_pontos;
int num_threads;
long long total_circulo = 0;
pthread_mutex_t mutex_pi;

void* worker_monte_carlo(void* arg) {
    long rank = (long) arg;
    long long pontos_por_thread = total_pontos / num_threads;
    long long pontos_no_circulo_local = 0;
    
    // Semente única por thread para garantir thread-safety e evitar o lock global do rand()
    unsigned int seed = (unsigned int)(time(NULL) + rank + 1);

    for (long long i = 0; i < pontos_por_thread; i++) {
        double x = (double)rand_r(&seed) / RAND_MAX;
        double y = (double)rand_r(&seed) / RAND_MAX;

        if (x * x + y * y <= 1.0) {
            pontos_no_circulo_local++;
        }
    }

    pthread_mutex_lock(&mutex_pi);
    total_circulo += pontos_no_circulo_local;
    pthread_mutex_unlock(&mutex_pi);

    return NULL;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Uso: %s <N> <num_threads>\n", argv[0]);
        return 1;
    }

    total_pontos = atoll(argv[1]);
    num_threads = atoi(argv[2]);

    pthread_t* thread_handles = malloc(num_threads * sizeof(pthread_t));
    pthread_mutex_init(&mutex_pi, NULL);

    struct timespec inicio, fim;
    clock_gettime(CLOCK_MONOTONIC, &inicio);

    for (long thread = 0; thread < num_threads; thread++) {
        pthread_create(&thread_handles[thread], NULL, worker_monte_carlo, (void*)thread);
    }

    for (long thread = 0; thread < num_threads; thread++) {
        pthread_join(thread_handles[thread], NULL);
    }

    clock_gettime(CLOCK_MONOTONIC, &fim);
    double tempo = (fim.tv_sec - inicio.tv_sec) + (fim.tv_nsec - inicio.tv_nsec) / 1e9;

    double pi_estimado = 4.0 * (double)total_circulo / (double)total_pontos;

    printf("[Monte Carlo Pi] N=%lld | Threads=%d | Pi: %.6f | Tempo: %.4f s\n", 
           total_pontos, num_threads, pi_estimado, tempo);

    pthread_mutex_destroy(&mutex_pi);
    free(thread_handles);

    return 0;
}