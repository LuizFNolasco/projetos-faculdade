#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX_WORD_LENGTH 100
#define MAX_WORDS 1000

void find_words_in_text(const char *words_file, const char *text_file) {
    char words[MAX_WORDS][MAX_WORD_LENGTH];
    int word_count = 0;

    // Abrir o arquivo de palavras
    FILE *words_fp = fopen("texto2.txt", "r");
    if (words_fp == NULL) {
        printf("Erro ao abrir o arquivo de palavras\n");
        return;
    }

    // Ler as palavras do arquivo
    while (fscanf(words_fp, "%s", words[word_count]) != EOF) {
        word_count++;
    }
    fclose(words_fp);

    // Abrir o arquivo de texto
    FILE *text_fp = fopen("texto.txt", "r");
    if (text_fp == NULL) {
        printf("Erro ao abrir o arquivo de texto\n");
        return;
    }

    char text_char;
    int char_count = 0;
    char buffer[MAX_WORD_LENGTH];
    int buffer_index = 0;

    // Ler o arquivo de texto caractere por caractere
    while ((text_char = fgetc(text_fp)) != EOF) {
        char_count++;

        // Se o caractere for um separador, verificar a palavra no buffer
        if (text_char == ' ' || text_char == '.' || text_char == ',' || text_char == '\n' || text_char == '\t') {
            if (buffer_index > 0) {
                buffer[buffer_index] = '\0';
                for (int i = 0; i < word_count; i++) {
                    if (strcmp(buffer, words[i]) == 0) {
                        printf("Palavra '%s' encontrada na posição %d\n", buffer, char_count - buffer_index);
                    }
                }
                buffer_index = 0;
            }
        } else {
            // Adicionar o caractere ao buffer
            buffer[buffer_index++] = text_char;
        }
    }

    // Verificar a última palavra no buffer
    if (buffer_index > 0) {
        buffer[buffer_index] = '\0';
        for (int i = 0; i < word_count; i++) {
            if (strcmp(buffer, words[i]) == 0) {
                printf("Palavra '%s' encontrada na posição %d\n", buffer, char_count - buffer_index);
            }
        }
    }

    fclose(text_fp);
}

int main() {
    find_words_in_text("palavras.txt", "texto.txt");
    return 0;
}