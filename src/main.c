/* Grupo 12
    Luiz Felipe Pereira Nolasco - 2025102338
    Gessica de Souza Silva - 2022102174
*/

#include "kernel.h"
#include "io.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <input_file>\n", argv[0]);
        return EXIT_FAILURE;
    }
    
    FILE *file = fopen(argv[1], "r");
    if (!file) {
        perror("Failed to open input file");
        return EXIT_FAILURE;
    }
    
    int quantum = 500;
    int n_cpus = 1;  // padrão: monoprocessador

#ifdef MULTI
    n_cpus = 2; //  para multiprocessador
#endif

    Kernel *kernel = read_processes(file, quantum, n_cpus);
    fclose(file);

    if (!kernel) {
        fprintf(stderr, "Failed to read processes from file.\n");
        return EXIT_FAILURE;
    }

    kernel_simulate_cpus(kernel);
    destroy_kernel(kernel);
    
    return 0;
}
