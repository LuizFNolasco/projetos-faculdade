# Diretórios
SRC_DIR := src
BIN_DIR := bin
IN_DIR := in
OUT_DIR := out
INC_DIR := include

# Nome do executável
TARGET := trabSO

# Compilador e flags
CC := gcc
CFLAGS := -Wall -Wextra -I$(SRC_DIR) -I$(INC_DIR)

# Lista de arquivos .c e .o
SRC_FILES := $(wildcard $(SRC_DIR)/*.c)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.c, $(BIN_DIR)/%.o, $(SRC_FILES))

# --- Regras principais ---
all: $(TARGET)

monoprocessador: clean
	$(MAKE) all

multiprocessador: clean
	$(MAKE) all

# Como gerar o executável
$(TARGET): $(OBJ_FILES)
	$(CC) $(OBJ_FILES) -o $@

# Como compilar arquivos .c para .o
$(BIN_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Executar programa normalmente (entrada -> saída)
run: $(TARGET)
	@mkdir -p $(OUT_DIR)
	./$(TARGET) $(IN_DIR)/in.txt > $(OUT_DIR)/out.txt

# Teste com Valgrind: saída e logs separados
valgrind: $(TARGET)
	@mkdir -p $(OUT_DIR)
	valgrind -s --leak-check=full --show-leak-kinds=all \
		--log-file=$(OUT_DIR)/valgrind.log \
		./$(TARGET) $(IN_DIR)/entradas/6.txt > $(OUT_DIR)/out.txt

# Limpa objetos e executável
clean:
	rm -rf $(BIN_DIR)/*.o $(TARGET) $(OUT_DIR)/*

# Compacta tudo em um tar.gz para entrega, mantendo pastas
TARNAME = 202210214
tarball: clean
	tar -czvf $(TARNAME).tar.gz src include Makefile README.md

.PHONY: all clean run valgrind monoprocessador multiprocessador tarball
