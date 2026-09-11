#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <semaphore.h>

#define THREAD_NUM 24 // Definindo a quantidade de Threads que serão criadas

sem_t controlador; // Declarando o semáforo que servirá como o meio de controle de concorrência das threads

int tarefaConcluida = 0;
int tarefasExec = 0;
int tarefasEspera = THREAD_NUM;

sem_t updtConclusao; // Semáforo para controlar a atualização das tarefas concluídas

sem_t updtExec; // Semáforo para controlar a atualização das tarefas em execução em um dado momento

void* auxiliar(void* arg) { // Função para informar status das tarefas
    while(tarefaConcluida < THREAD_NUM) {
        sleep(10);
        printf("\nTarefas Concluídas: %d \n"
                "Tarefas em Execução: %d \n"
                "Tarefas em Espera: %d\n\n", tarefaConcluida, tarefasExec, (tarefasEspera-tarefaConcluida-tarefasExec));
    }
}

void* tarefa(void* arg) {
    printf("[Tarefa %d] Aguardando execução.\n", *(int*)arg);
    sem_wait(&controlador); // A tarefa tenta iniciar a execução. Se os 4 espaços de execução não estiverem preenchidos, o semáforo aprova.

    sem_wait(&updtExec);
    tarefasExec++; // Controla quantas tarefas estão em execução no momento
    sem_post(&updtExec);
    printf("[Tarefa %d] Execução iniciada.\n", *(int*)arg);
    sleep(7);

    printf("[Tarefa %d] Finalizada.\n", *(int*)arg);
    sem_wait(&updtConclusao);
    tarefaConcluida++; // Controla quantas tarefas foram concluídas
    sem_post(&updtConclusao);

    sem_wait(&updtExec);
    tarefasExec--; // Antes de terminar a excução, garante a corretude do valor
    sem_post(&updtExec);
    sem_post(&controlador); // Após concluir a execução, a tarefa informa a finalização e o semáforo é incrementado, liberando seu espaço de execução
    free(arg);
}

int main() {
    pthread_t threads[THREAD_NUM];
    pthread_t threadAux;

    sem_init(&controlador, 0, 4); // Inicializando o semáforo e definindo seu contador para 4

    sem_init(&updtConclusao, 0, 1);
    sem_init(&updtExec, 0, 1);

    int i;
    for (i = 0; i < THREAD_NUM; i++) { // Loop para criar as threads
        int* a = malloc(sizeof(int));
        *a = i; // "a" será passado como argumento para cada thread e corresponde ao número daquela thread
        if (pthread_create(&threads[i], NULL, &tarefa, a) != 0) {
            perror("Não foi possível criar a thread");
        }
    }

    if (pthread_create(&threadAux, NULL, &auxiliar, NULL) != 0) {
        perror("Não foi possível criar a thread");
    }; // Thread para printar status

    for (i = 0; i < THREAD_NUM; i++) { // Loop para garantir o join das threads
        if (pthread_join(threads[i], NULL) != 0) {
            perror("Não foi possível dar join thread");
        }
    }

    if (pthread_join(threadAux, NULL) != 0) {
        perror("Não foi possível dar join thread");
    };

    sem_destroy(&controlador);
    sem_destroy(&updtConclusao);
    sem_destroy(&updtExec);
}