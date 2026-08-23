#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

typedef struct node{
    char *nome;
    char *caminho;
    char **comando;
    struct node *next;

    char *arquivosai;
    char *arquivoentra;
    int appendar;
}node;

node *lista_tarefas = NULL;

typedef struct job{
    int id;
    pid_t pid;
    struct job *next;
}job;

job *lista_de_jobs = NULL;
int proxID = 1;





node* criar_tarefa(char *linha){

    char *nome = strtok(NULL, " ");
    char *copnome = strdup(nome);

    char *caminho = strtok(NULL, " ");
    char *copcaminho = strdup(caminho);

    char *comando = strtok(NULL, " ");
    char *copcomando;

    if(comando != NULL){
        copcomando = strdup(comando);

    }else{
        copcomando = NULL;

    }

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

    tarefa->arquivosai = NULL;
    tarefa->arquivoentra = NULL;
    tarefa->appendar = 0;

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

        if(tarefa->arquivosai != NULL){
            int leitordearquivo;

            if(tarefa->appendar == 1){
                leitordearquivo = open(tarefa->arquivosai, O_WRONLY | O_CREAT | O_APPEND, 0644);

            }else{
                leitordearquivo = open(tarefa->arquivosai, O_WRONLY | O_CREAT | O_TRUNC, 0644);

            }
            dup2(leitordearquivo, 1);
            close(leitordearquivo);

        }

        int leitordearquivo;
        if(tarefa->arquivoentra != NULL){
            leitordearquivo = open(tarefa->arquivoentra, O_RDONLY);

            dup2(leitordearquivo, 0);
            close(leitordearquivo);
        }

        execvp(tarefa->caminho, tarefa->comando);

        printf("Erro no filho\n");
        exit(1);

    }else if (fok >= 1){
        wait(NULL);

    }else{
        printf("Não foi possível executar a tarefa\n");
    }
}

pid_t executar_tarefa_parallel(char *nome){
    node *tarefa = procurar_tarefa(nome);

    if (tarefa == NULL){
        printf("Tarefa não encontrada\n");
        return -1;
    }

    pid_t fok = fork();
    if(fok == 0){

        if(tarefa->arquivosai != NULL){
            int leitordearquivo;

            if(tarefa->appendar == 1){
                leitordearquivo = open(tarefa->arquivosai, O_WRONLY | O_CREAT | O_APPEND, 0644);

            }else{
                leitordearquivo = open(tarefa->arquivosai, O_WRONLY | O_CREAT | O_TRUNC, 0644);

            }
            dup2(leitordearquivo, 1);
            close(leitordearquivo);

        }

        int leitordearquivo;
        if(tarefa->arquivoentra != NULL){
            leitordearquivo = open(tarefa->arquivoentra, O_RDONLY);

            dup2(leitordearquivo, 0);
            close(leitordearquivo);
        }

        execvp(tarefa->caminho, tarefa->comando);

        printf("Erro no filho\n");
        exit(1);
    }

    return fok;
}

void procurar_arquivo(char *nome, char *tipo, char *arquivo){
    node *tarefa = procurar_tarefa(nome);
    
    if (tarefa == NULL){
        printf("Tarefa nao existe\n");
        return;
    }
    
    if (strcmp (tipo, "input") == 0){
        tarefa->arquivoentra = strdup(arquivo);

    }else if (strcmp (tipo, "output") == 0){
        tarefa->arquivosai = strdup(arquivo);
        tarefa->appendar = 0;

    }else if (strcmp (tipo, "append") == 0){
        tarefa->arquivosai = strdup(arquivo);
        tarefa->appendar = 1;
    }

}

void executar_JOB(char *nome){
    node *tarefa = procurar_tarefa(nome);

    if (tarefa == NULL){
        printf("Tarefa não encontrada\n");
        return;
    }

    pid_t fok = fork();
    if(fok == 0){
        if(tarefa->arquivosai != NULL){
            int leitordearquivo;

            if(tarefa->appendar == 1){
                leitordearquivo = open(tarefa->arquivosai, O_WRONLY | O_CREAT | O_APPEND, 0644);

            }else{
                leitordearquivo = open(tarefa->arquivosai, O_WRONLY | O_CREAT | O_TRUNC, 0644);

            }
            dup2(leitordearquivo, 1);
            close(leitordearquivo);

        }

        int leitordearquivo;
        if(tarefa->arquivoentra != NULL){
            leitordearquivo = open(tarefa->arquivoentra, O_RDONLY);

            dup2(leitordearquivo, 0);
            close(leitordearquivo);
        }

        execvp(tarefa->caminho, tarefa->comando);

        printf("Erro no filho\n");
        exit(1);

    }else if(fok > 0){
        job *novojob = malloc(sizeof(job));
        
        novojob->id = proxID;
        novojob->pid = fok;
        novojob->next = lista_de_jobs;

        lista_de_jobs = novojob;

        printf("[%d] %d\n", novojob->id, novojob->pid);

        proxID++;
    }
}

void procurar_JOB(){
    job *aux = lista_de_jobs;

    while(aux != NULL){
       printf("[%d] %d\n", aux->id, aux->pid);
    
        aux = aux->next;
    }
}

job *procurar_IDJOB(int id){

    job *aux = lista_de_jobs;

    while(aux != NULL){
       if (aux->id == id){
        return aux;
       }

       aux = aux->next;
    }
    return NULL;
}

void wait_job(int id){
    job *novojob = procurar_IDJOB(id);

    if(novojob == NULL){
        printf("Job não encontrado\n");
        return;
    }

    waitpid(novojob->pid, NULL, 0);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


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
            char *token2 = strtok(NULL, " ");
            
            if(strcmp (token2, "sequential") == 0){
                token2 = strtok(NULL, " ");

                while(token2 != NULL){
                    executar_tarefa(token2);
                    token2 = strtok(NULL, " ");
                }

            }else if(strcmp (token2, "parallel") == 0){
                pid_t *idfilho = malloc(sizeof(pid_t));

                char *tokenP;
                tokenP = strtok(NULL, " ");

                int contador = 0;

                while(tokenP != NULL){
                    idfilho = realloc(idfilho, sizeof(pid_t) * (contador+1));
                    idfilho[contador] = executar_tarefa_parallel(tokenP);

                    contador++;

                    tokenP = strtok(NULL, " ");
                }

                int i=0;
                while(i != contador){
                    waitpid(idfilho[i], NULL, 0);

                    i++;
                }

            }else if(strcmp (token2, "pipe") == 0){
                char **cano = malloc(sizeof(char*));

                char *tokenpipe;
                tokenpipe = strtok(NULL, " ");

                int contador = 0;

                while(tokenpipe != NULL){
                    cano = realloc(cano, sizeof(char*) * (contador+1));
                    cano[contador] = strdup(tokenpipe);

                    contador++;

                    tokenpipe = strtok(NULL, " ");
                }


                int **canos = malloc(sizeof(int*) * (contador - 1));

                for(int i = 0; i < contador - 1; i++){
                    canos[i] = malloc(sizeof(int) * 2);
                    pipe(canos[i]);

                }

                pid_t *idpipe = malloc(sizeof(pid_t) * contador);

                for (int i=0; i < contador; i++){
                    node *tarefa = procurar_tarefa(cano[i]);
                    idpipe[i] = fork();

                    if(idpipe[i] == 0){
                        
                        if(i > 0){
                            dup2(canos[i-1][0], 0);
                        }

                        if(i < contador - 1){
                            dup2(canos[i][1], 1);
                        }

                        for(int j = 0; j < contador - 1; j++){
                            close(canos[j][0]);
                            close(canos[j][1]);
                        }

                        execvp(tarefa->caminho, tarefa->comando);

                        printf("Erro no filho\n");
                        exit(1);
                    }
                }

                for(int i = 0; i < contador - 1; i++){
                    close(canos[i][0]);
                    close(canos[i][1]);

                }

                for(int i = 0; i < contador; i++){
                    waitpid(idpipe[i], NULL, 0);
                }

            }else{
                token2[strcspn(token2, "\n")] = '\0';
                executar_tarefa(token2);

            }

        }else if(strcmp(token1, "input") == 0 || strcmp(token1, "output") == 0 || strcmp(token1, "append") == 0){
            char *nome = strtok(NULL, " ");
            char *arquivo = strtok(NULL, " ");

            procurar_arquivo(nome, token1, arquivo);

        }else if(strcmp(token1, "start") == 0){
            char *nome = strtok(NULL, " ");

            executar_JOB(nome);
        
        }else if(strcmp(token1, "jobs") == 0){
            procurar_JOB();

        }else if(strcmp(token1, "wait") == 0){
            char *idjob = strtok(NULL, " ");

            int id = atoi(idjob);

            wait_job(id);

        }else if ((strcmp (token1, "exit") == 0)){
            break;

        }else{
            printf("Essa função não existe\n");
        }

    }while(1);
    
}