/*
    Grupo 3
    Integrantes: Eliaquim Torres, Luiz Felipe Pereira, Murilo Cunha.
*/

#ifndef __TRABALHOED1_H__
#define __TRABALHOED1_H__

//Inclusão das bibliotecas necessarias para a implementação do programa
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Definição da estrutura e das variaveis responsaveis por guardar a posição em que o tributo
//                          ou o bestante estão na matriz
typedef struct posicao {
    int lin;
    int colun;
    struct posicao *prox;
} posicao;

//Definição da estrutura e das variaveis para a criação da fila de posições
typedef struct fila {
    posicao *inicio;
    int passos;
} fila;

/*
Função:
    criaFila
Descrição:
    Inicializa fila que será a fila de posições como vazia
Entrada:
    fila *f: ponteiro para a estrutura fila
Saida:
    void 
*/
void criaFila(fila *f);

/*
Função:
    criaFilaDePosicoes
Descrição:
    Cria um novo nó na lista simplesmente encadeada para a fila de posições
Entrada:
    int coluna: cordenada x, posição do elemento na matriz
    int linha: cordenada y, posição do elemento na matriz
Saida:
    Retorna um ponteiro para o nó que será a nova posição
*/
posicao *criaFilaDePosicoes(int coluna, int linha);

/*
Função:
    insereNaFila
Descrição:
    Atualiza a fila com a nova posição do atributo ou bestante
Entrada:
    fila *f: ponteiro para a fila de posições
    int coluna: cordenada x da nova posição
    int linha: cordenada y da nova posição
Saida:
    void
*/
void insereNaFila(fila *f, int coluna, int linha);

/*
Função:
    removeDaFila
Descrição:
    Remove e retorna a primeira posição da fila
Entrada:
    fila *f: ponteiro da fila de posições   
Saida:
    Retorna a posição removida do inicio da fila, se a fila estiver vazia, retorna NULL
*/
posicao *removeDaFila(fila *f);

/*
Função:
    criaMatriz
Descrição:
    Cria e inicializa matriz do jogo
Entrada:
    int numeroDeColunas: numero de colunas da matriz
    int numeroDeLinhas: numero de linhas da matriz
    posicao **tributo:  ponteiro para ponteiro da posição inicial do tributo
    int *numeroBestantes: ponteiro para o numero de bestantes
Saida:
    retorna a matriz criada
*/
char **criaMatriz(int numeroDeColunas, int numeroDeLinhas, posicao **tributo, int *numeroBestante);

/*
Função:
    LocalValido
Descrição:
    Verifica se uma posição na matriz é valida ou não
Entrada:
    int x: cordenada x da posição que será verificada
    int y: cordenada y da posição que será verificada
    int numeroDeColunas: numero total de colunas da matriz
    int numeroDeLinhas: numero total de linhas da matriz
    char **matriz: matriz do jogo
Saida:
    retorna 1 se a posição for valida e 0 se não for
*/
int LocalValido(int x, int y, int numeroDeColunas, int numeroDeLinhas, char **matriz);

/*
Função:
    BuscaBestante
Descrição:
    Realiza uma busca em largura para calcular quanto tempo os bestantes demoram
    para chegar na saida
Entrada:
    char **matriz: matriz do jogo
    int numeroDeColunas: numero de colunas da matriz
    int numeroDeLinhas: numero de linhas da matriz  
    int **tempoBestante: matriz para armazenar o tempo dos bestantes
Saida:
    void
*/
void BuscaBestante(char **matriz, int numeroDeColunas, int numeroDeLinhas, int **tempoBestante);

/*
Função:
    BuscaTributo
Descrição:
    Realiza uma busca em largura para encontrar um caminho para fuga do tributo, 
    se encontrar imprime "YES" e o caminho que ele percorreu
Entrada:
    char **matriz: matriz do jogo
    int numeroDeColunas: numero de colunas da matriz
    int numeroDeLinhas: numero de linhas da matriz
    posicao *tributo: posição inicial do tributo
    int **tempoBestante: matriz com o tempo dos bestantes
    int **tempoTributo: matriz com o tempo do tributo
Saida:
    Se o tributo encontrar um caminho para fuga retorna 1, se não retorna 0
*/
int BuscaTributo(char **matriz, int numeroDeColunas, int numeroDeLinhas, posicao *tributo, int **tempoBestante, int **tempoTributo);

#endif