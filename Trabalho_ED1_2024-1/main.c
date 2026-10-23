/*
    Grupo 3
    Integrantes: Eliaquim Torres, Luiz Felipe Pereira, Murilo Cunha.
*/
#include "TrabalhoED1.h"

int main() {
    int numeroDeColunas, numeroDeLinhas, numeroBestante, i, j;
    char **matriz = NULL;
    posicao *tributo = NULL;
    scanf("%d %d", &numeroDeColunas, &numeroDeLinhas);

    matriz = criaMatriz(numeroDeColunas, numeroDeLinhas, &tributo, &numeroBestante);

    int **tempoBestante = (int**)calloc(numeroDeColunas, sizeof(int*));
    int **tempoTributo =  (int**)calloc(numeroDeColunas, sizeof(int*));

    for(i = 0; i < numeroDeColunas; i++) {
        tempoBestante[i] = (int*)calloc(numeroDeLinhas, sizeof(int));
        tempoTributo[i] =  (int*)calloc(numeroDeLinhas, sizeof(int));
        for(j = 0; j < numeroDeLinhas; j++){
            tempoBestante[i][j] = -1;
            tempoTributo[i][j] = -1;
        }
    }

    BuscaBestante(matriz, numeroDeColunas, numeroDeLinhas, tempoBestante);

    if(!BuscaTributo(matriz, numeroDeColunas, numeroDeLinhas, tributo, tempoBestante, tempoTributo)) {
        printf("NO\n");
    }

    // Liberar memória
    for(i = 0; i < numeroDeColunas; i++) {
        free(matriz[i]);
        free(tempoBestante[i]);
        free(tempoTributo[i]);
    }
    free(matriz);
    free(tempoBestante);
    free(tempoTributo);
    free(tributo);

return 0;
}