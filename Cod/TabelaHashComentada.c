#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <ctype.h>
#include <locale.h>
#include <iconv.h>

// Estrutura da lista encadeada para armazenar apos quantos caracteres cada palavra aparece
typedef struct aparicoes{
    int tamanho; //Numero de após quantos caracteres a palavra aparece
    struct aparicoes *prox; //Ponteiro para o próximo nó com a contagem de caracteres  
}aparicoes;

// Estrutura da tabela hash para armazenar a palavra e a lista de apos quantos caracteres aparece a palavra
typedef struct tabela{
    char *chave; //Ponteiro para a palavra chave 
    char *chaveSemAcento; //Ponteiro para a palavra sem acentos, caracteres especiais e letras maiusculas
    aparicoes *Lista; //Ponteiro para a lista de após quantos caracteres ocorre cada ocorrencia da palavra
}tabela;

// Estrutura do no de palavras chaves, para facilitar na ordenação das palavras
typedef struct no{
    char *palavraNo; //Ponteiro para a versão da palavra sem acentos, caracteres especiais e letras maiusculas
    struct no *prox; //Ponteiro para o proximo nó com a versão da palavra sem acentos, caracteres especiais e letras maiusculas
}no;

//Cria a tabelaHash com as varaiaves zerados
void criaTabela(tabela *tabelaHash, int tamanhoTabela) {
    int i; // Contador para percorrer a tabela hash
    for(i = 0; i < tamanhoTabela; i++) {
        //Garantindo que cada registro do vetor aponte para nenhum espaço da memoria 
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
            //Liberar a memoria da palavra
            free(tabelaHash[i].chave);
            //Liberar a memoria da versão da palavra sem acento ou caracteres especiais
            if(tabelaHash[i].chaveSemAcento != NULL) {
                free(tabelaHash[i].chaveSemAcento);
            }

            // Liberar a memoria da lista de aparições
            while(atual != NULL) {
                proximo = atual->prox;

                free(atual);

                atual = proximo;
            }
        }
    }
}

// Funcao para inicializar o nó que portara apos quantos caracteres que a palavra aparece
aparicoes *criaLista(int caracteres){
    //Alocação de memoria para um ponteiro para a estrutura aparições
    aparicoes *novo = (aparicoes*)calloc(1, sizeof(aparicoes));

    if(novo == NULL){
        printf("Erro ao tentar alocar");
        return NULL;
    }
    //Inicializa os campos do nó com 
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

//Função responsável por retirar os acentos e caracteres especiais da palavra
char *ajeitaPalavra(char *novaPalavra, int tamanhoPalavra) {
    int j = 0; //Índice do caractere que está sendo alterado

    for (int i = 0; i < tamanhoPalavra; i++) {
        unsigned char c = (unsigned char)novaPalavra[i];

        //Converte letras maiúsculas para minúsculas
        if (c >= 'A' && c <= 'Z') {
            novaPalavra[j] = c + 32; //Converte para minúscula
            j++;
            continue;
        }

        //Mantém letras minúsculas e números
        if ((c >= 'a' && c <= 'z') || (c >= '0' && c <= '9')) {
            novaPalavra[j] = c;
            j++;
            continue;
        }

        //Converte letras acentuadas para versão sem acento
        //Maiúsculas
        switch (c) {
            //Á, À, Â, Ã, Ä, Å
            case 193: case 192: case 194: case 195: case 196: case 197:
                novaPalavra[j] = 'a'; j++; break;
            //É, È, Ê, Ë
            case 201: case 200: case 202: case 203:
                novaPalavra[j] = 'e'; j++; break;
            //Í, Ì, Î, Ï
            case 205: case 204: case 206: case 207:
                novaPalavra[j] = 'i'; j++; break;
            //Ó, Ò, Ô, Õ, Ö, Ø
            case 211: case 210: case 212: case 213: case 214: case 216:
                novaPalavra[j] = 'o'; j++; break;
            //Ú, Ù, Û, Ü
            case 218: case 217: case 219: case 220:
                novaPalavra[j] = 'u'; j++; break;
            //Ç
            case 199:
                novaPalavra[j] = 'c'; j++; break;
            //Ñ
            case 209:
                novaPalavra[j] = 'n'; j++; break;
            //Ý
            case 221:
                novaPalavra[j] = 'y'; j++; break;

            //Minúsculas acentuadas
            //á, à, â, ã, ä, å
            case 225: case 224: case 226: case 227: case 228: case 229:
                novaPalavra[j] = 'a'; j++; break;
            //é, è, ê, ë
            case 233: case 232: case 234: case 235:
                novaPalavra[j] = 'e'; j++; break;
            //í, ì, î, ï
            case 237: case 236: case 238: case 239:
                novaPalavra[j] = 'i'; j++; break;
            //ó, ò, ô, õ, ö, ø
            case 243: case 242: case 244: case 245: case 246: case 248:
                novaPalavra[j] = 'o'; j++; break;
            //ú, ù, û, ü
            case 250: case 249: case 251: case 252:
                novaPalavra[j] = 'u'; j++; break;
            //ç
            case 231:
                novaPalavra[j] = 'c'; j++; break;
            //ñ
            case 241:
                novaPalavra[j] = 'n'; j++; break;
            //ý, ÿ
            case 253: case 255:
                novaPalavra[j] = 'y'; j++; break;
            //Outros caracteres especiais serão ignorados
            default:
                break;
        }
    }
    //Finaliza a string
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

    //Verifica se o tamanho da tabela é maior ou não do que a posição, para a divisão não dar um numero pequeno demais
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
        //Verifica se tem alguma palavra no campo chave e se a palavra buscada está na tabela
        if(tabelaHash[aux].chave != NULL && strcmp(tabelaHash[aux].chave, palavra) == 0){
            //Inserção na lista encadeada de aparições a quantidade de caracteres que vem antes da palavra chave
            tabelaHash[aux].Lista = insereFimAparicoes(tabelaHash[aux].Lista, caracteres);
            
            return 1;
        }
        //Como o metodo é de hash linear, é feito uma soma no resultado da função hash, caso a posição esteja com uma palavra diferente da buscada
        aux = aux + k;
        k++;
    }
return 0;
}

//Insere a palavra na tabelaHash
void inserirPalavra(tabela *tabelaHash, char *palavra, int tamanhoTabela, int caracteres, int tamanhoPalavra){  
    //Verifica se a palavra está ou não na tabela  
    int resultadoBusca = buscarPalavra(tabelaHash, palavra, tamanhoTabela, caracteres, tamanhoPalavra);
    int aux = funcaoHash(tamanhoPalavra, tamanhoTabela);
    int k = 1;
    char *ajeita;

    if(resultadoBusca == 0){
        //Caso a posição esteja ocupada, ele calcula outra para inserir a nova palavra usando o metodo do hash linear
        while(tabelaHash[aux].chave != NULL && aux < tamanhoTabela){
            aux = aux + k;
            k++;
        }
        //Alocação dinamica para não disperdiçar memoria onde será inserida a palavra
        tabelaHash[aux].chave = (char*)calloc(tamanhoPalavra + 1, sizeof(char));
        //Função que passa uma copia do segundo argumento e passa para o primeiro
        strcpy(tabelaHash[aux].chave, palavra); 

        //Alocação dinamica para nao disperdiçar memoria onde será inserida a versão da palavra sem acento, caracteres especiais e letras maiusculas
        tabelaHash[aux].chaveSemAcento = (char*)calloc(tamanhoPalavra + 1, sizeof(char));
        //Variavel que recebe a versão da palavra sem acento, caracteres especiais e letras maiusculas
        ajeita = ajeitaPalavra(palavra, tamanhoPalavra);
        //Função que faz uma copia do segundo argumento e passa para o primeiro
        strcpy(tabelaHash[aux].chaveSemAcento, ajeita);

        //Inicializando o ponteiro para a lista de aparições da nova palavra
        tabelaHash[aux].Lista = NULL;
    }
}

//Funcao responsavel pela impressao da tabela inteira, ata mesmo as casas com caracteres nulos
void imprimirTabela(tabela *tabelaHash, int tamanhoTabela, int tamanhoPalavra){
    int i;

    printf("--------------------------------\n");

    for(i = 0; i < tamanhoTabela; i++) {
        //Imprime um numero de até 7 digitos, é preenchido da esq para a dir
        printf("| %-7d | ", i);
        
        if(tabelaHash[i].chave != NULL) {
            //Imprime a palavra chave em caso dela ter até 18 digitos, é preenchido da esq para a dir
            printf("%-18s |\n", tabelaHash[i].chave);
        } else {
            //Caso não tenha palavra imprime um espaço vazio
            printf("%-18s |\n", "");
        }
    }
    
    printf("--------------------------------\n");
}

//Cria o no que guardar  a lista de palavras chave
no *criaNo(char *palavra, int tamanhoPalavra){
    no *novo = (no*)calloc(1, sizeof(no));
    char *ajeita;

    if(novo == NULL){
        printf("Erro ao tentar alocar");
        return NULL;
    }
    //Aloca memoria dinamicamente para não disperdiçar memoria
    novo->palavraNo = (char*)calloc(tamanhoPalavra + 1, sizeof(char));
    //Versão da palavra sem acentos, caracteres especiais e letras maiusculas
    ajeita = ajeitaPalavra(palavra, tamanhoPalavra);

    //Função que passa uma copia do segundo argumento e passa para o primeiro 
    strcpy(novo->palavraNo, ajeita);

    novo->prox = NULL;

return novo;
}

//Insere de forma ordenada a palavra sem acentos e caracteres especiais dentro de uma lista encadeada
void insereOrdPalavra(no **listaPalavra, char *palavra, int tamanhoPalavra) {
    no *novo = criaNo(palavra, tamanhoPalavra);
    no *aux = *listaPalavra;
    no *anterior = NULL;
    int resultado;

    //Caso a lista esteja vazia, insere no inicio
    if (*listaPalavra == NULL) {
        *listaPalavra = novo;
        return;
    }

    //Percorre a lista para encontrar a posição correta de inserção
    while (aux != NULL) {
        //Função que compara duas strings e retorna 0 caso sejam iguais
        resultado = strcmp(aux->palavraNo, novo->palavraNo);

        //Verifica se a palavra já existe na lista
        if (resultado == 0) {
            free(novo->palavraNo);
            free(novo);
            return;
        }

        //Encontrou a posição correta para inserção
        if (resultado > 0) {
            break;
        }

        anterior = aux;
        aux = aux->prox;
    }

    //Insere o novo nó na posição correta
    if (anterior == NULL) {
        //Insere no inicio da lista
        novo->prox = *listaPalavra;
        *listaPalavra = novo;
    } else {
        //Insere no meio ou no final da lista
        novo->prox = anterior->prox;
        anterior->prox = novo;
    }
}

//Elimina todos os nó's que estão na lista encadeada
void liberaListaPalavra(no **listaPalavras) {
    no *atual = *listaPalavras;
    no *proximo;

    //Loop para liberar a memoria alocada para todos os nó's que estão na lista
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
    no *aux2 = listaPalavras; //Variavel para percorrer a lista de palavras sem acentos, caracteres especiais e letras maiusculas
    int posicao, tentativas;

    while (aux2 != NULL) {
        //Strlen = retorna o tamanho da string em quantidade de caracteres
        posicao = funcaoHash(strlen(aux2->palavraNo), tamanhoTabela);
        tentativas = 0;

        while (tentativas < tamanhoTabela) {
            //Verifica se existe uma palavra na posição indicada e se a palavra que se encontra nessa tabela é a mesma que está na posição aux2 da lista encadeada
            if (tabelaHash[posicao].chave != NULL && strcmp(tabelaHash[posicao].chaveSemAcento, aux2->palavraNo) == 0) {
                //Imprime a palavra
                printf("%s", tabelaHash[posicao].chave);
                aux = tabelaHash[posicao].Lista;

                //Imprime a lista encadeada de ocorrencias da palavra
                while (aux != NULL) {
                    printf(" %d", aux->tamanho);
                    aux = aux->prox;
                }
                printf("\n");
                break;
            } else { //Caso dê errado é feito o hash linear
                posicao = posicao + tentativas;
                tentativas++;
            }
        }
        aux2 = aux2->prox;
    }
    printf("\n");
}

// Aumenta o tamanho da tabela, para conseguir guardar mais elementos
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

    //Inicializa a nova tabela
    criaTabela(novaTabela, novoTamanho);

    //Copia os elementos da tabela antiga para a nova tabela
    for (int i = 0; i < *tamanhoTabela; i++) {
        if ((*tabelaHash)[i].chave != NULL) {
            //Transfere todas as palavras e lista de ocorrencias de cada palavra da tabela antiga para nova 
            posicao = funcaoHash(strlen((*tabelaHash)[i].chave), novoTamanho);
            //strdup = calcula o tamanho, aloca a memoria dinamicamente e duplica a palavra
            novaTabela[posicao].chave = strdup((*tabelaHash)[i].chave);
            novaTabela[posicao].chaveSemAcento = strdup((*tabelaHash)[i].chaveSemAcento);

            //Variaveis para fazer a transferencia
            aparicoes *listaVelha = (*tabelaHash)[i].Lista;
            aparicoes *aux = listaVelha;
            aparicoes *novaLista = NULL;

            //Loop para transferencia da lista de após quantos caracteres a palavra ocorre
            while(aux != NULL){
                //Novo nó que receberá as informações
                aparicoes *novo = (aparicoes*)calloc(1, sizeof(aparicoes));

                novo->tamanho = aux->tamanho;
                novo->prox = NULL;

                if(novaLista == NULL){
                    novaLista = novo;
                }

                aux = aux->prox;
            }
            //Passagem do ponteiro principal para o inicio da lista
            novaTabela[posicao].Lista = novaLista;
        }
    }

    // Libera a memória da tabela antiga
    for (int i = 0; i < *tamanhoTabela; i++) {
        if ((*tabelaHash)[i].chave != NULL) {
            free((*tabelaHash)[i].chave);
            free((*tabelaHash)[i].chaveSemAcento);

            aparicoes *aux = (*tabelaHash)[i].Lista;

            //Libera memoria da lista encadeada de após quantos caracteres a palavra ocorre
            while (aux != NULL) {
                aparicoes *prox = aux->prox;
                free(aux);
                aux = prox;
            }
        }
    }
    free(*tabelaHash);

    //Atualiza o ponteiro da tabela hash e o do tamanho
    *tabelaHash = novaTabela;
    *tamanhoTabela = novoTamanho;
}

//Abre o arquivo de palavras chave e insere tanto na lista encadeada quando na tabela
void recebeArqPalavrasChave(tabela **tabelaHash, no **listaPalavra, char *arqPalavrasChave, char *palavraFormada, int caractereAtual, int indice, 
    int tamanhoPalavraFormada, int caracteres, int tamanhoTabela) {
    
    int posicaoInicio = 0;
    char *temp;

    printf("Digite o nome do arquivo de palavras chaves: ");
    scanf("%s", arqPalavrasChave);

    //Abertura do arquvio para leitura
    FILE *arqP = fopen(arqPalavrasChave, "r");

    if (arqP == NULL) {
        printf("Erro ao tentar abrir o arquivo.\n");
        return;
    }

    //Loop para percorrer o arquivo de palavras chave
    while ((caractereAtual = fgetc(arqP)) != EOF) {
        //Caso não tenha um espaço, virgula ou algum tipo de pontuação insere o caracter na palavra
        if (caractereAtual != ' ' && caractereAtual != '!' && caractereAtual != '.' && caractereAtual != ',' && caractereAtual != ';' 
            && caractereAtual != ':' && caractereAtual != '(' && caractereAtual != ')') {
            
            //Caso o indice esteja muito proximo do tamanho maximo da palavraFormada é feito uma realocação  
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
            palavraFormada[indice] = caractereAtual; //Insere o caractere lido na palavra que está se formando
            indice++; //Quantidade de caracteres que a palavra possui

        } else if (indice > 0) {

            palavraFormada[indice] = '\0'; //Anular o ultimo caracter já que a palavra pode possuir resquicios de outras palavras formadas anteriormente
            posicaoInicio = caracteres - indice; // Posição de início da palavra
            //Insere a palavra formada na tabela hash
            inserirPalavra((*tabelaHash), palavraFormada, tamanhoTabela, posicaoInicio, indice); 
            //Insere a palavra formada na lista encadeada de palavras sem acentos, caracteres especiais e letras maiusculas
            insereOrdPalavra(listaPalavra, palavraFormada, indice);

            indice = 0; //Reinicia o contador de caracteres da palavra
            j = 0;
        }
        caracteres++; //Incrementa o contador de quantos caracteres foram lidos
    }

    if (indice > 0) {
        palavraFormada[indice] = '\0';
        posicaoInicio = caracteres - indice;
        
        inserirPalavra((*tabelaHash), palavraFormada, tamanhoTabela, posicaoInicio, indice);
        insereOrdPalavra(listaPalavra, palavraFormada, indice);
    }
    //Fecha o arquivo de palavras chave
    fclose(arqP);
}

//Abre o arquivo de texto e insere após quantos caracteres cada palavra aparece
void recebeArqTexto(tabela **tabelaHash, char *arqTexto, char *palavraFormada, int caractereAtual, int indice, int tamanhoTabela, 
    int caracteres, int tamanhoPalavraFormada) {
    
    int posicaoInicio = 0;
    char *temp;
    
    printf("Digite o nome do arquivo a ser lido: ");
    scanf("%s", arqTexto);

    //Abre o arquivo de texto para leitura
    FILE *arqT = fopen(arqTexto, "r");

    if (arqT == NULL) {
        printf("Erro ao tentar abrir o arquivo.\n");
        return;
    }

    while ((caractereAtual = fgetc(arqT)) != EOF) {
        if (caractereAtual != ' ' && caractereAtual != '!' && caractereAtual != '.' && caractereAtual != ',' && caractereAtual != ';' 
            && caractereAtual != ':' && caractereAtual != '(' && caractereAtual != ')') {

            if (indice >= tamanhoPalavraFormada - 1) {
                
                tamanhoPalavraFormada *= 2;
                temp = (char *)realloc(palavraFormada, tamanhoPalavraFormada * sizeof(char));
                
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
            
            //Como a tabela já está formada é feito uma busca, que inclui a inserção de após quantos caracteres a palavra ocorre
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
    //Abre o arquivo para leitura e escrita
    FILE *arquivoPtr = fopen(arquivo, "r+");

    //Buffers para leitura e escrita do arquivo
    char bufferEntrada[1024];
    char bufferSaida[1024];
    size_t tamanhoEntrada, tamanhoSaida;

    //Ponteiros auxiliares para função iconv
    char *entradaAux;
    char *saidaAux;

    long posicaoInicial = 0; //Guarda a posição inicial para sobrescrever

    if (arquivoPtr == NULL) {
        perror("Erro ao abrir arquivo");
        exit(EXIT_FAILURE);
    }

    //Inicializa o conversor de encoding
    iconv_t conversor = iconv_open("ISO-8859-1", "UTF-8");
    //Verifica se ouve um erro ao tentar abrir o arquivo
    if (conversor == (iconv_t)-1) {
        //Perror = só pra exibir uma mensagem de erro personalizada
        perror("Erro ao abrir conversor de encoding");
        fclose(arquivoPtr);
        exit(EXIT_FAILURE);
    }

    //Lê o arquivo linha por linha, convertendo e sobrescrevendo
    while (fgets(bufferEntrada, sizeof(bufferEntrada), arquivoPtr)) {
        //Prepara os buffers para a conversão
        entradaAux = bufferEntrada;
        saidaAux = bufferSaida;
        tamanhoEntrada = strlen(bufferEntrada);
        tamanhoSaida = sizeof(bufferSaida);

        //Converte o encoding
        if (iconv(conversor, &entradaAux, &tamanhoEntrada, &saidaAux, &tamanhoSaida) == (size_t)-1) {
            perror("Erro ao converter encoding");
            iconv_close(conversor);
            fclose(arquivoPtr);
            exit(EXIT_FAILURE);
        }

        //Move o ponteiro do arquivo para a posição inicial da linha atual
        fseek(arquivoPtr, posicaoInicial, SEEK_SET);

        //Escreve o conteúdo convertido no arquivo
        fwrite(bufferSaida, 1, sizeof(bufferSaida) - tamanhoSaida, arquivoPtr);

        //Atualiza a posição inicial para a próxima linha
        posicaoInicial = ftell(arquivoPtr);
    }

    //Fecha o conversor e o arquivo
    iconv_close(conversor);
    fclose(arquivoPtr);
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

     setlocale(LC_ALL, "Portuguese_Brazil");

    recebeArqPalavrasChave(&tabelaHash, &listaPalavra, arqPalavrasChave, palavraFormada, caractereAtual, indice, 
    tamanhoPalavraFormada, caracteres, tamanhoTabela);

    recebeArqTexto(&tabelaHash, arqTexto, palavraFormada, caractereAtual, indice, tamanhoTabela, 
    caracteres, tamanhoPalavraFormada);

    aumentaTabela(&tabelaHash, listaPalavra, &tamanhoTabela);   
    
    // exibirLista(listaPalavra);
    //imprimirTabela(tabelaHash, tamanhoTabela, 10);

    imprimeOrdTabela(tabelaHash, listaPalavra, tamanhoTabela);

    liberaListaPalavra(&listaPalavra); 
    excluirTabela(tabelaHash, tamanhoTabela);

return 0;
}