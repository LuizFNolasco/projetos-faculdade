/*
Grupo 4
Integrantes: Emerson Boone, Thaelen de Morais, Luiz Felipe Pereira
*/

#ifndef __TABELAHASH_H__
#define __TABELAHASH_H__

//Bibliotecas utilizadas
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<locale.h>
#include<iconv.h>

//Estrutura responsável por formar uma lista encadeada de após quantos caracteres a palavra aparece
typedef struct aparicoes{
    int tamanho;
    struct aparicoes *prox;
}aparicoes;

//Estrutura responsável pela tabela hash, armazena a palavra-chave, uma versão dela com e sem acento e uma lista 
//encadeada de após quantos caracteres a palavra apareceu
typedef struct tabela{
    char *chave;
    char *chaveSemAcento;
    aparicoes *Lista;
}tabela;

//Estrutura responsável por formar uma lista encadeada de nós das palavras-chave sem acento para realizar a ordenação de forma alfabética
typedef struct no{
    char *palavraNo;
    struct no *prox;
}no;

/*
Função:
    criaTabela
Descrição:
    Responsável por inicializar os campos da estrutura da tabela hash
Entrada:
    tabela *tabelaHash: um ponteiro para o vetor de registro da estrutura tabela
    int tamanhoTabela: o atual tamanho da tabela hash
Saida:
    void
*/
void criaTabela(tabela *tabelaHash, int tamanhoTabela);

/*
Função:
    excluirTabela
Descrição:
    Responsável por excluir a tabela, realizando uma série de frees que irão liberar a memória alocada para a palavra-chave com e sem acento e a lista encadeada que cada palavra possui
Entrada:
    tabela *tabelaHash: um ponteiro para o vetor de registro da estrutura tabela
    int tamanho: o atual tamanho da tabela
Saida:
    void
*/
void excluirTabela(tabela *tabelaHash, int tamanhoTabela);

/*
Função:
    *criaLista
Descrição:
    Cria um nó da estrutura aparicoes e inicializa o próprio com o número de após quantos caracteres a palavra aparece
Entrada:
    int caracteres: o número de após quantos caracteres a palavra aparece
Saida:
    Um nó com dados da estrutura aparições 
*/
aparicoes *criaLista(int caracteres);

/*
Função:
    *insereFimAparicoes
Descrição:
    Insere o novo nó no fim da lista encadeada de aparições da palavra
Entrada:
    aparicoes *listaAparicoes: ponteiro para a lista de após quantos caracteres a palavra aparece
    int caracteres: o número de após quantos caracteres a palavra aparece
Saida:
    A lista encadeada atualizada em relação da quantidade de vezes que a palavra aparece e após quantos caracteres a palavra aparece
*/
aparicoes *insereFimAparicoes(aparicoes *listaAparicoes, int caracteres);

/*
Função:
    *AjeitaPalavra
Descrição:
    Retira todos os caracteres acentuados, maiúsculos ou especiais
Entrada:
    char *novaPalavra: ponteiro para a palavra que será mudada
    int tamanhoPalavra: o tamanho da palavra que será mudada
Saida:
    A nova palavra, sem caracteres acentuados, maiúsculos ou especiais
*/
char *ajeitaPalavra(char *novaPalavra, int tamanhoPalavra);

/*
Função:
    funcaoHash
Descrição:
    Responsável por calcular o índice onde a palavra se encontrará na tabela hash
Entrada:
    int tamanhoPalavra: o tamanho da palavra
    int tamanhoTabela: o atual tamanho da tabela
Saida:
    Um inteiro com o código gerado após o cálculo feito com a palavra
*/
int funcaoHash(int tamanhoPalavra, int tamanhoTabela);

/*
Função:
    buscaPalavra
Descrição:
    Busca a palavra na tabela hash, caso ache a palavra insere na lista encadeada o número de caracteres que aparecem após a ocorrência da palavra
Entrada:
    tabela *tabelaHash: um ponteiro para o vetor de registro da estrutura tabela
    char *palavra: a palavra que será buscada na tabela
    int tamanhoTabela: o atual tamanho da tabela
    int caracteres: após quantos caracteres a palavra aparece
    int tamanhoPalavra: o tamanho da palavra que será buscada
Saida:
    Um inteiro indicado se a palavra está ou não na tabela (1 ou 0 respectivamente)
*/
int buscarPalavra(tabela *tabelaHash, char *palavra, int tamanhoTabela, int caracteres, int tamanhoPalavra);

/*
Função:
    inserirPalavra
Descrição:
    Insere a palavra na tabela hash conforme o seu código hash gerado pela funcaoHash, usa a buscaPalavra para saber se a palavra está ou não na tabela
Entrada:
    tabela *tabelaHash: um ponteiro para o vetor de registro da estrutura 
    char *palavra: ponteiro para a palavra que será inserida na tabela hash
    int tamanhoTabela: atual tamanho da tabela hash
    int caracteres: após quantos caracteres a palavra aparece
    int tamanhoPalavra: tamanho da palavra
Saida:
    void
*/
void inserirPalavra(tabela *tabelaHash, char *palavra, int tamanhoTabela, int caracteres, int tamanhoPalavra);

/*
Função:
    imprimirTabela
Descrição:
    Imprime toda a tabela hash, mostrando a palavra e a posição em que se encontra
Entrada:
    tabela *tabelaHash: ponteiro para o vetor de registros da estrutura tabela
    int tamanhoTabela: atual tamanho da tabela
Saida:
    void
*/
void imprimirTabela(tabela *tabelaHash, int tamanhoTabela);

/*
Função:
    *criaNo
Descrição:
    Cria um nó da versão da palavra sem acentos, letras maiúsculas e caracteres especiais
Entrada:
    char *palavra: a palavra que será inserida no nó
    int tamanhoPalavra: tamanho da palavra
Saida:
    Um nó com a versão da palavra sem acentos, caracteres especiais e letras maiúsculas
*/
no *criaNo(char *palavra, int tamanhoPalavra);

/*
Função:
    insereOrdPalavra
Descrição:
    Transforma a palavra em uma versão sem acentos, caracteres especiais e letras maiúsculas (com ajuda da função ?ajeitaPalavra?)e insere na lista de palavras ordenadamente (com ajuda da função ?strcmp?)
Entrada:
    no **listaPalavra: ponteiro para a lista de palavras sem acentos, caracteres especiais e letras maiúsculas
    char *palavra: ponteiro para a palavra que será inserida na lista
    int tamanhoPalavra: tamanho da palavra
Saida:
    void
*/
void insereOrdPalavra(no **listaPalavra, char *palavra, int tamanhoPalavra);

/*
Função:
    liberaListaPalavras
Descrição:
    Liberar o espaço da memória guardado para a lista de palavras ordenadas e sem acento
Entrada:
    no **listaPalavras: ponteiro para a lista de palavras sem acento, caracteres especiais e letras maiúsculas
Saida:
    void
*/
void liberaListaPalavra(no **listaPalavras);

/*
Função:
    imprimeOrdTabela
Descrição:
    Imprime as palavras-chave e a após quantos caracteres é a sua ocorrência
Entrada:
    tabela *tabelaHash: ponteiro para o vetor de registros da estrutura tabela
    no *listaPalavras: lista encadeada ordenada com todas as palavras-chave sem acento, caracteres especiais e letras maiúsculas
Saida:
    void
*/
void imprimeOrdTabela(tabela *tabelaHash, no *listaPalavras, int tamanhoTabela);

/*
Função:
    aumentaTabela
Descrição:
    Aumenta o número da tabela para o dobro do atual
Entrada:
    tabela **tabelaHash: ponteiro de ponteiro para o vetor de registros da estrutura tabela
    no *listaPalavras: ponteiro para a lista encadeada de palavras sem acento, caracteres especiais e letras maiúsculas
    int *tamanhoTabela: ponteiro para o tamanho da tabela
Saida:
    void
*/
void aumentaTabela(tabela **tabelaHash, no *listaPalavras, int *tamanhoTabela);

/*
Função:
    recebeArqPalavrasChave
Descrição:
    Abre o arquivo de palavras-chave, inserindo as versões com e sem acentos, caracteres especiais e letras maiúsculas na tabela hash e a versão sem acentos, caracteres especiais e letras maiúsculas na lista encadeada
Entrada:
    tabela **tabelaHash: ponteiro para o ponteiro do vetor de registros da estrutura tabela
    no **listaPalavras: ponteiro de ponteiro para a lista encadeada com as palavras sem acento, caracteres especiais e letras maiúsculas
    char *arqPalavrasChave: nome do arquivo que o usuário deseja abrir
    char *palavraFormada: ponteiro para a junção de caracteres que formam uma possível palavra-chave
    int caractereAtual: atual caractere lido do arquivo
    int indice: posição que o caractere lido irá ocupar na palavraFormada
    int tamanhoPalavraFormada: quantidade de caracteres lidos do início ao fim da palavra
    int caracteres: numero de caracteres lidos no arquivo de palavra-chave
    int tamanhoTabela: atual tamanho da tabela hash
Saida:
    void
*/
void recebeArqPalavrasChave(tabela **tabelaHash, no **listaPalavra, char *arqPalavrasChave, char *palavraFormada, int caractereAtual, int indice, 
    int tamanhoPalavraFormada, int caracteres, int tamanhoTabela);

/*
Função:
    recebeArqTexto
Descrição:
    Lê o arquivo de texto para contabilizar a ocorrência das palavras-chave e após quantos caracteres cada uma delas aparece e insere na tabela hash os dois dados citados
Entrada:
    tabela **tabelaHash: ponteiro para o ponteiro do vetor de registros da estrutura tabela
    char *arqTexto: ponteiro para o nome do arquivo de texto que será aberto
    char *palavraFormada: ponteiro para a palavra formada pela junção de caracteres lidos do arquivo de texto
    int caractereAtual: atual caractere lido do arquivo de texto
    int indice: posição que o caractere lido irá ocupar na palavraFormada
    int tamanhoTabela: atual tamanho da tabela hash
    int caracteres: quantidade de caracteres lidos do arquivo texto
    int tamanhoPalavraFormada: tamanho da palavra encontrada no arquivo texto
Saida:
    void
*/
void recebeArqTexto(tabela **tabelaHash, char *arqTexto, char *palavraFormada, int caractereAtual, int indice, int tamanhoTabela, 
    int caracteres, int tamanhoPalavraFormada);

/*
Função:
    converterArq
Descrição:
    Converte um arquivo para iso 8859-1
Entrada:
    const char *arquivoEntrada: arquivo que terá o encoding alterado
    const char *arquivoSaida: arquivo com o encoding alterado
Saida:
    void
*/
void converterArq(const char *arquivoEntrada, const char *arquivoSaida);

#endif