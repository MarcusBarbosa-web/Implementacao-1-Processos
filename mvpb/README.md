# ProcessFlow

Orquestrador de processos que cria e executa tarefas via fork/exec/wait.

## Arquivos

- `src/Programa.c` — código-fonte completo do ProcessFlow
- `src/exemplo.pf` — arquivo de workflow de exemplo (modo não-interativo)
- `Makefile` — faz a compilação, limpeza e teste
- `src/evidencias.log` — log dos testes executados durante o desenvolvimento

## Como compilar

Na pasta raiz do projeto, onde tá o Makefile, use o comando abaixo:

    make

O executável é gerado em `src/output/Programa`.

## Como executar

Modo interativo: digite os comandos direto no prompt `Processflow>`:

    cd src
    ./output/Programa

Modo workflow: lê os comandos de um arquivo `.pf`:

    cd src
    ./output/Programa exemplo.pf

## Como testar

    make test

Roda o `exemplo.pf` automaticamente.

## Como limpar

    make clean

Remove o executável compilado.

## Sistema operacional

Desenvolvido e testado em WSL (Ubuntu) rodando sobre Windows.
