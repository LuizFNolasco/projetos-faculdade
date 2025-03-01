#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <ctype.h>
#include <locale.h>
//#include "iconv.h"

//
typedef struct aparicoes{
    int tamanho;
    struct aparicoes *prox;
}aparicoes;

//
typedef struct tabela{
    char *chave;
    char *chaveSemAcento;
    aparicoes *Lista;
}tabela;

//
typedef struct no{
    char *palavraNo;
    struct no *prox;
}no;

/*
Função:

Descrição:

Entrada:

Saida:

*/
void criaTabela(tabela *tabelaHash, int tamanhoTabela);

/*
Função:

Descrição:

Entrada:

Saida:

*/
void excluirTabela(tabela *tabelaHash, int tamanhoTabela);

/*
Função:

Descrição:

Entrada:

Saida:

*/
aparicoes *criaLista(int caracteres);

/*
Função:

Descrição:

Entrada:

Saida:

*/
aparicoes *insereFimAparicoes(aparicoes *listaAparicoes, int caracteres);

/*
Função:

Descrição:

Entrada:

Saida:

*/
char *ajeitaPalavra(char *novaPalavra, int tamanhoPalavra);

/*
Função:

Descrição:

Entrada:

Saida:

*/
int funcaoHash(int tamanhoPalavra, int tamanhoTabela);

/*
Função:

Descrição:

Entrada:

Saida:

*/
int buscarPalavra(tabela *tabelaHash, char *palavra, int tamanhoTabela, int caracteres, int tamanhoPalavra);

/*
Função:

Descrição:

Entrada:

Saida:

*/
void inserirPalavra(tabela *tabelaHash, char *palavra, int tamanhoTabela, int caracteres, int tamanhoPalavra);

/*
Função:

Descrição:

Entrada:

Saida:

*/
void imprimirTabela(tabela *tabelaHash, int tamanhoTabela, int tamanhoPalavra);

/*
Função:

Descrição:

Entrada:

Saida:

*/
no *criaNo(char *palavra, int tamanhoPalavra);

/*
Função:

Descrição:

Entrada:

Saida:

*/
void insereOrdPalavra(no **listaPalavra, char *palavra, int tamanhoPalavra);

/*
Função:

Descrição:

Entrada:

Saida:

*/
void exibirLista(no *listaPalavras);

/*
Função:

Descrição:

Entrada:

Saida:

*/
void liberaListaPalavra(no **listaPalavras);

/*
Função:

Descrição:

Entrada:

Saida:

*/
void imprimeOrdTabela(tabela *tabelaHash, no *listaPalavras, int tamanhoTabela);

/*
Função:

Descrição:

Entrada:

Saida:

*/
void aumentaTabela(tabela **tabelaHash, no *listaPalavras, int *tamanhoTabela);

/*
Função:

Descrição:

Entrada:

Saida:

*/
void recebeArqPalavrasChave(tabela **tabelaHash, no **listaPalavra, char *arqPalavrasChave, char *palavraFormada, int caractereAtual, int indice, 
    int tamanhoPalavraFormada, int possivelFinal, int caracteres, int tamanhoTabela, int j);

/*
Função:

Descrição:

Entrada:

Saida:

*/
void recebeArqTexto(tabela **tabelaHash, char *arqTexto, char *palavraFormada, int caractereAtual, int possivelFinal, int indice, int tamanhoTabela, 
    int caracteres, int tamanhoPalavraFormada, int j);

/*
Função:

Descrição:

Entrada:

Saida:

*/
void converterParaISO8859_1(const char *arquivoEntrada, const char *arquivoSaida);

/*
Função:

Descrição:

Entrada:

Saida:

*/
void menu();


/*
// Converte o encoding do arquivo lido para ISO-8859-1
char *convertEncoding(const char *input, const char *fromEncoding, const char *toEncoding) {
    iconv_t cd = iconv_open(toEncoding, fromEncoding);
    if (cd == (iconv_t)-1) {
        perror("iconv_open");
        return NULL;
    }

    size_t inBytesLeft = strlen(input);
    size_t outBytesLeft = inBytesLeft * 2; // Buffer de saída maior para garantir espaço suficiente
    char *output = (char *)malloc(outBytesLeft);
    if (output == NULL) {
        perror("malloc");
        iconv_close(cd);
        return NULL;
    }

    char *inBuf = (char *)input;
    char *outBuf = output;
    if (iconv(cd, &inBuf, &inBytesLeft, &outBuf, &outBytesLeft) == (size_t)-1) {
        perror("iconv");
        free(output);
        iconv_close(cd);
        return NULL;
    }

    *outBuf = '\0'; // Adicionar o caractere nulo ao final da string convertida
    iconv_close(cd);
    return output;
}
*/