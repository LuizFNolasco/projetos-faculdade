#include<stdio.h>
#include<stdlib.h>
#include<string.h>

// Estrutura para armazenar apos quantos carcteres aparece a palavra
typedef struct aparicoes{
    int tamanho;
    struct aparicoes *prox;
}aparicoes;

// Estrutura para armazenar a palavra e a lista de apos quantos caracteres aparece a palavra
typedef struct elem{
    char *chave;
    aparicoes *Lista;
}elem;

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

//Função de hash usando o metodo da divisão
int funcaoHash(elem *tabelaHash, char palavra, int tamanhoTabela){
    int qtd = strlen(palavra);
    int posicao = (qtd * 2), k = 1;

    if(tamanhoTabela > posicao){
        posicao = tamanhoTabela / posicao;
    }else{
        posicao = posicao / tamanhoTabela;
    }

return posicao;
}

// Função para buscar uma palavra na tabela hash, caso a palavra esteja na tabela, a função irá
// inserir apos quantos caracteres a palavra aparece
int buscarPalavra(elem *tabelaHash, char palavra, int tamanhoTabela, int caracteres){
    int aux = funcaoHash(tabelaHash, palavra, tamanhoTabela);
    int i = 0,  k = 1, posicao = aux;

    while(aux < tamanhoTabela){
        if(tabelaHash[aux].chave == palavra){
            tabelaHash[aux].Lista = insereFimAparicoes(&tabelaHash[aux].Lista, caracteres);
        }
        aux = aux + k;
        k++;
    }
return 0;
}

void inserirPalavra(elem *tabelaHash, char palavra, int tamanhoTabela, int caracteres, int *tamanhoPalavra){
    int resultadoBusca = buscarPalavra(tabelaHash, palavra, tamanhoTabela, caracteres);
    int aux = funcaoHash(tabelaHash, palavra, tamanhoTabela);

    if(resultadoBusca == 0){
        strcpy(tabelaHash[aux].chave, palavra);
        tabelaHash[aux].Lista = NULL;

        //Guardar o tamanho da maior palavra
        if(strlen(palavra) > *tamanhoPalavra){
            *tamanhoPalavra = strlen(palavra);
        }
    }
}

//Função responsavel pela impressão da tabela inteira, até mesmo as casas com caracteres nulos
void imprimirTabela(elem *tabelaHash, int tamanhoTabela, int *tamanhoPalavra){
    
    for(int j = 0; j < *tamanhoPalavra + 9; j++){
        printf("_");
    }
    printf("\n");

    for(int i = 0; i < tamanhoTabela; i++){
        
        printf("| %d | %s | ", i, tabelaHash[i].chave);
        
        for(int k = 0; k < *tamanhoPalavra + 9; k++){
            printf("_");
        }
        printf("\n");
    }
}

//Organizar e imprimir uma palavra
void imprimirPalavras(){
    char vetorLetras[256] = {"Inicio", "À", "Á", "Â", "Ã", "A", "à", "á", "â", "ã", "B" "b", "C", "c", "D", "d", 
                            "È", "É", "Ê", "E", "è", "é", "ê", "e", "F", "f", "G", "g", "H", "h", "i", "j", "k", "l", "m",
                             "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z"};
}

//Função para aumentar a tabela hash
void aumentaTabela(elem *tabelaHash, int *tamanhoTabela){
    elem *novaTabela = (elem*)calloc((*tamanhoTabela * 2), sizeof(elem));

    if(novaTabela == NULL){
        printf("Erro ao tentar alocar");
        return;
    }

    for(int i = 0; i < *tamanhoTabela; i++){
        strcpy(novaTabela[i].chave, tabelaHash[i].chave);
        novaTabela[i].Lista = tabelaHash[i].Lista;
    }

    *tamanhoTabela = *tamanhoTabela * 2;
    
    free(tabelaHash);
    
    tabelaHash = novaTabela;
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