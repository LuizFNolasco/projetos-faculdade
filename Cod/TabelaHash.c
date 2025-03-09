/*
Grupo 4
Integrantes: Emerson Boone, Thaelen de Morais, Luiz Felipe Pereira
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <wchar.h>

typedef struct aparicoes {
    int tamanho;
    struct aparicoes *prox;
} aparicoes;

typedef struct tabela {
    char *chave;
    char *chaveSemAcento;
    aparicoes *Lista;
} tabela;

typedef struct no {
    char *palavraNor;
    char *palavraSem;
    struct no *prox;
} no;

void criaTabela(tabela *tabelaHash, int tamanhoTabela) {
    for (int i = 0; i < tamanhoTabela; i++) {
        tabelaHash[i].chave = NULL;
        tabelaHash[i].chaveSemAcento = NULL;
        tabelaHash[i].Lista = NULL;
    }
}

void excluirTabela(tabela *tabelaHash, int tamanhoTabela) {
    for (int i = 0; i < tamanhoTabela; i++) {
        if (tabelaHash[i].chave != NULL) {
            free(tabelaHash[i].chave);
            if (tabelaHash[i].chaveSemAcento != NULL) {
                free(tabelaHash[i].chaveSemAcento);
            }

            aparicoes *atual = tabelaHash[i].Lista;
            while (atual != NULL) {
                aparicoes *proximo = atual->prox;
                free(atual);
                atual = proximo;
            }
        }
    }
}

aparicoes *criaLista(int caracteres) {
    aparicoes *novo = (aparicoes *)calloc(1, sizeof(aparicoes));
    if (novo == NULL) {
        printf("Erro ao tentar alocar");
        return NULL;
    }
    novo->tamanho = caracteres;
    novo->prox = NULL;
    return novo;
}

aparicoes *insereFimAparicoes(aparicoes *listaAparicoes, int caracteres) {
    aparicoes *novo = criaLista(caracteres);
    if (listaAparicoes == NULL) {
        return novo;
    }

    aparicoes *aux = listaAparicoes;
    while (aux->prox != NULL) {
        aux = aux->prox;
    }
    aux->prox = novo;
return listaAparicoes;
}

char *ajeitaPalavra(char *palavra, int tamanhoPalavra) {
    int j = 0;
    char *novaPalavra = (char*)calloc(tamanhoPalavra + 1, sizeof(char));
    if (novaPalavra == NULL) {
        printf("Erro ao alocar memória para novaPalavra\n");
        return NULL;
    }

    for (int i = 0; i < tamanhoPalavra; i++) {
        unsigned char c = (unsigned char)palavra[i];

        if (c >= 'a' && c <= 'z'){
            novaPalavra[j] = c;
            j++;
            continue;
        }
        switch (c) {
            case 192: case 193: case 194: case 195: case 196: case 197:
            case 224: case 225: case 226: case 227: case 228: case 229:
                novaPalavra[j] = 'a'; j++; break;
            case 200: case 201: case 202: case 203:
            case 232: case 233: case 234: case 235:
                novaPalavra[j] = 'e'; j++; break;
            case 204: case 205: case 206: case 207:
            case 236: case 237: case 238: case 239:
                novaPalavra[j] = 'i'; j++; break;
            case 210: case 211: case 212: case 213: case 214: case 216:
            case 242: case 243: case 244: case 245: case 246: case 248:
                novaPalavra[j] = 'o'; j++; break;
            case 217: case 218: case 219: case 220:
            case 249: case 250: case 251: case 252:
                novaPalavra[j] = 'u'; j++; break;
            case 199: case 231:
                novaPalavra[j] = 'c'; j++; break;
            case 209: case 241:
                novaPalavra[j] = 'n'; j++; break;
            case 221: case 253:
                novaPalavra[j] = 'y'; j++; break;
            default:
                break;
        }
    }
    novaPalavra[j] = '\0';
    return novaPalavra;
}

int funcaoHash(char *palavra, int tamanhoPalavra, int tamanhoTabela) {
    unsigned char l = (unsigned char)palavra[0];

    int j = (int)l;

    int posicao = tamanhoPalavra * j;

    while(posicao > tamanhoTabela) {
        posicao = posicao / 5;
    }

    return posicao;
}

int buscarPalavra(tabela *tabelaHash, char *palavra, int tamanhoTabela, int caracteres, int tamanhoPalavra) {
    int posicao = funcaoHash(palavra, tamanhoPalavra, tamanhoTabela);
    int tentativas = 0;
    
    while (tentativas < tamanhoTabela) {
        if (tabelaHash[posicao].chave != NULL && strcmp(tabelaHash[posicao].chave, palavra) == 0) {
            tabelaHash[posicao].Lista = insereFimAparicoes(tabelaHash[posicao].Lista, caracteres);
            return 1;
        }
        posicao = (posicao + 1) % tamanhoTabela;
        tentativas++;
    }
    return 0;
}

void inserirPalavra(tabela *tabelaHash, char *palavra, int tamanhoTabela, int caracteres, int tamanhoPalavra) {
    int resultadoBusca = buscarPalavra(tabelaHash, palavra, tamanhoTabela, caracteres, tamanhoPalavra);
    
    if (resultadoBusca == 0) {
        int posicao = funcaoHash(palavra, tamanhoPalavra, tamanhoTabela);
        int tentativas = 0;

        while (tabelaHash[posicao].chave != NULL && strcmp(tabelaHash[posicao].chave, palavra) != 0 && tentativas < tamanhoTabela) {
            posicao = (posicao + 1) % tamanhoTabela;
            tentativas++;
        }
        
        char *ajeita = (char*)calloc(tamanhoPalavra + 1, sizeof(char));
        if (ajeita == NULL) {
            printf("Erro ao alocar memória para chave\n");
            free(ajeita);
            return;
        }
        ajeita = ajeitaPalavra(palavra, tamanhoPalavra);


        tabelaHash[posicao].chave = (char *)calloc(tamanhoPalavra + 1, sizeof(char));
        if (tabelaHash[posicao].chave == NULL) {
            printf("Erro ao alocar memória para chave\n");
            free(ajeita);
            return;
        }
        strcpy(tabelaHash[posicao].chave, palavra);

        tabelaHash[posicao].chaveSemAcento = (char *)calloc(tamanhoPalavra + 1, sizeof(char));
        if (tabelaHash[posicao].chaveSemAcento == NULL) {
            printf("Erro ao alocar memória para chaveSemAcento\n");
            free(ajeita);
            free(tabelaHash[posicao].chave);
            tabelaHash[posicao].chave = NULL;
            return;
        }
        strcpy(tabelaHash[posicao].chaveSemAcento, ajeita);

        tabelaHash[posicao].Lista = NULL;

        free(ajeita);
    }
}

void imprimirTabela(tabela *tabelaHash, int tamanhoTabela) {
    printf("--------------------------------\n");
    for (int i = 0; i < tamanhoTabela; i++) {
        printf("| %-7d | ", i);
        if (tabelaHash[i].chave != NULL) {
            printf("%-18s |\n", tabelaHash[i].chave);
        } else {
            printf("%-18s |\n", "");
        }
    }
    printf("--------------------------------\n");
}

no *criaNo(char *palavra, int tamanhoPalavra) {
    no *novo = (no *)calloc(1, sizeof(no));
    
    if (novo == NULL) {
        printf("Erro ao tentar alocar");
        return NULL;
    }
    
    novo->palavraNor = strdup(palavra);
    if (novo->palavraNor == NULL) {
        printf("Erro ao alocar memória para palavraNor\n");
        free(novo);
        return NULL;
    }

    char *ajeita = (char *)calloc(tamanhoPalavra + 1, sizeof(char));
    if (ajeita == NULL) {
        free(novo->palavraNor);
        free(novo);
        return NULL;
    }
    ajeita = ajeitaPalavra(palavra, tamanhoPalavra);
    
    novo->palavraSem = strdup(ajeita);
    if (novo->palavraSem == NULL) {
        printf("Erro ao alocar memória para palavraSem\n");
        free(novo->palavraNor);
        free(novo);
        free(ajeita);
        return NULL;
    }
    
    novo->prox = NULL;
    free(ajeita);

    return novo;
}

void insereOrdPalavra(no **listaPalavra, char *palavra, int tamanhoPalavra) {
    no *novo = criaNo(palavra, tamanhoPalavra);
    if (novo == NULL) {
        return;
    }
    
    no *aux = *listaPalavra;
    no *anterior = NULL;

    if (*listaPalavra == NULL) {
        *listaPalavra = novo;
        return;
    }

    while (aux != NULL && strcmp(aux->palavraSem, novo->palavraSem) < 0) {
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
        free(atual->palavraNor);
        free(atual->palavraSem);
        free(atual);
        atual = proximo;
    }
    
}

void imprimeOrdTabela(tabela *tabelaHash, no *listaPalavras, int tamanhoTabela) {
    aparicoes *aux = NULL;
    no *aux2 = listaPalavras;
    int posicao, tentativas;

    while (aux2 != NULL) {
        posicao = funcaoHash(aux2->palavraNor, strlen(aux2->palavraNor), tamanhoTabela);
        tentativas = 0;

        while (tentativas < tamanhoTabela) {
            if (tabelaHash[posicao].chave != NULL && strcmp(tabelaHash[posicao].chaveSemAcento, aux2->palavraSem) == 0) {
                
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
    int novoTamanho = *tamanhoTabela * 2;
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
            int tamanhoPalavra = strlen((*tabelaHash)[i].chave);
            int posicao = funcaoHash((*tabelaHash)[i].chave, tamanhoPalavra, novoTamanho);
            int tentativas = 0;
            
            while (novaTabela[posicao].chave != NULL && tentativas < novoTamanho) {
                posicao = (posicao + 1) % novoTamanho;
                tentativas++;
            }
            
            novaTabela[posicao].chave = strdup((*tabelaHash)[i].chave);
            novaTabela[posicao].chaveSemAcento = strdup((*tabelaHash)[i].chaveSemAcento);

            if (novaTabela[posicao].chave == NULL || novaTabela[posicao].chaveSemAcento == NULL) {
                printf("Erro ao alocar memória para as chaves.\n");
                excluirTabela(novaTabela, novoTamanho);
                free(novaTabela);
                return;
            }

            aparicoes *listaVelha = (*tabelaHash)[i].Lista;
            aparicoes *novaLista = NULL;

            while (listaVelha != NULL) {
                novaLista = insereFimAparicoes(novaLista, listaVelha->tamanho);
                listaVelha = listaVelha->prox;
            }

            novaTabela[posicao].Lista = novaLista;
        }
    }

    excluirTabela(*tabelaHash, *tamanhoTabela);
    free(*tabelaHash);

    *tabelaHash = novaTabela;
    *tamanhoTabela = novoTamanho;

}

void recebeArqPalavrasChave(tabela **tabelaHash, no **listaPalavra, char *arqPalavrasChave, char *palavraFormada, int caractereAtual, int indice,
    int tamanhoPalavraFormada, int caracteres, int *tamanhoTabela) {
    
    int posicaoInicio = 0, temLetra = 0;
    int inseridos = 0;
    char *temp;

    printf("Digite o nome do arquivo de palavras chaves: ");
    scanf("%s", arqPalavrasChave);

    FILE *arqP = fopen(arqPalavrasChave, "r,ccs=UTF-8");
    if (arqP == NULL) {
        printf("Erro ao tentar abrir o arquivo.\n");
        return;
    }

    while ((caractereAtual = fgetwc(arqP)) != WEOF) {
        wchar_t c = (wchar_t)caractereAtual;

        if (c == 10 || c == 32 || c == 33 || c == 40 || c == 41 || c == 44 || c == 46 || c == 58 || 
        c == 59 || c == 63 || c == 91 || c == 93 || c == 123 || c == 125) {
            
            if (indice > 0 && temLetra > 0) {
                palavraFormada[indice] = '\0';
                posicaoInicio = caracteres - indice;
                
                inserirPalavra((*tabelaHash), palavraFormada, (*tamanhoTabela), posicaoInicio, indice);
                insereOrdPalavra(listaPalavra, palavraFormada, indice);
                inseridos++;
                
                if(inseridos >= (*tamanhoTabela) * 0.9) {
                    aumentaTabela(tabelaHash, (*listaPalavra), tamanhoTabela);
                }
            }
            
            indice = 0;
            temLetra = 0;
            caracteres++;
            
        } else {
            if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= 192 && c <= 255)) {
                if(c >= 'A' && c <= 'Z'){
                    c += 32;
                }
                temLetra = 1;
            }

            if (indice >= tamanhoPalavraFormada - 1) {
                temp = (char *)realloc(palavraFormada, (tamanhoPalavraFormada * 2) * sizeof(char));
                if (temp == NULL) {
                    printf("Erro ao realocar memória.\n");
                    fclose(arqP);
                    return;
                }
                palavraFormada = temp;
                tamanhoPalavraFormada *= 2;
            }

            palavraFormada[indice] = (char)c;
            indice++;
            caracteres++;
        }
    }

    if (indice > 0 && temLetra > 0) {
        palavraFormada[indice] = '\0';
        posicaoInicio = caracteres - indice;
        
        inserirPalavra((*tabelaHash), palavraFormada, (*tamanhoTabela), posicaoInicio, indice);
        insereOrdPalavra(listaPalavra, palavraFormada, indice);
    }

    fclose(arqP);
}

void recebeArqTexto(tabela **tabelaHash, char *arqTexto, char *palavraFormada, int caractereAtual, int indice, int tamanhoTabela, 
    int caracteres, int tamanhoPalavraFormada) {
    
    int posicaoInicio = 0;
    char *temp;
    int temLetra = 0;

    printf("Digite o nome do arquivo a ser lido: ");
    scanf("%s", arqTexto);

    FILE *arqT = fopen(arqTexto, "r, ccs=UTF-8");
    if (arqT == NULL) {
        printf("Erro ao tentar abrir o arquivo.\n");
        return;
    }

    while ((caractereAtual = fgetwc(arqT)) != WEOF) {
        wchar_t c = (wchar_t)caractereAtual;

        if (c == 10 || c == 32 || c == 33 || c == 40 || c == 41 || c == 44 || c == 46 || c == 58 || 
            c == 59 || c == 63 || c == 91 || c == 93 || c == 123 || c == 125) {
            if (indice > 0 && temLetra > 0) {
                palavraFormada[indice] = '\0';
                posicaoInicio = caracteres - indice;
                buscarPalavra((*tabelaHash), palavraFormada, tamanhoTabela, posicaoInicio, indice);
            }
            
            indice = 0;
            temLetra = 0;
            caracteres++;
        } else {
            if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= 192 && c <= 255)) {
                if(c >= 'A' && c <= 'Z'){
                    c += 32;
                }
                temLetra = 1;
            }

            if (indice >= tamanhoPalavraFormada - 1) {
                temp = (char *)realloc(palavraFormada, (tamanhoPalavraFormada * 2) * sizeof(char));
                if (temp == NULL) {
                    printf("Erro ao realocar memória.\n");
                    fclose(arqT);
                    return;
                }
                palavraFormada = temp;
                tamanhoPalavraFormada *= 2;
            }

            palavraFormada[indice] = (char)c;
            indice++;
            caracteres++;
        }
    }

    if (indice > 0 && temLetra > 0) {
        palavraFormada[indice] = '\0';
        posicaoInicio = caracteres - indice;
        buscarPalavra((*tabelaHash), palavraFormada, tamanhoTabela, posicaoInicio, indice);
    }

    fclose(arqT);
}

int main() {
    tabela *tabelaHash;
    no *listaPalavra = NULL;

    char arqPalavrasChave[1000], arqTexto[1000];
    int caractereAtual = 0, tamanhoPalavraFormada = 100, indice = 0;
    int tamanhoTabela = 81, caracteres = 1;

    tabelaHash = (tabela *)calloc(tamanhoTabela, sizeof(tabela));
    if (tabelaHash == NULL) {
        printf("Erro ao alocar memória para tabelaHash\n");
        return 1;
    }
    
    char *palavraFormada = (char *)calloc(tamanhoPalavraFormada, sizeof(char));
    if (palavraFormada == NULL) {
        printf("Erro ao alocar memória para palavraFormada\n");
        free(tabelaHash);
        return 1;
    }

    criaTabela(tabelaHash, tamanhoTabela);
    setlocale(LC_ALL, "Portuguese_Brazil");

    recebeArqPalavrasChave(&tabelaHash, &listaPalavra, arqPalavrasChave, palavraFormada, caractereAtual, indice, 
    tamanhoPalavraFormada, caracteres, &tamanhoTabela);

    recebeArqTexto(&tabelaHash, arqTexto, palavraFormada, caractereAtual, indice, tamanhoTabela, 
    caracteres, tamanhoPalavraFormada);

    imprimeOrdTabela(tabelaHash, listaPalavra, tamanhoTabela);

    imprimirTabela(tabelaHash, tamanhoTabela);
    
    liberaListaPalavra(&listaPalavra);
    excluirTabela(tabelaHash, tamanhoTabela);

    free(palavraFormada);
    free(tabelaHash);

    
    return 0;
}