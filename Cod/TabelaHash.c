/*
Grupo 4
Integrantes: Emerson Boone, Thaelen de Morais, Luiz Felipe Pereira
*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<locale.h>
#include<iconv.h>

typedef struct aparicoes{
    int tamanho;
    struct aparicoes *prox;
}aparicoes;

typedef struct tabela{
    char *chave;
    char *chaveSemAcento;
    aparicoes *Lista;
}tabela;

typedef struct no{
    char *palavraNo;
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
    int i = 0;

    aparicoes *atual = tabelaHash[i].Lista;
    aparicoes *proximo;

    for(i; i < tamanhoTabela; i++) {
        if(tabelaHash[i].chave != NULL) {
            free(tabelaHash[i].chave);
            if(tabelaHash[i].chaveSemAcento != NULL) {
                free(tabelaHash[i].chaveSemAcento);
            }

            while(atual != NULL) {
                proximo = atual->prox;
                free(atual);
                atual = proximo;
            }
        }
    }
}

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

char *ajeitaPalavra(char *novaPalavra, int tamanhoPalavra) {
    int j = 0;  
    
    for (int i = 0; i < tamanhoPalavra; i++) {
        unsigned char c = (unsigned char)novaPalavra[i];
        
        if (c >= 'A' && c <= 'Z') {
            novaPalavra[j] = c + 32;
            j++;
            continue;
        }
        if ((c >= 'a' && c <= 'z') || (c >= '0' && c <= '9')) {
            novaPalavra[j] = c;
            j++;
            continue;
        }
        switch (c) {
            case 193: case 192: case 194: case 195: case 196: case 197:
                novaPalavra[j] = 'a'; j++; break;
            case 201: case 200: case 202: case 203:
                novaPalavra[j] = 'e'; j++; break;
            case 205: case 204: case 206: case 207:
                novaPalavra[j] = 'i'; j++; break;
            case 211: case 210: case 212: case 213: case 214: case 216:
                novaPalavra[j] = 'o'; j++; break;
            case 218: case 217: case 219: case 220:
                novaPalavra[j] = 'u'; j++; break;
            case 199:
                novaPalavra[j] = 'c'; j++; break;
            case 209:
                novaPalavra[j] = 'n'; j++; break;
            case 221:
                novaPalavra[j] = 'y'; j++; break;
                
            case 225: case 224: case 226: case 227: case 228: case 229:
                novaPalavra[j] = 'a'; j++; break;
            case 233: case 232: case 234: case 235:
                novaPalavra[j] = 'e'; j++; break;
            case 237: case 236: case 238: case 239:
                novaPalavra[j] = 'i'; j++; break;
            case 243: case 242: case 244: case 245: case 246: case 248:
                novaPalavra[j] = 'o'; j++; break;
            case 250: case 249: case 251: case 252:
                novaPalavra[j] = 'u'; j++; break;
            case 231:
                novaPalavra[j] = 'c'; j++; break;
            case 241:
                novaPalavra[j] = 'n'; j++; break;
            case 253: case 255:
                novaPalavra[j] = 'y'; j++; break;
  
            default:
                break;
        }
    }
    novaPalavra[j] = '\0';
    
return novaPalavra;
}

int funcaoHash(int tamanhoPalavra, int tamanhoTabela){
    int posicao = (tamanhoPalavra * 2), k = 1;

    if(tamanhoPalavra == 0){
        return 0;
    }
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
    char *ajeita;

    if(resultadoBusca == 0){
        
        while(tabelaHash[aux].chave != NULL && aux < tamanhoTabela){
            aux = aux + k;
            k++;
        }
        
        tabelaHash[aux].chave = (char*)calloc(tamanhoPalavra + 1, sizeof(char));
        strcpy(tabelaHash[aux].chave, palavra); 

        tabelaHash[aux].chaveSemAcento = (char*)calloc(tamanhoPalavra + 1, sizeof(char));
        ajeita = ajeitaPalavra(palavra, tamanhoPalavra);
        strcpy(tabelaHash[aux].chaveSemAcento, ajeita);

        tabelaHash[aux].Lista = NULL;
    }
}

void imprimirTabela(tabela *tabelaHash, int tamanhoTabela){
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

no *criaNo(char *palavra, int tamanhoPalavra){
    no *novo = (no*)calloc(1, sizeof(no));
    char *ajeita;

    if(novo == NULL){
        printf("Erro ao tentar alocar");
        return NULL;
    }
    novo->palavraNo = (char*)calloc(tamanhoPalavra + 1, sizeof(char));
    ajeita = ajeitaPalavra(palavra, tamanhoPalavra);

    strcpy(novo->palavraNo, ajeita);

    novo->prox = NULL;

return novo;
}

void insereOrdPalavra(no **listaPalavra, char *palavra, int tamanhoPalavra) {
    no *novo = criaNo(palavra, tamanhoPalavra);
    no *aux = *listaPalavra;
    no *anterior = NULL;
    int resultado;

    if (*listaPalavra == NULL) {
        *listaPalavra = novo;
        return;
    }

    while (aux != NULL) {
        resultado = strcmp(aux->palavraNo, novo->palavraNo);

        if (resultado == 0) {
            free(novo->palavraNo);
            free(novo);
            return;
        }
        if (resultado > 0) {
            break;
        }
        anterior = aux;
        aux = aux->prox;
    }

    if (anterior == NULL) {
        novo->prox = *listaPalavra;
        *listaPalavra = novo;
    } else {
        novo->prox = anterior->prox;
        anterior->prox = novo;
    }
}

void liberaListaPalavra(no **listaPalavras) {
    no *atual = *listaPalavras;
    no *proximo;

    while (atual != NULL) {
        proximo = atual->prox;

        free(atual->palavraNo);
        free(atual);
        
        atual = proximo;
    }
}

void imprimeOrdTabela(tabela *tabelaHash, no *listaPalavras, int tamanhoTabela) {
    aparicoes *aux = NULL;
    no *aux2 = listaPalavras;
    int posicao, tentativas;

    while (aux2 != NULL) {
        posicao = funcaoHash(strlen(aux2->palavraNo), tamanhoTabela);
        tentativas = 0;

        while (tentativas < tamanhoTabela) {
            if (tabelaHash[posicao].chave != NULL && strcmp(tabelaHash[posicao].chaveSemAcento, aux2->palavraNo) == 0) {
                printf("%s", tabelaHash[posicao].chave);
                aux = tabelaHash[posicao].Lista;

                while (aux != NULL) {
                    printf(" %d", aux->tamanho);
                    aux = aux->prox;
                }
                printf("\n");
                break;
            } else {
                posicao = (posicao + 1) % tamanhoTabela;
                tentativas++;
            }
        }
        aux2 = aux2->prox;
    }
    printf("\n");
}

void aumentaTabela(tabela **tabelaHash, no *listaPalavras, int *tamanhoTabela) {
    int novoTamanho = *tamanhoTabela * 2, posicao;
    tabela *novaTabela = (tabela*)calloc(novoTamanho, sizeof(tabela));
 
    if (*tabelaHash == NULL || tamanhoTabela == NULL) {
        printf("Erro: ponteiro nulo passado para aumentaTabela.\n");
        return;
    }

    if (novaTabela == NULL) {
        printf("Erro ao tentar alocar memória para a nova tabela.\n");
        return;
    }

    criaTabela(novaTabela, novoTamanho);

    for (int i = 0; i < *tamanhoTabela; i++) {
        if ((*tabelaHash)[i].chave != NULL) {
            posicao = funcaoHash(strlen((*tabelaHash)[i].chave), novoTamanho);
            
            novaTabela[posicao].chave = strdup((*tabelaHash)[i].chave);
            novaTabela[posicao].chaveSemAcento = strdup((*tabelaHash)[i].chaveSemAcento);

            aparicoes *listaVelha = (*tabelaHash)[i].Lista;
            aparicoes *aux = listaVelha;
            aparicoes *novaLista = NULL;

            while(aux != NULL){
                aparicoes *novo = (aparicoes*)calloc(1, sizeof(aparicoes));

                novo->tamanho = aux->tamanho;
                novo->prox = NULL;

                if(novaLista == NULL){
                    novaLista = novo;
                }
                aux = aux->prox;
            }
            novaTabela[posicao].Lista = novaLista;
        }
    }
    for (int i = 0; i < *tamanhoTabela; i++) {
        if ((*tabelaHash)[i].chave != NULL) {
            free((*tabelaHash)[i].chave);
            free((*tabelaHash)[i].chaveSemAcento);

            aparicoes *aux = (*tabelaHash)[i].Lista;

            while (aux != NULL) {
                aparicoes *prox = aux->prox;
                free(aux);
                aux = prox;
            }
        }
    }
    free(*tabelaHash);

    *tabelaHash = novaTabela;
    *tamanhoTabela = novoTamanho;
}

void recebeArqPalavrasChave(tabela **tabelaHash, no **listaPalavra, char *arqPalavrasChave, char *palavraFormada, int caractereAtual, int indice, 
    int tamanhoPalavraFormada, int caracteres, int tamanhoTabela) {
    
    int posicaoInicio = 0;
    char *temp;

    printf("Digite o nome do arquivo de palavras chaves: ");
    scanf("%s", arqPalavrasChave);

    FILE *arqP = fopen(arqPalavrasChave, "r");
    if (arqP == NULL) {
        printf("Erro ao tentar abrir o arquivo.\n");
        return;
    }

    while ((caractereAtual = fgetc(arqP)) != EOF) {
        if (caractereAtual != ' ' && caractereAtual != '!' && caractereAtual != '.' && caractereAtual != ',' && caractereAtual != ';' 
            && caractereAtual != ':' && caractereAtual != '(' && caractereAtual != ')') {

            if (indice >= tamanhoPalavraFormada - 1) {

                tamanhoPalavraFormada *= 2;
                temp = (char *)realloc(palavraFormada, tamanhoPalavraFormada * sizeof(char));
                
                if (temp == NULL) {
                    printf("Erro ao realocar memória.\n");
                    fclose(arqP);
                    return;
                }
                palavraFormada = temp;
            }

            palavraFormada[indice] = caractereAtual;
            indice++;

        } else if (indice > 0) {

            palavraFormada[indice] = '\0';
            posicaoInicio = caracteres - indice;
            
            inserirPalavra((*tabelaHash), palavraFormada, tamanhoTabela, posicaoInicio, indice);
            insereOrdPalavra(listaPalavra, palavraFormada, indice);

            indice = 0;
        }
        caracteres++;
    }

    if (indice > 0) {
        palavraFormada[indice] = '\0';
        posicaoInicio = caracteres - indice;
        
        inserirPalavra((*tabelaHash), palavraFormada, tamanhoTabela, posicaoInicio, indice);
        insereOrdPalavra(listaPalavra, palavraFormada, indice);
    }

    fclose(arqP);
}

void recebeArqTexto(tabela **tabelaHash, char *arqTexto, char *palavraFormada, int caractereAtual, int indice, int tamanhoTabela, 
    int caracteres, int tamanhoPalavraFormada) {
    
    int posicaoInicio = 0;
    char *temp;

    printf("Digite o nome do arquivo a ser lido: ");
    scanf("%s", arqTexto);

    FILE *arqT = fopen(arqTexto, "r");
    if (arqT == NULL) {
        printf("Erro ao tentar abrir o arquivo.\n");
        return;
    }

    while ((caractereAtual = fgetc(arqT)) != EOF) {
        if (caractereAtual != ' ' && caractereAtual != '!' && caractereAtual != '.' && caractereAtual != ',' && caractereAtual != ';' 
            && caractereAtual != ':' && caractereAtual != '(' && caractereAtual != ')'){
            if (indice >= tamanhoPalavraFormada - 1) {
                
                tamanhoPalavraFormada *= 2;
                char *temp = (char *)realloc(palavraFormada, tamanhoPalavraFormada * sizeof(char));
                
                if (temp == NULL) {
                    printf("Erro ao realocar memória.\n");
                    fclose(arqT);
                    return;
                }
                palavraFormada = temp;
            }

            palavraFormada[indice] = caractereAtual;
            indice++;
        } else if (indice > 0) {
            
            palavraFormada[indice] = '\0';
            posicaoInicio = caracteres - indice;
           
            buscarPalavra((*tabelaHash), palavraFormada, tamanhoTabela, posicaoInicio, indice);
            
            indice = 0;
        }
        caracteres++;
    }

    if (indice > 0) {
        palavraFormada[indice] = '\0';
        posicaoInicio = caracteres - indice;
        
        buscarPalavra((*tabelaHash), palavraFormada, tamanhoTabela, posicaoInicio, indice);
    }

    fclose(arqT);
}

void converterAqr(const char *arquivo) {
    FILE *arquivoPtr = fopen(arquivo, "r+");

    if (arquivoPtr == NULL) {
        perror("Erro ao abrir arquivo");
        exit(EXIT_FAILURE);
    }

    char bufferEntrada[1024];
    char bufferSaida[1024];
    size_t tamanhoEntrada, tamanhoSaida;

    char *entradaAux;
    char *saidaAux;

    long posicaoInicial = 0;

    iconv_t conversor = iconv_open("ISO-8859-1", "UTF-8");

    if (conversor == (iconv_t)-1) {
        perror("Erro ao abrir conversor de encoding");
        fclose(arquivoPtr);
        exit(EXIT_FAILURE);
    }

    while (fgets(bufferEntrada, sizeof(bufferEntrada), arquivoPtr)) {
        
        entradaAux = bufferEntrada;
        saidaAux = bufferSaida;
        
        tamanhoEntrada = strlen(bufferEntrada);
        tamanhoSaida = sizeof(bufferSaida);

        if (iconv(conversor, &entradaAux, &tamanhoEntrada, &saidaAux, &tamanhoSaida) == (size_t)-1) {
            
            perror("Erro ao converter encoding");
            iconv_close(conversor);
            
            fclose(arquivoPtr);
            exit(EXIT_FAILURE);
        }

        fseek(arquivoPtr, posicaoInicial, SEEK_SET);

        fwrite(bufferSaida, 1, sizeof(bufferSaida) - tamanhoSaida, arquivoPtr);

        posicaoInicial = ftell(arquivoPtr);
    }

    iconv_close(conversor);
    fclose(arquivoPtr);
}

int main(){
    tabela *tabelaHash;
    no *listaPalavra = NULL;
    aparicoes *listaAparicoes = NULL;

    char arqPalavrasChave[1000], arqTexto[1000];
    int caractereAtual, tamanhoPalavraFormada = 100, indice = 0;
    int posicaoTabela = 0, tamanhoTabela = 81, caracteres = 1, j = 0, possivelFinal = 0;

    tabelaHash = (tabela*)calloc(tamanhoTabela, sizeof(tabela));
    char *palavraFormada = (char*)calloc(tamanhoPalavraFormada, sizeof(char));

    criaTabela(tabelaHash, tamanhoTabela);  

    setlocale(LC_ALL, "Portuguese_Brazil");

    recebeArqPalavrasChave(&tabelaHash, &listaPalavra, arqPalavrasChave, palavraFormada, caractereAtual, indice, 
    tamanhoPalavraFormada, caracteres, tamanhoTabela);

    recebeArqTexto(&tabelaHash, arqTexto, palavraFormada, caractereAtual, indice, tamanhoTabela, 
    caracteres, tamanhoPalavraFormada);

    aumentaTabela(&tabelaHash, listaPalavra, &tamanhoTabela);   
    
    // exibirLista(listaPalavra);
    //imprimirTabela(tabelaHash, tamanhoTabela;

    imprimeOrdTabela(tabelaHash, listaPalavra, tamanhoTabela);

    liberaListaPalavra(&listaPalavra); 
    excluirTabela(tabelaHash, tamanhoTabela);

return 0;
}