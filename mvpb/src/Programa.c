#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

typedef struct node{
    char *nome;
    char *caminho;
    char **comando;
    struct node *next;
}node;

node *lista_tarefas = NULL;

node* criar_tarefa(char *linha){

    char *nome = strtok(NULL, " ");
    char *copnome = strdup(nome);

    char *caminho = strtok(NULL, " ");
    char *copcaminho = strdup(caminho);

    char *comando = strtok(NULL, " ");
    char *copcomando = strdup(comando);

    char **alocacomando = malloc(sizeof(char*));
    alocacomando[0] = copcaminho;
    int contador = 1;

    while (copcomando != NULL){
        alocacomando = realloc(alocacomando, sizeof(char*) * (contador+1));
        alocacomando[contador] = copcomando;
        contador++;

        comando = strtok(NULL, " ");
        if (comando != NULL) {
            copcomando = strdup(comando);

        }else{
            copcomando = NULL;
        }
    }
    alocacomando = realloc(alocacomando, sizeof(char*) * (contador+1));
    alocacomando[contador] = NULL;

    node *tarefa = (node*)malloc(sizeof(node));
    tarefa->nome = copnome;
    tarefa->caminho = copcaminho;
    tarefa->comando = alocacomando;
    tarefa->next = NULL;

    return tarefa;
}

node *procurar_tarefa(char *nome){

    node *aux = lista_tarefas;

    while(aux != NULL){
       if (strcmp(aux->nome, nome) == 0){
        return aux;
       }

       aux = aux->next;
    }
    return NULL;
}

void executar_tarefa(char *nome){
    node *tarefa = procurar_tarefa(nome);

    if (tarefa == NULL){
        printf("Tarefa não encontrada\n");
        return;
    }

    pid_t fok = fork();
    if(fok == 0){
        execvp(tarefa->caminho, tarefa->comando);

        printf("Erro no filho");
        exit(1);

    }else if (fok >= 1){
        wait(NULL);

    }else{
        printf("Não foi possível executar a tarefa\n");
    }
}

int main(){
    char linha[3000];

    do{
        printf("Processflow> ");

        fgets(linha, sizeof(linha), stdin);
        linha[strcspn(linha, "\n")] = '\0';

        char *token1 = strtok(linha, " ");
        token1[strcspn(token1, "\n")] = '\0';

        if ((strcmp (token1, "task") == 0)){
            node *tarefa = criar_tarefa(linha);
            tarefa->next = lista_tarefas;
            lista_tarefas = tarefa;

        }else if((strcmp (token1, "run") == 0)){
            char *nome = strtok(NULL, " ");
            nome[strcspn(nome, "\n")] = '\0';
            executar_tarefa(nome);

        }else if ((strcmp (token1, "exit") == 0)){
            break;

        }else{
            printf("Essa função não existe\n");
        }

    }while(1);
    
}