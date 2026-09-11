# Relatório - Atividade de Programação Concorrente

## 1. Informações gerais

O problema escolhido foi o de número 3: "Gerenciador de Tarefas". A linguagem escolhida foi C.

## 2. Instruções para compilação e execução

Para compilar o programa, é necessário baixar ou clonar os arquivos e utilizar um compilador C como o GCC e incluir a flag de ligação da biblioteca de threads (pthread). 

Comando para compilação (Não é estritamente necessário compilar, basta executar o arquivo "program"):
gcc gerenciador-tarefas.c -o program -pthread

Comando para execução:
./program

## 3. Explicação da estratégia de sincronização

A estratégia de sincronização utiliza semáforos para duas finalidades distintas: controle de concorrência e exclusão mútua. 
Para o controle de concorrência, foi definido um limite máximo de tarefas que podem executar sua carga de trabalho simultaneamente, nesse caso, 4. Isso evita a sobrecarga do sistema e impede que mais de 4 threads executem ao mesmo tempo. 
Para a exclusão mútua, a estratégia garante que as variáveis globais, que armazenam o estado do sistema (quantas tarefas estão rodando, quantas terminaram e quantas estão em espera), não sofram condições de corrida quando múltiplas tarefas tentam atualizá-las ao mesmo tempo.

## 4. Descrição dos semáforos utilizados

O programa faz uso de três semáforos:
- controlador: semáforo contador responsável por restringir o número de tarefas (threads) que podem entrar na região de processamento principal ao mesmo tempo.
- updtExec: semáforo binário cuja função é garantir exclusão mútua durante a leitura e escrita da variável global tarefasExec.
- updtConclusao: semáforo binário que garante exclusão mútua durante a leitura e escrita da variável global tarefaConcluida.

## 5. Valor inicial de cada semáforo

- controlador: inicializado com o valor 4, permitindo que até 4 tarefas obtenham acesso simultâneo.
- updtExec: inicializado com o valor 1, liberando o acesso inicial à região crítica da variável.
- updtConclusao: inicializado com o valor 1, liberando o acesso inicial à região crítica da variável.

## 6. Explicação das operações wait() e signal()

No código fonte em C, as funções da biblioteca semaphore.h representam essas operações da seguinte maneira:
- sem_wait(): representa a operação wait(). Ela verifica o valor do semáforo. Se o valor for maior que 0, ele é decrementado e a thread continua executando. Se o valor for igual a 0, a thread é suspensa e fica aguardando até que o semáforo seja liberado.
- sem_post(): representa a operação signal(). Ela incrementa o valor do semáforo. Se houver alguma thread bloqueada aguardando por este semáforo, essa thread é acordada e pode prosseguir com sua execução.

## 7. Identificação das regiões críticas

As regiões críticas deste programa são os trechos de código onde ocorrem manipulações de recursos compartilhados ou acesso restrito. Elas foram identificadas e isoladas da seguinte forma:
- Região crítica de concorrência múltipla: o bloco entre o sem_wait(&controlador) e o sem_post(&controlador), onde ocorre a simulação de trabalho e a impressão de início de execução da tarefa.
- Região crítica de atualização de execução: as linhas onde ocorrem o incremento e decremento da variável tarefasExec, estritamente delimitadas pelas operações no semáforo updtExec.
- Região crítica de atualização de conclusão: a linha onde ocorre o incremento da variável tarefaConcluida, estritamente delimitada pelas operações no semáforo updtConclusao.

## 8. Exemplo de execução do programa

Abaixo está um exemplo da saída gerada pelo programa ao ser executado. O sistema inicializa 24 tarefas, mas devido à limitação do semáforo controlador, apenas 4 executam simultaneamente. A thread auxiliar exibe relatórios periódicos do progresso.

```text
[Tarefa 2] Aguardando execução.
[Tarefa 2] Execução iniciada.
[Tarefa 0] Aguardando execução.
[Tarefa 0] Execução iniciada.
[Tarefa 1] Aguardando execução.
[Tarefa 1] Execução iniciada.
[Tarefa 3] Aguardando execução.
[Tarefa 3] Execução iniciada.
[Tarefa 4] Aguardando execução.
[Tarefa 5] Aguardando execução.
[Tarefa 6] Aguardando execução.
[Tarefa 7] Aguardando execução.
[Tarefa 8] Aguardando execução.
[Tarefa 9] Aguardando execução.
[Tarefa 10] Aguardando execução.
[Tarefa 11] Aguardando execução.
[Tarefa 12] Aguardando execução.
[Tarefa 13] Aguardando execução.
[Tarefa 15] Aguardando execução.
[Tarefa 14] Aguardando execução.
[Tarefa 16] Aguardando execução.
[Tarefa 18] Aguardando execução.
[Tarefa 17] Aguardando execução.
[Tarefa 19] Aguardando execução.
[Tarefa 20] Aguardando execução.
[Tarefa 21] Aguardando execução.
[Tarefa 22] Aguardando execução.
[Tarefa 23] Aguardando execução.
[Tarefa 2] Finalizada.
[Tarefa 3] Finalizada.
[Tarefa 0] Finalizada.
[Tarefa 4] Execução iniciada.
[Tarefa 1] Finalizada.
[Tarefa 6] Execução iniciada.
[Tarefa 5] Execução iniciada.
[Tarefa 7] Execução iniciada.

Tarefas Concluídas: 4 
Tarefas em Execução: 4 
Tarefas em Espera: 16

[Tarefa 7] Finalizada.
[Tarefa 5] Finalizada.
[Tarefa 6] Finalizada.
[Tarefa 9] Execução iniciada.
[Tarefa 4] Finalizada.
[Tarefa 11] Execução iniciada.
[Tarefa 10] Execução iniciada.
[Tarefa 8] Execução iniciada.

Tarefas Concluídas: 8 
Tarefas em Execução: 4 
Tarefas em Espera: 12

[Tarefa 10] Finalizada.
[Tarefa 12] Execução iniciada.
[Tarefa 11] Finalizada.
[Tarefa 13] Execução iniciada.
[Tarefa 8] Finalizada.
[Tarefa 15] Execução iniciada.
[Tarefa 9] Finalizada.
[Tarefa 14] Execução iniciada.
[Tarefa 14] Finalizada.
[Tarefa 15] Finalizada.
[Tarefa 13] Finalizada.
[Tarefa 18] Execução iniciada.
[Tarefa 12] Finalizada.
[Tarefa 16] Execução iniciada.
[Tarefa 17] Execução iniciada.
[Tarefa 19] Execução iniciada.

Tarefas Concluídas: 16 
Tarefas em Execução: 4 
Tarefas em Espera: 4

[Tarefa 16] Finalizada.
[Tarefa 18] Finalizada.
[Tarefa 21] Execução iniciada.
[Tarefa 19] Finalizada.
[Tarefa 22] Execução iniciada.
[Tarefa 17] Finalizada.
[Tarefa 23] Execução iniciada.
[Tarefa 20] Execução iniciada.

Tarefas Concluídas: 20 
Tarefas em Execução: 4 
Tarefas em Espera: 0

[Tarefa 20] Finalizada.
[Tarefa 23] Finalizada.
[Tarefa 21] Finalizada.
[Tarefa 22] Finalizada.

Tarefas Concluídas: 24 
Tarefas em Execução: 0 
Tarefas em Espera: 0


```