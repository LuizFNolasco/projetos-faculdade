#include<stdio.h>
#include<stdlib.h>
#include<string.h>

// Estrutura para armazenar apos quantos carcteres aparece a palavra
typedef struct aparicoes{
    int qtd;
    struct aparicoes *prox;
}aparicoes;

// Estrutura para armazenar a palavra e a lista de aparicoes
typedef struct elem{
    char chave[25];
    aparicoes *Lista;
}elem;

// Função para inicializar um nó que portará apos quantos caracteres que a palavra aparece
aparicoes *criaLista(int caracteres){
    aparicoes *novo = (aparicoes*)calloc(1, sizeof(aparicoes));

    if(novo == NULL){
        printf("Erro ao tentar alocar");
        return NULL;
    }
    novo->qtd = caracteres;
    novo->prox = NULL;
return novo;
}

// Função para encadear os nós que portarão apos quantos caracteres que a palavra aparece
aparicoes *insereFimAparicoes(aparicoes *ListaAparicoes, int caracteres){
    aparicoes *novo = criaLista(caracteres);
    aparicoes *aux = ListaAparicoes;

    if(ListaAparicoes == NULL){
        ListaAparicoes = novo;
        return NULL;
    }
    while(aux->prox != NULL){
        aux = aux->prox;
    }
    aux->prox = novo;
return aux;
}

int criarTabelaHash(){

}

//Função de hash usando o metodo da divisão
int funcaoHash(elem *tabelaHash, char palavra, int tamanhoTabela){
    int tamanhoPalavra = strlen(palavra);
    int posicao = tamanhoPalavra / 2, k = 1;
    int i = 0;

return posicao;
}

// Função para buscar uma palavra na tabela hash, caso a palavra esteja na tabela, a função irá
// inserir apos quantos caracteres a palavra aparece
int buscarPalavra(elem *tabelaHash, char palavra, int tamanhoTabela, int caracteres){
    int aux = funcaoHash(tabelaHash, palavra, tamanhoTabela);
    int i = 0,  k = 1, posicao = aux;

    while(aux < tamanhoTabela){
        if(tabelaHash[aux].chave == palavra){

            if(tabelaHash[aux].Lista == NULL){
                tabelaHash[aux].Lista = insereFimAparicoes(&tabelaHash[aux].Lista, caracteres);
                return 1;
            }

            while(tabelaHash[aux].Lista != NULL){
                if(tabelaHash[aux].Lista->qtd == caracteres){
                    return 1;
                }
                tabelaHash[aux].Lista = tabelaHash[aux].Lista->prox;
            }
        }
        aux = aux + k;
        k++;
    }
return 0;
}

void inserirPalavra(elem *tabelaHash, char palavra, int tamanhoTabela, int caracteres){
    int resultadoBusca = buscarPalavra(tabelaHash, palavra, tamanhoTabela, caracteres);
    int aux = funcaoHash(tabelaHash, palavra, tamanhoTabela);

    if(resultadoBusca == 0){
        strcpy(tabelaHash[aux].chave, palavra);
        tabelaHash[aux].Lista = NULL;
    }
}

int main(){
    elem palavras;
    char arqPalavrasChave[50];

    printf("Digite o nome do arquivo: ");
    scanf("%s", &arqPalavrasChave);

    FILE *arq;
    arq = fopen(arqPalavrasChave, "r");

    if(arq == NULL){
        printf("Erro ao tentar abrir o arquivo"); return 0;
    }
    
    fclose(arq);
    
return 0;
}