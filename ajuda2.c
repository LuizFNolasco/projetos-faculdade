#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <ctype.h>
#include <locale.h>
//#include "iconv.h"

// Estrutura para armazenar apos quantos carcteres aparece a palavra
typedef struct aparicoes{
    int tamanho;
    struct aparicoes *prox;
}aparicoes;

// Estrutura para armazenar a palavra e a lista de apos quantos caracteres aparece a palavra
typedef struct tabela{
    char *chave;
    char *chaveSemAcento;
    aparicoes *Lista;
}tabela;

typedef struct no{
    char *palavra;
    struct no *prox;
}no;

void criaTabela(tabela *tabelaHash, int tamanhoTabela) {
    int i;
    for(i = 0; i < tamanhoTabela; i++) {
        tabelaHash[i].chave = NULL;
        tabelaHash[i].chaveSemAcento = NULL;
        tabelaHash[i].Lista = NULL;
    }
}

void excluirTabela(tabela *tabelaHash, int tamanhoTabela) {
    int i;

    for(i = 0; i < tamanhoTabela; i++) {
        if(tabelaHash[i].chave != NULL) {
            free(tabelaHash[i].chave);

            if(tabelaHash[i].chaveSemAcento != NULL) {
                free(tabelaHash[i].chaveSemAcento);
            }

            // Liberar a lista de aparicoes
            aparicoes *atual = tabelaHash[i].Lista;
            aparicoes *proximo;

            while(atual != NULL) {
                proximo = atual->prox;
                free(atual);
                atual = proximo;
            }
        }
    }
}

// Funcao para inicializar um no que portara apos quantos caracteres que a palavra aparece
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

// Funcaoo para encadear os nos que portarao apos quantos caracteres que a palavra aparece
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


char *ajeitaPalavra(char *palavra, int tamanhoPalavra){
    char vetorCaracteres[] = { 'À', 'Á', 'Â', 'Ã', 'A', 'à', 'á', 'â', 'ã', 'a', 'B', 'b', 'Ç', 'C', 'ç', 'c', 'D', 'd', 'È', 'É', 'Ê', 'E', 'è', 'é', 'ê', 
        'e', 'F', 'f', 'G', 'g', 'H', 'h', 'Í', 'I', 'í', 'i', 'J', 'j', 'k', 'k', 'L', 'l', 'M', 'm', 'N', 'n', 'Ó', 'Ô', 'Õ', 'O', 'ó', 'ô', 'õ', 'o', 'P',
        'p', 'Q', 'q', 'R', 'r', 'S', 's', 'T', 't', 'Ú', 'U', 'ú', 'u', 'V', 'v', 'W', 'w', 'X', 'x', 'Y', 'y', 'Z', 'z', '!', '@', '#', '$', '&', '*', '(', ')' };
    char *palavraNova = (char*)calloc(tamanhoPalavra + 1, sizeof(char));
    int j = 0;

    if(palavraNova == NULL){
        printf("Erro ao tentar alocar memoria para palavraNova");
        return NULL;
    }

    //Para percorrer o vetor de letras
    for(int i = 0; i < tamanhoPalavra; i++){
        for(int posicaoCaracter = 0; posicaoCaracter < 86; posicaoCaracter++){
            if(palavra[i] == vetorCaracteres[posicaoCaracter]){

                if(posicaoCaracter >= 0 && posicaoCaracter <= 10){
                    palavraNova[j] = 'a';
                }else if(posicaoCaracter >= 11 && posicaoCaracter <= 12){
                    palavraNova[j] = 'b';
                }else if(posicaoCaracter >= 13 && posicaoCaracter <= 16){
                    palavraNova[j] = 'c';
                }else if(posicaoCaracter >= 17 && posicaoCaracter <= 18){
                    palavraNova[j] = 'd';
                }else if(posicaoCaracter >= 19 && posicaoCaracter <= 26){
                    palavraNova[j] = 'e';
                }else if(posicaoCaracter >= 27 && posicaoCaracter <= 28){
                    palavraNova[j] = 'f';
                }else if(posicaoCaracter >= 29 && posicaoCaracter <= 30){
                    palavraNova[j] = 'g';
                }else if(posicaoCaracter >= 31 && posicaoCaracter <= 32){
                    palavraNova[j] = 'h';
                }else if(posicaoCaracter >= 33 && posicaoCaracter <= 36){
                    palavraNova[j] = 'i';
                }else if(posicaoCaracter >= 37 && posicaoCaracter <= 38){
                    palavraNova[j] = 'j';
                }else if(posicaoCaracter >= 39 && posicaoCaracter <= 40){
                    palavraNova[j] = 'k';
                }else if(posicaoCaracter >= 41 && posicaoCaracter <= 42){
                    palavraNova[j] = 'l';
                }else if(posicaoCaracter >= 43 && posicaoCaracter <= 44){
                    palavraNova[j] = 'm';
                }else if(posicaoCaracter >= 45 && posicaoCaracter <= 46){
                    palavraNova[j] = 'n';
                }else if(posicaoCaracter >= 47 && posicaoCaracter <= 54){
                    palavraNova[j] = 'o';
                }else if(posicaoCaracter >= 55 && posicaoCaracter <= 56){
                    palavraNova[j] = 'p';
                }else if(posicaoCaracter >= 57 && posicaoCaracter <= 58){
                    palavraNova[j] = 'q';
                }else if(posicaoCaracter >= 59 && posicaoCaracter <= 60){
                    palavraNova[j] = 'r';
                }else if(posicaoCaracter >= 61 && posicaoCaracter <= 62){
                    palavraNova[j] = 's';
                }else if(posicaoCaracter >= 63 && posicaoCaracter <= 64){
                    palavraNova[j] = 't';
                }else if(posicaoCaracter >= 65 && posicaoCaracter <= 68){
                    palavraNova[j] = 'u';
                }else if(posicaoCaracter >= 69 && posicaoCaracter <= 70){
                    palavraNova[j] = 'v';
                }else if(posicaoCaracter >= 71 && posicaoCaracter <= 72){
                    palavraNova[j] = 'w';
                }else if(posicaoCaracter >= 73 && posicaoCaracter <= 74){
                    palavraNova[j] = 'x';
                }else if(posicaoCaracter >= 75 && posicaoCaracter <= 76){
                    palavraNova[j] = 'y';
                }else if(posicaoCaracter >= 77 && posicaoCaracter <= 78){
                    palavraNova[j] = 'z';
                }else if(posicaoCaracter >= 79 && posicaoCaracter <= 86){
                    continue;
                }
                j++;
            }
        }
    }
    palavraNova[j] = '\0';
return palavraNova;
}

//Funcao de hash usando o metodo da divisao
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

// Funcao para buscar uma palavra na tabela hash, caso a palavra esteja na tabela, a funcao ira
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
        
        if(aux >= tamanhoTabela){
            printf("Tabela cheia, não foi possível inserir a palavra %s\n", palavra);
            return;
        }
        
        tabelaHash[aux].chave = (char*)calloc(tamanhoPalavra + 1, sizeof(char));
        strcpy(tabelaHash[aux].chave, palavra);

        tabelaHash[aux].chaveSemAcento = (char*)calloc(tamanhoPalavra + 1, sizeof(char));
        char *ajeita = ajeitaPalavra(palavra, tamanhoPalavra);
        strcpy(tabelaHash[aux].chaveSemAcento, ajeita);
        free(ajeita);  // Libera a memória alocada por ajeitaPalavra

        tabelaHash[aux].Lista = criaLista(caracteres);
    }
}

//Funcao responsavel pela impressao da tabela inteira, ata mesmo as casas com caracteres nulos
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

/*
//Funcao para aumentar a tabela hash
void aumentaTabela(tabela *tabelaHash, int *tamanhoTabela){
    tabela *novaTabela = (tabela*)calloc((*tamanhoTabela * 2), sizeof(tabela));
    int j = 0;
    if(novaTabela == NULL){
        printf("Erro ao tentar alocar");
        return;
    }

    for(int i = 0; i < *tamanhoTabela; i++){

        j = (novaTabela, tabelaHash[i].chave, *tamanhoTabela);

        if(j == 0){
            continue;
        }

        novaTabela[j].chave = tabelaHash[i].chave;
        novaTabela[j].Lista = tabelaHash[i].Lista;
    }

    *tamanhoTabela = *tamanhoTabela * 2;
    
    free(tabelaHash);
    
    tabelaHash = novaTabela;
}
*/
//Cria o no que guardar  a lista de palavras chave
no *criaNo(char *palavra, int tamanhoPalavra){
    no *novo = (no*)calloc(1, sizeof(no));

    if(novo == NULL){
        printf("Erro ao tentar alocar");
        return NULL;
    }
    novo->palavra = (char*)calloc(tamanhoPalavra + 1, sizeof(char));
    if(novo->palavra == NULL){
        printf("Erro ao tentar alocar memoria para nova palavra");
        free(novo);
        return NULL;
    }
    
    char *ajeita = ajeitaPalavra(palavra, tamanhoPalavra);
    strcpy(novo->palavra, ajeita);
    free(ajeita);  // Libera a memória alocada por ajeitaPalavra

    novo->prox = NULL;

return novo;
}

void insereOrdPalavra(no **listaPalavra, char *palavra, int tamanhoPalavra){
    no *novo = criaNo(palavra, tamanhoPalavra);
    no *aux = *listaPalavra;
    no *anterior = NULL;
    int resultado;

    if(novo == NULL){
        return;  // Não foi possível criar o nó
    }

    if(*listaPalavra == NULL){
        *listaPalavra = novo;
        return;  // Removido o exit(1) que encerrava o programa
    }

    while(aux != NULL){
        resultado = strcmp(aux->palavra, novo->palavra);

        if(resultado == 0){
            // Palavra já existe na lista, liberar o novo nó e retornar
            free(novo->palavra);
            free(novo);
            return;  // Removido o exit(1) que encerrava o programa
        }
        else if(resultado > 0){
            // Inserir antes do nó atual
            if(anterior == NULL){
                // Inserir no início
                novo->prox = *listaPalavra;
                *listaPalavra = novo;
            }
            else{
                // Inserir no meio
                novo->prox = aux;
                anterior->prox = novo;
            }
            return;
        }
        anterior = aux;
        aux = aux->prox;
    }
    // Inserir no final
    anterior->prox = novo;
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
    
    *listaPalavras = NULL;  // Defina o ponteiro da lista como NULL após liberá-la
}

int main(){
    tabela tabelaHash[81]; // 9x9
    no *listaPalavra = NULL;
    aparicoes *listaAparicoes = NULL;

    char arqPalavrasChave[1000], arqTexto[1000];
    int caractereAtual, tamanhoPalavraFormada = 100, indice = 0;
    int posicaoTabela = 0, tamanhoTabela = 81, caracteres = 1, j = 0, possivelFinal = 0;

    char *palavraFormada = (char*)calloc(tamanhoPalavraFormada, sizeof(char));
    if(palavraFormada == NULL){
        printf("Erro ao alocar memória para palavraFormada");
        return 1;
    }

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
        //Enquanto o caractere atual for diferente de espaco e pontuacao a palavra a formada
        if(caractereAtual != ' ' && caractereAtual != '.' && caractereAtual != ',' && caractereAtual != ';' && caractereAtual != ':'){
            //Caso passe o numero maximo de caracteres
            if(indice >= tamanhoPalavraFormada - 1){
                tamanhoPalavraFormada *= 2;
                char *temp = (char*)realloc(palavraFormada, tamanhoPalavraFormada * sizeof(char));
                if(temp == NULL){
                    printf("Erro ao realocar memória para palavraFormada");
                    free(palavraFormada);
                    fclose(arqP);
                    return 1;
                }
                palavraFormada = temp;
            }

            if(isalnum(caractereAtual) == 0 && indice == 0){
                continue;
            }
            
            palavraFormada[indice] = caractereAtual;
            indice++;

        // Indice > 0 significa que a palavra foi formada e chegou em um espaco
        }else if(indice > 0){
            palavraFormada[indice] = '\0';
            insereOrdPalavra(&listaPalavra, palavraFormada, indice);
            inserirPalavra(tabelaHash, palavraFormada, tamanhoTabela, caracteres, indice);
            indice = 0;
        }
        caracteres++;
    }
    
    //Verifica se chegou ao final do arquivo e esqueceu uma palavra
    if(indice > 0) {
        palavraFormada[indice] = '\0';
        insereOrdPalavra(&listaPalavra, palavraFormada, indice);
        inserirPalavra(tabelaHash, palavraFormada, tamanhoTabela, caracteres, indice);
    }

    fclose(arqP);
    free(palavraFormada);
    
    // Reinicializa as variáveis para a leitura do próximo arquivo
    indice = 0; 
    caracteres = 1;
    j = 0;
    possivelFinal = 0;
    tamanhoPalavraFormada = 100;
    
    // Realoca palavraFormada após ser liberada
    palavraFormada = (char*)calloc(tamanhoPalavraFormada, sizeof(char));
    if(palavraFormada == NULL){
        printf("Erro ao alocar memória para palavraFormada");
        liberaListaPalavra(&listaPalavra);
        excluirTabela(tabelaHash, tamanhoTabela);
        return 1;
    }

    //Usando o arquivo de texto   
    printf("Digite o nome do arquivo a ser lido: ");
    scanf("%s", arqTexto);  // Removido o & desnecessário

    FILE *arqT;
    arqT = fopen(arqTexto, "r");

    if(arqT == NULL){
        printf("Erro ao tentar abrir o arquivo");
        free(palavraFormada);
        liberaListaPalavra(&listaPalavra);
        excluirTabela(tabelaHash, tamanhoTabela);
        return 0;
    }

    while((caractereAtual = fgetc(arqT)) != EOF){
        if(caractereAtual != ' ' && caractereAtual != '.' && caractereAtual != ',' && caractereAtual != ';' && caractereAtual != ':'){
            if(indice >= tamanhoPalavraFormada - 1){
                tamanhoPalavraFormada *= 2;
                char *temp = (char*)realloc(palavraFormada, tamanhoPalavraFormada * sizeof(char));
                if(temp == NULL){
                    printf("Erro ao realocar memória para palavraFormada");
                    free(palavraFormada);
                    fclose(arqT);
                    liberaListaPalavra(&listaPalavra);
                    excluirTabela(tabelaHash, tamanhoTabela);
                    return 1;
                }
                palavraFormada = temp;
            }

            palavraFormada[indice] = caractereAtual;
            indice++;
        }else if(indice > 0){
            palavraFormada[indice] = '\0';
            buscarPalavra(tabelaHash, palavraFormada, tamanhoTabela, caracteres, indice);
            indice = 0;
        }
        caracteres++;
    }

    if(indice > 0){
        palavraFormada[indice] = '\0';
        buscarPalavra(tabelaHash, palavraFormada, tamanhoTabela, caracteres, indice);
    }

    fclose(arqT);
    free(palavraFormada);

    printf("\nLista de palavras-chave:\n");
    exibirLista(listaPalavra);
    
    printf("\nTabela Hash:\n");
    imprimirTabela(tabelaHash, tamanhoTabela, 10);

    liberaListaPalavra(&listaPalavra); 
    excluirTabela(tabelaHash, tamanhoTabela);

return 0;
}