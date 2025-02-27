#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <ctype.h>
#include <locale.h>

// Estrutura para armazenar apos quantos carcteres aparece a palavra
typedef struct aparicoes{
    int tamanho;
    struct aparicoes *prox;
}aparicoes;

// Estrutura para armazenar a palavra e a lista de apos quantos caracteres aparece a palavra
typedef struct tabela{
    char *chave;
    aparicoes *Lista;
}tabela;

typedef struct no{
    char *palavra;
    struct no *prox;
}no;

//Cria o no que guardará a lista de palavras chave
no *criaNo(char *palavra, int tamanhoPalavra){
    no *novo = (no*)calloc(1, sizeof(no));

    if(novo == NULL){
        printf("Erro ao tentar alocar");
        return NULL;
    }
    novo->palavra = (char*)calloc(tamanhoPalavra + 1, sizeof(char));
    strcpy(novo->palavra, palavra);

    novo->prox = NULL;

return novo;
}

void insereInicioListaPalavras(no **ListaPalavra, char *palavra, int tamanhoPalavra){
    no *novo = criaNo(palavra, tamanhoPalavra);

    novo->prox = *(ListaPalavra);
    *(ListaPalavra) = novo;
}

void exibirLista(no *listaPalavras) {
    no *atual = listaPalavras;
    
    while (atual != NULL) {
        printf("%s\n", atual->palavra);
        atual = atual->prox;
    }
}

void liberaListaPalavra(no **listaPalavras) {
    no *atual = *listaPalavras;
    no *proximo;

    while (atual != NULL) {
        proximo = atual->prox;

        free(atual->palavra);
        free(atual);
        
        atual = proximo;
    }
}

void criaTabela(tabela *tabelaHash, int tamanhoTabela) {
    int i;
    for(i = 0; i < tamanhoTabela; i++) {
        tabelaHash[i].chave = NULL;
        tabelaHash[i].Lista = NULL;
    }
}

void liberaTabelaHash(tabela *tabelaHash, int tamanhoTabela) {
    int i;

    aparicoes *atual = tabelaHash[i].Lista;
    aparicoes *proximo;

    for(i = 0; i < tamanhoTabela; i++) {
        if(tabelaHash[i].chave != NULL) {
            free(tabelaHash[i].chave);

            // Liberar a lista de aparições
            while(atual != NULL) {
                proximo = atual->prox;

                free(atual);

                atual = proximo;
            }
        }
    }
}

// Função para inicializar um nó que portará apos quantos caracteres que a palavra aparece
aparicoes *criaLista(int caracteres){
    aparicoes *novo = (aparicoes*)calloc(1, sizeof(aparicoes));

    if(novo == NULL){
        printf("Erro ao tentar alocar");
        return NULL;
    }
    novo->tamanho = caracteres;
    novo->prox = NULL;

return novo;
}

// Função para encadear os nós que portarão apos quantos caracteres que a palavra aparece
aparicoes *insereFimAparicoes(aparicoes *listaAparicoes, int caracteres){
    aparicoes *novo = criaLista(caracteres);
    aparicoes *aux = listaAparicoes;

    if(listaAparicoes == NULL){
        return novo;
    }
    while(aux->prox != NULL){
        aux = aux->prox;
    }
    aux->prox = novo;
return listaAparicoes;
}

//Função de hash usando o metodo da divisão
int funcaoHash(int tamanhoPalavra, int tamanhoTabela){
    int posicao = (tamanhoPalavra * 2), k = 1;

    if(tamanhoPalavra == 0){
        return 0;
    }

    //Caso seja um numero de 3 digitos, transformar ele em 2 digitos
    if(posicao / 100 != 0){
        posicao = posicao / 10;
    }

    if(tamanhoTabela > posicao){
        posicao = tamanhoTabela / posicao;
    }else{
        posicao = posicao / tamanhoTabela;
    }

return posicao;
}

// Função para buscar uma palavra na tabela hash, caso a palavra esteja na tabela, a função irá
// inserir apos quantos caracteres a palavra aparece
int buscarPalavra(tabela *tabelaHash, char *palavra, int tamanhoTabela, int caracteres, int tamanhoPalavra){
    int aux = funcaoHash(tamanhoPalavra, tamanhoTabela);
    int k = 1;

    while(aux < tamanhoTabela){
        if(tabelaHash[aux].chave != NULL && strcmp(tabelaHash[aux].chave, palavra) == 0){
            tabelaHash[aux].Lista = insereFimAparicoes(tabelaHash[aux].Lista, caracteres);
            
            return 1;
        }
        aux = aux + k;
        k++;
    }
return 0;
}

void inserirPalavra(tabela *tabelaHash, char *palavra, int tamanhoTabela, int caracteres, int tamanhoPalavra){    
    int resultadoBusca = buscarPalavra(tabelaHash, palavra, tamanhoTabela, caracteres, tamanhoPalavra);
    int aux = funcaoHash(tamanhoPalavra, tamanhoTabela);
    int k = 1;

    if(resultadoBusca == 0){
        
        while(tabelaHash[aux].chave != NULL && aux < tamanhoTabela){
            aux = aux + k;
            k++;
        }
        
        tabelaHash[aux].chave = (char*)calloc(tamanhoPalavra + 1, sizeof(char));
        strcpy(tabelaHash[aux].chave, palavra);

        tabelaHash[aux].Lista = criaLista(caracteres);
    }
}

//Função responsavel pela impressão da tabela inteira, até mesmo as casas com caracteres nulos
void imprimirTabela(tabela *tabelaHash, int tamanhoTabela, int tamanhoPalavra){
    int i;

    printf("--------------------------------\n");

    for(i = 0; i < tamanhoTabela; i++) {
        printf("| %-7d | ", i);
        
        if(tabelaHash[i].chave != NULL) {
            printf("%-18s |\n", tabelaHash[i].chave);
        } else {
            printf("%-18s |\n", "");
        }
    }
    
    printf("--------------------------------\n");
}

int main(){
    tabela tabelaHash[81]; // 9x9
    no *listaPalavra = NULL;
    aparicoes *listaAparicoes = NULL;

    char arqPalavrasChave[1000];
    int caractereAtual, tamanhoPalavraFormada = 100, indice = 0;
    int posicaoTabela = 0, tamanhoTabela = 81, caracteres = 1;

    char *palavraFormada = (char*)calloc(tamanhoPalavraFormada, sizeof(char));

    criaTabela(tabelaHash, tamanhoTabela);  

    //Para o isalnum receber valores que pentencem a ligua portuguesa
    setlocale(LC_ALL, "pt_BR.iso88591");

    //Usando o arquivo de palavras chaves
    printf("Digite o nome do arquivo de palavras chaves: ");
    scanf("%s", arqPalavrasChave);

    FILE *arqP;
    arqP = fopen(arqPalavrasChave, "r");

    if(arqP == NULL){
        printf("Erro ao tentar abrir o arquivo"); 
        free(palavraFormada);
        return 0;
    }

    while((caractereAtual = fgetc(arqP)) != EOF){
        //Enquanto o caractere atual for diferente de espaço e pontuação a palavra é formada
        if(caractereAtual != ' ' && caractereAtual != '.' && caractereAtual != ',' && caractereAtual != ';' && caractereAtual != ':'){
            //Caso passe o numero maximo de caracteres
            if(indice >= tamanhoPalavraFormada - 1){
                tamanhoPalavraFormada *= 2;
                palavraFormada = (char*)realloc(palavraFormada, tamanhoPalavraFormada * sizeof(char));
            }
            palavraFormada[indice] = caractereAtual;
            indice++;

        // Indice > 0 significa que a palavra foi formada e chegou em um espaço
        }else if(indice > 0){
            palavraFormada[indice] = '\0';
            insereInicioListaPalavras(&listaPalavra, palavraFormada, indice);

            inserirPalavra(tabelaHash, palavraFormada, tamanhoTabela, caracteres, indice);

            indice = 0;
        }
        caracteres++;
    }
    //Verifica se chegou ao final do arquivo e esqueceu uma palavra
    if(indice > 0) {
        palavraFormada[indice] = '\0';
        insereInicioListaPalavras(&listaPalavra, palavraFormada, indice);

        inserirPalavra(tabelaHash, palavraFormada, tamanhoTabela, caracteres, indice);
    }

    fclose(arqP);
    free(palavraFormada);

    exibirLista(listaPalavra);
    imprimirTabela(tabelaHash, tamanhoTabela, 10);

    liberaListaPalavra(&listaPalavra); 
    liberaTabelaHash(tabelaHash, tamanhoTabela);
return 0;
}