/*
    Grupo 3
    Integrantes: Eliaquim Torres, Luiz Felipe Pereira, Murilo Cunha.
*/

#include "TrabalhoED1.h"

void criaFila(fila *f) {
    f->inicio = NULL;
    f->passos = 0;
}

posicao *criaFilaDePosicoes(int coluna, int linha) {
    posicao *novo = (posicao*)calloc(1, sizeof(posicao));
    
    if(novo == NULL) { printf("\nERRO DE ALOCACAO \n"); exit(1); }

    novo->colun = coluna;
    novo->lin = linha;
    novo->prox = NULL;

return novo;
}

void insereNaFila(fila *f, int coluna, int linha) {
    posicao *novo = criaFilaDePosicoes(coluna, linha);

    if(f->inicio == NULL) {
        f->inicio = novo;
    } else {
        posicao *aux = f->inicio;
        while(aux->prox != NULL) {
            aux = aux->prox;
        }
        aux->prox = novo;
    }
}

posicao *removeDaFila(fila *f) {

    if(f->inicio == NULL) return NULL;

    posicao *removido = f->inicio;
    f->inicio = f->inicio->prox;
    removido->prox = NULL;

return removido;
}

char **criaMatriz(int numeroDeColunas, int numeroDeLinhas, posicao **tributo, int *numeroBestante) {
    int i, j;
    *numeroBestante = 0;

    char **matriz = (char **)calloc(numeroDeColunas, sizeof(char*));

    for(i = 0; i < numeroDeColunas; i++) {
        matriz[i] = (char*)calloc(numeroDeLinhas + 1, sizeof(char));
    }

    for(i = 0; i < numeroDeColunas; i++) {
        for(j = 0; j < numeroDeLinhas; j++) {
            scanf(" %c", &matriz[i][j]);
            if(matriz[i][j] == 'A') {
                *tributo = criaFilaDePosicoes(i, j);
            } else if(matriz[i][j] == 'M') {
                (*numeroBestante)++;
            }
        }
    }

return matriz;
}

int LocalValido(int x, int y, int numeroDeColunas, int numeroDeLinhas, char **matriz) {
    
    if(x >= 0 && x < numeroDeColunas && y >= 0 && y < numeroDeLinhas && matriz[x][y] != '#'){
        return 1;
    }
return 0;
}

void BuscaBestante(char **matriz, int numeroDeColunas, int numeroDeLinhas, int **tempoBestante) {
    int i, j;

    int direcaoX[4] = {-1, 1, 0, 0};
    int direcaoY[4] = {0, 0, -1, 1};

    fila f;
    criaFila(&f);

    for(i = 0; i < numeroDeColunas; i++) {
        for(j = 0; j < numeroDeLinhas; j++) {
            if(matriz[i][j] == 'M') {
                insereNaFila(&f, i, j);
                tempoBestante[i][j] = 0;
            }
        }
    }

    while(f.inicio != NULL) {
        posicao *atual = removeDaFila(&f);
        for(i = 0; i < 4; i++) {
            int novoX = atual->colun + direcaoX[i];
            int novoY = atual->lin + direcaoY[i];
            if(LocalValido(novoX, novoY, numeroDeColunas, numeroDeLinhas, matriz) && tempoBestante[novoX][novoY] == -1) {
                tempoBestante[novoX][novoY] = tempoBestante[atual->colun][atual->lin] + 1;
                insereNaFila(&f, novoX, novoY);
            }
        }
        free(atual);
    }
}

int BuscaTributo(char **matriz, int numeroDeColunas, int numeroDeLinhas, posicao *tributo, int **tempoBestante, int **tempoTributo) {
    int i;

    int direcaoX[4] = {-1, 1, 0, 0};
    int direcaoY[4] = {0, 0, -1, 1};
    char direcaoChar[4] = {'U', 'D', 'L', 'R'};

    fila f;
    criaFila(&f);

    insereNaFila(&f, tributo->colun, tributo->lin);
    tempoTributo[tributo->colun][tributo->lin] = 0;

    while(f.inicio != NULL) {
        posicao *atual = removeDaFila(&f);
        int x = atual->colun;
        int y = atual->lin;
        //Ja ta na estrutura
        int passos = tempoTributo[x][y];

        if(x == 0 || x == numeroDeColunas - 1 || y == 0 || y == numeroDeLinhas - 1) {
            printf("YES\n%d\n", passos);
            char *caminho = (char*)calloc(passos + 1, sizeof(char));
            int idx = passos - 1;
            while(passos > 0) {
                for(i = 0; i < 4; i++) {
                    int prevX = x - direcaoX[i];
                    int prevY = y - direcaoY[i];
                    if(LocalValido(prevX, prevY, numeroDeColunas, numeroDeLinhas, matriz) && tempoTributo[prevX][prevY] == passos - 1) {
                        caminho[idx--] = direcaoChar[i];
                        x = prevX;
                        y = prevY;
                        passos--;
                        break;
                    }
                }
            }
            printf("%s\n", caminho);
            free(caminho);
            return 1;
        }

        for(i = 0; i < 4; i++) {
            int novoX = x + direcaoX[i];
            int novoY = y + direcaoY[i];
            if(LocalValido(novoX, novoY, numeroDeColunas, numeroDeLinhas, matriz) && tempoTributo[novoX][novoY] == -1) {
                if(tempoBestante[novoX][novoY] == -1 || tempoBestante[novoX][novoY] > passos + 1) {
                    tempoTributo[novoX][novoY] = passos + 1;
                    insereNaFila(&f, novoX, novoY);
                }
            }
        }
        free(atual);
    }
return 0;
}