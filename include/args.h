#ifndef ARGS_H
#define ARGS_H

#define MAX_FILENAME_LENGTH 64
#define MAX_LENGTH 32
#define MAX_PASS_LENGTH 512

struct args {
    char embed_or_extract;  /* 1 if embed, 2 if extract */
    char in_file[MAX_FILENAME_LENGTH];
    char bitmap_file[MAX_FILENAME_LENGTH];
    char out_file[MAX_FILENAME_LENGTH];
    char steg_algorithm[MAX_LENGTH];
    char cipher_algorithm[MAX_LENGTH];
    char mode[MAX_LENGTH];
    char password[MAX_PASS_LENGTH];
};

/**
 * Interpreta la linea de comandos (argc, argv) llenando
 * args con defaults o la seleccion humana. Puede cortar
 * la ejecucion.
 */
void
parse_args(const int argc, char **argv);

/**
 * Retorna un puntero a la estructura que contiene la configuracion.
 * */
struct args *get_args();

#endif //ARGS_H
