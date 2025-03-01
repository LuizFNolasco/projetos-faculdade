#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <ctype.h>
#include <locale.h>
#include <iconv.h>

// Estrutura da lista encadeada para armazenar apos quantos caracteres cada palavra aparece
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

// Estrutura do no de palavras chaves, para facilitar na ordenação das palavras
typedef struct no{
    char *palavraNo;
    struct no *prox;
}no;

//Cria a tabelaHash com as varaiaves zerados
void criaTabela(tabela *tabelaHash, int tamanhoTabela) {
    int i;
    for(i = 0; i < tamanhoTabela; i++) {
        tabelaHash[i].chave = NULL;
        tabelaHash[i].chaveSemAcento = NULL;
        tabelaHash[i].Lista = NULL;
    }
}

//Exclui todos os dados da tabela
void excluirTabela(tabela *tabelaHash, int tamanhoTabela) {
    int i = 0;

    //Variaveis necessarias
    aparicoes *atual = tabelaHash[i].Lista;
    aparicoes *proximo;

    //Loop para percorrer as palavras existentes na tabelaHash
    for(i; i < tamanhoTabela; i++) {
        if(tabelaHash[i].chave != NULL) {
            //Liberar a palavra
            free(tabelaHash[i].chave);
            //Lisberar a versão da palavra sem acento ou caracteres especiais
            if(tabelaHash[i].chaveSemAcento != NULL) {
                free(tabelaHash[i].chaveSemAcento);
            }

            // Liberar a lista de aparições
            while(atual != NULL) {
                proximo = atual->prox;

                free(atual);

                atual = proximo;
            }
        }
    }
}

// Funcao para inicializar o no que portara apos quantos caracteres que a palavra aparece
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

//Função responsavel por retirar os acentos e caracteres especiais da palavra
char *ajeitaPalavra(char *novaPalavra, int tamanhoPalavra) {
    int j = 0;  // ï¿½ndice para a nova string (pode ser menor que a original)
    
    for (int i = 0; i < tamanhoPalavra; i++) {
        unsigned char c = (unsigned char)novaPalavra[i];
        
        // Converte letras maiï¿½sculas para minï¿½sculas
        if (c >= 'A' && c <= 'Z') {
            novaPalavra[j] = c + 32;
            j++;
            continue;
        }
        
        // Mantï¿½m letras minï¿½sculas e nï¿½meros
        if ((c >= 'a' && c <= 'z') || (c >= '0' && c <= '9')) {
            novaPalavra[j] = c;
            j++;
            continue;
        }
        
        // Converte letras acentuadas para versï¿½es sem acento
        // Maiï¿½sculas
        switch (c) {
            // A com acentos
            case 193: case 192: case 194: case 195: case 196: case 197:
                novaPalavra[j] = 'a'; j++; break;
            // E com acentos
            case 201: case 200: case 202: case 203:
                novaPalavra[j] = 'e'; j++; break;
            // I com acentos
            case 205: case 204: case 206: case 207:
                novaPalavra[j] = 'i'; j++; break;
            // O com acentos
            case 211: case 210: case 212: case 213: case 214: case 216:
                novaPalavra[j] = 'o'; j++; break;
            // U com acentos
            case 218: case 217: case 219: case 220:
                novaPalavra[j] = 'u'; j++; break;
            // C cedilha
            case 199:
                novaPalavra[j] = 'c'; j++; break;
            // N com til
            case 209:
                novaPalavra[j] = 'n'; j++; break;
            // Y com acentos
            case 221:
                novaPalavra[j] = 'y'; j++; break;
                
            // Minï¿½sculas acentuadas
            // a com acentos (ï¿½, ï¿½, ï¿½, ï¿½, ï¿½, ï¿½)
            case 225: case 224: case 226: case 227: case 228: case 229:
                novaPalavra[j] = 'a'; j++; break;
            // e com acentos (ï¿½, ï¿½, ï¿½, ï¿½)
            case 233: case 232: case 234: case 235:
                novaPalavra[j] = 'e'; j++; break;
            // i com acentos (ï¿½, ï¿½, ï¿½, ï¿½)
            case 237: case 236: case 238: case 239:
                novaPalavra[j] = 'i'; j++; break;
            // o com acentos (ï¿½, ï¿½, ï¿½, ï¿½, ï¿½, ï¿½)
            case 243: case 242: case 244: case 245: case 246: case 248:
                novaPalavra[j] = 'o'; j++; break;
            // u com acentos (ï¿½, ï¿½, ï¿½, ï¿½)
            case 250: case 249: case 251: case 252:
                novaPalavra[j] = 'u'; j++; break;
            // c cedilha
            case 231:
                novaPalavra[j] = 'c'; j++; break;
            // n com til
            case 241:
                novaPalavra[j] = 'n'; j++; break;
            // y com acentos (ï¿½, ï¿½)
            case 253: case 255:
                novaPalavra[j] = 'y'; j++; break;
                
            // Outros caracteres especiais sï¿½o ignorados
            default:
                // Nï¿½o adiciona caractere
                break;
        }
    }
    // Finaliza a string
    novaPalavra[j] = '\0';
    
return novaPalavra;
}

//Calcula o indice em que a palavra ira se encontrar
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

//Insere a palavra na tabelaHash
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

        tabelaHash[aux].chaveSemAcento = (char*)calloc(tamanhoPalavra + 1, sizeof(char));
        char *ajeita = ajeitaPalavra(palavra, tamanhoPalavra);
        strcpy(tabelaHash[aux].chaveSemAcento, ajeita);

        tabelaHash[aux].Lista = NULL;
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

//Cria o no que guardar  a lista de palavras chave
no *criaNo(char *palavra, int tamanhoPalavra){
    no *novo = (no*)calloc(1, sizeof(no));

    if(novo == NULL){
        printf("Erro ao tentar alocar");
        return NULL;
    }
    novo->palavraNo = (char*)calloc(tamanhoPalavra + 1, sizeof(char));
    char *ajeita = ajeitaPalavra(palavra, tamanhoPalavra);

    strcpy(novo->palavraNo, ajeita);

    novo->prox = NULL;

return novo;
}

//Insere de forma ordenada a palavra sem acentos e caracteres especiais dentro de uma lista encadeada
void insereOrdPalavra(no **listaPalavra, char *palavra, int tamanhoPalavra) {
    no *novo = criaNo(palavra, tamanhoPalavra);
    no *aux = *listaPalavra;
    no *anterior = NULL;

    // Caso a lista esteja vazia, insere no inï¿½cio
    if (*listaPalavra == NULL) {
        *listaPalavra = novo;
        return;
    }

    // Percorre a lista para encontrar a posiï¿½ï¿½o correta de inserï¿½ï¿½o
    while (aux != NULL) {
        int resultado = strcmp(aux->palavraNo, novo->palavraNo);

        // Verifica se a palavra jï¿½ existe na lista
        if (resultado == 0) {
            free(novo->palavraNo);
            free(novo);
            return;
        }

        // Encontrou a posiï¿½ï¿½o correta para inserï¿½ï¿½o
        if (resultado > 0) {
            break;
        }

        anterior = aux;
        aux = aux->prox;
    }

    // Insere o novo nï¿½ na posiï¿½ï¿½o correta
    if (anterior == NULL) {
        // Insere no inï¿½cio da lista
        novo->prox = *listaPalavra;
        *listaPalavra = novo;
    } else {
        // Insere no meio ou no final da lista
        novo->prox = anterior->prox;
        anterior->prox = novo;
    }
}

// Realiza a impressão de todos os nó's que estão na lista encadeada
void exibirLista(no *listaPalavras) {
    no *atual = listaPalavras;
    
    while (atual != NULL) {
        printf("%s\n", atual->palavraNo);
        atual = atual->prox;
    }
}

//Elimina todos os nó's que estão na lista encadeada
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

// Imprime a tabela de forma que os elementos apareçam ordenados por ordem alfabetica
void imprimeOrdTabela(tabela *tabelaHash, no *listaPalavras, int tamanhoTabela) {
    aparicoes *aux = NULL;
    no *aux2 = listaPalavras;
    int posicao;

    while (aux2 != NULL) {
        posicao = funcaoHash(strlen(aux2->palavraNo), tamanhoTabela);
        int tentativas = 0;

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

// Aumenta o tamanho da tabela, para conseguir guardar mais elementos
void aumentaTabela(tabela **tabelaHash, no *listaPalavras, int *tamanhoTabela) {
    if (*tabelaHash == NULL || tamanhoTabela == NULL) {
        printf("Erro: ponteiro nulo passado para aumentaTabela.\n");
        return;
    }

    int novoTamanho = *tamanhoTabela * 2;
    tabela *novaTabela = (tabela*)calloc(novoTamanho, sizeof(tabela));

    int posicao;

    if (novaTabela == NULL) {
        printf("Erro ao tentar alocar memória para a nova tabela.\n");
        return;
    }

    // Inicializa a nova tabela
    criaTabela(novaTabela, novoTamanho);

    // Copia os elementos da tabela antiga para a nova tabela
    for (int i = 0; i < *tamanhoTabela; i++) {
        if ((*tabelaHash)[i].chave != NULL) {
            // Insere a palavra e todos os elementos da lista de colisões
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

    // Libera a memória da tabela antiga
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

    // Atualiza o ponteiro da tabela hash e o tamanho
    *tabelaHash = novaTabela;
    *tamanhoTabela = novoTamanho;
}

//Abre o arquivo de palavras chave e insere tanto na lista encadeada quando na tabela
void recebeArqPalavrasChave(tabela **tabelaHash, no **listaPalavra, char *arqPalavrasChave, char *palavraFormada, int caractereAtual, int indice, 
    int tamanhoPalavraFormada, int possivelFinal, int caracteres, int tamanhoTabela, int j) {
    
    int posicaoInicio = 0;

    printf("Digite o nome do arquivo de palavras chaves: ");
    scanf("%s", arqPalavrasChave);

    FILE *arqP = fopen(arqPalavrasChave, "r");
    if (arqP == NULL) {
        printf("Erro ao tentar abrir o arquivo.\n");
        return;
    }

    while ((caractereAtual = fgetc(arqP)) != EOF) {
        if (caractereAtual != ' ') {
            if (indice >= tamanhoPalavraFormada - 1) {
                tamanhoPalavraFormada *= 2;
                char *temp = (char *)realloc(palavraFormada, tamanhoPalavraFormada * sizeof(char));
                if (temp == NULL) {
                    printf("Erro ao realocar memória.\n");
                    fclose(arqP);
                    return;
                }
                palavraFormada = temp;
            }

            if ((caractereAtual == '!' || caractereAtual == '.' || caractereAtual == ',' || caractereAtual == ';' 
                || caractereAtual == ':' || caractereAtual == '@' || caractereAtual == '#' || caractereAtual == '$'
                || caractereAtual == '%' || caractereAtual == '&' || caractereAtual == '*' || caractereAtual == '(' 
                || caractereAtual == ')') && j <= 1) {
                possivelFinal = indice;
                j++;
            }

            palavraFormada[indice] = caractereAtual;
            indice++;
        } else if (indice > 0) {
            if (palavraFormada[indice - 1] == '!' || palavraFormada[indice - 1] == '.' || palavraFormada[indice - 1] == ',' || palavraFormada[indice - 1] == ';' 
                || palavraFormada[indice - 1] == ':' || palavraFormada[indice - 1] == '@' || palavraFormada[indice - 1] == '#' || palavraFormada[indice - 1] == '$'
                || palavraFormada[indice - 1] == '%' || palavraFormada[indice - 1] == '&' || palavraFormada[indice - 1] == '*' || palavraFormada[indice - 1] == '(' 
                || palavraFormada[indice - 1] == ')') {
                palavraFormada[possivelFinal] = '\0';
                posicaoInicio = caracteres - indice; // Posição de início da palavra
                inserirPalavra((*tabelaHash), palavraFormada, tamanhoTabela, posicaoInicio, possivelFinal);
                insereOrdPalavra(listaPalavra, palavraFormada, possivelFinal);
            } else {
                palavraFormada[indice] = '\0';
                posicaoInicio = caracteres - indice; // Posição de início da palavra
                inserirPalavra((*tabelaHash), palavraFormada, tamanhoTabela, posicaoInicio, indice);
                insereOrdPalavra(listaPalavra, palavraFormada, indice);
            }
            indice = 0;
            j = 0;
        }
        caracteres++; // Incrementa o contador de caracteres
    }

    if (indice > 0) {
        palavraFormada[indice] = '\0';
        posicaoInicio = caracteres - indice; // Posição de início da palavra
        inserirPalavra((*tabelaHash), palavraFormada, tamanhoTabela, posicaoInicio, indice);
        insereOrdPalavra(listaPalavra, palavraFormada, indice);
    }

    fclose(arqP);
}

//Abre o arquivo de texto e insere após quantos caracteres cada palavra aparece
void recebeArqTexto(tabela **tabelaHash, char *arqTexto, char *palavraFormada, int caractereAtual, int possivelFinal, int indice, int tamanhoTabela, 
    int caracteres, int tamanhoPalavraFormada, int j) {
    
    int posicaoInicio = 0;
    
    printf("Digite o nome do arquivo a ser lido: ");
    scanf("%s", arqTexto);

    FILE *arqT = fopen(arqTexto, "r");
    if (arqT == NULL) {
        printf("Erro ao tentar abrir o arquivo.\n");
        return;
    }

    while ((caractereAtual = fgetc(arqT)) != EOF) {
        if (caractereAtual != ' ') {
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

            if ((caractereAtual == '!' || caractereAtual == '.' || caractereAtual == ',' || caractereAtual == ';' 
                || caractereAtual == ':' || caractereAtual == '@' || caractereAtual == '#' || caractereAtual == '$'
                || caractereAtual == '%' || caractereAtual == '&' || caractereAtual == '*' || caractereAtual == '(' 
                || caractereAtual == ')') && j <= 1) {
                possivelFinal = indice;
                j++;
            }

            palavraFormada[indice] = caractereAtual;
            indice++;
        } else if (indice > 0) {
            if (palavraFormada[indice - 1] == '!' || palavraFormada[indice - 1] == '.' || palavraFormada[indice - 1] == ',' || palavraFormada[indice - 1] == ';' 
                || palavraFormada[indice - 1] == ':' || palavraFormada[indice - 1] == '@' || palavraFormada[indice - 1] == '#' || palavraFormada[indice - 1] == '$'
                || palavraFormada[indice - 1] == '%' || palavraFormada[indice - 1] == '&' || palavraFormada[indice - 1] == '*' || palavraFormada[indice - 1] == '(' 
                || palavraFormada[indice - 1] == ')') {
                palavraFormada[possivelFinal] = '\0';
                posicaoInicio = caracteres - indice; // Posição de início da palavra
                buscarPalavra((*tabelaHash), palavraFormada, tamanhoTabela, posicaoInicio, possivelFinal);
            } else {
                palavraFormada[indice] = '\0';
                posicaoInicio = caracteres - indice; // Posição de início da palavra
                buscarPalavra((*tabelaHash), palavraFormada, tamanhoTabela, posicaoInicio, indice);
            }
            indice = 0;
            j = 0;
        }
        caracteres++; // Incrementa o contador de caracteres
    }

    if (indice > 0) {
        palavraFormada[indice] = '\0';
        posicaoInicio = caracteres - indice; // Posição de início da palavra
        buscarPalavra((*tabelaHash), palavraFormada, tamanhoTabela, posicaoInicio, indice);
    }

    fclose(arqT);
}

// Função para converter o encoding de um arquivo
void converterParaISO8859_1(const char *arquivoEntrada, const char *arquivoSaida) {
    FILE *entrada = fopen(arquivoEntrada, "r");
    FILE *saida = fopen(arquivoSaida, "w");

    if (!entrada || !saida) {
        perror("Erro ao abrir arquivo");
        exit(EXIT_FAILURE);
    }

    // Configura o conversor de encoding
    iconv_t conversor = iconv_open("ISO-8859-1", "UTF-8");
    if (conversor == (iconv_t)-1) {
        perror("Erro ao abrir conversor de encoding");
        fclose(entrada);
        fclose(saida);
        exit(EXIT_FAILURE);
    }

    char bufferEntrada[1024];
    char bufferSaida[1024];
    size_t tamanhoEntrada, tamanhoSaida;

    while (fgets(bufferEntrada, sizeof(bufferEntrada), entrada)) {
        char *entradaPtr = bufferEntrada;
        char *saidaPtr = bufferSaida;
        tamanhoEntrada = strlen(bufferEntrada);
        tamanhoSaida = sizeof(bufferSaida);

        // Converte o conteúdo
        if (iconv(conversor, &entradaPtr, &tamanhoEntrada, &saidaPtr, &tamanhoSaida) == (size_t)-1) {
            perror("Erro ao converter encoding");
            iconv_close(conversor);
            fclose(entrada);
            fclose(saida);
            exit(EXIT_FAILURE);
        }

        // Escreve o conteúdo convertido no arquivo de saída
        fwrite(bufferSaida, 1, sizeof(bufferSaida) - tamanhoSaida, saida);
    }

    // Fecha os arquivos e o conversor
    iconv_close(conversor);
    fclose(entrada);
    fclose(saida);

    printf("Arquivo convertido com sucesso para ISO-8859-1: %s\n", arquivoSaida);
}

//Menu que irá aparecer para o usuario
void menu(){

}

int main(){
    tabela *tabelaHash; // 9x9
    no *listaPalavra = NULL;
    aparicoes *listaAparicoes = NULL;

    char arqPalavrasChave[1000], arqTexto[1000];
    int caractereAtual, tamanhoPalavraFormada = 100, indice = 0;
    int posicaoTabela = 0, tamanhoTabela = 81, caracteres = 1, j = 0, possivelFinal = 0;

    tabelaHash = (tabela*)calloc(tamanhoTabela, sizeof(tabela));
    char *palavraFormada = (char*)calloc(tamanhoPalavraFormada, sizeof(char));

    criaTabela(tabelaHash, tamanhoTabela);  

     //setlocale(LC_ALL, "Portuguese_Brazil");

    recebeArqPalavrasChave(&tabelaHash, &listaPalavra, arqPalavrasChave, palavraFormada, caractereAtual, indice, 
    tamanhoPalavraFormada, possivelFinal, caracteres, tamanhoTabela, j);

    recebeArqTexto(&tabelaHash, arqTexto, palavraFormada, caractereAtual, possivelFinal, indice, tamanhoTabela, 
    caracteres, tamanhoPalavraFormada, j);

    aumentaTabela(&tabelaHash, listaPalavra, &tamanhoTabela);   
    
    // exibirLista(listaPalavra);
    //imprimirTabela(tabelaHash, tamanhoTabela, 10);

    imprimeOrdTabela(tabelaHash, listaPalavra, tamanhoTabela);

    liberaListaPalavra(&listaPalavra); 
    excluirTabela(tabelaHash, tamanhoTabela);

return 0;
}

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