#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <semaphore.h>

#define THREAD_NUM 24 // Definindo a quantidade de Threads que serão criadas

sem_t controlador; // Declarando o semáforo que servirá como o meio de controle de concorrência das threads

void* tarefa(void* arg) {
    printf("[Tarefa %d] Aguardando execução.\n", *(int*)arg);
    sem_wait(&controlador);
    printf("[Tarefa %d] Execução iniciada.\n", *(int*)arg);
    sleep(5);
    printf("[Tarefa %d] Finalizada.\n", *(int*)arg);
    sem_post(&controlador);
    free(arg);
}

int main() {
    pthread_t threads[THREAD_NUM];
    sem_init(&controlador, 0, 4); // Inicializando o semáforo e definindo seu contador para 4
    int i;
    for (i = 0; i < THREAD_NUM; i++) { // Loop para criar as threads
        int* a = malloc(sizeof(int));
        *a = i; // "a" será passado como argumento para cada thread e corresponde ao número daquela thread
        if (pthread_create(&threads[i], NULL, &tarefa, a) != 0) {
            perror("Não foi possível criar a thread");
        }
    }
    for (i = 0; i < THREAD_NUM; i++) { // Loop para garantir o join das threads
        if (pthread_join(threads[i], NULL) != 0) {
            perror("Não foi possível dar join thread");
        }
    }
    sem_destroy(&controlador);
}