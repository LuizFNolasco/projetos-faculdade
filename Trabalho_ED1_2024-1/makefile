all: teste

teste: TrabalhoED1.o main.o
	gcc -o teste TrabalhoED1.o main.o

F_TrabalhoED1.c: TrabalhoED1.c TrabalhoED1.h
	gcc -c TrabalhoED1.c -Wall -Werror -Wextra

main.o: main.c TrabalhoED1.h
	gcc -c main.c -Wall -Werror -Wextra

clear:
	rm -rf *.o teste