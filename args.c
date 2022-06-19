#include <stdio.h>     /* for printf */
#include <stdlib.h>    /* for exit */
#include <limits.h>    /* LONG_MIN et al */
#include <string.h>    /* memset */
#include <errno.h>
#include <getopt.h>

#include "include/args.h"

struct args args; /* struct con la configuracion */

static void
usage(const char *progname) {
    fprintf(stderr,
            "Uso: %s [OPTION]...\n"
            "\n"
            "   -h                  Imprime la ayuda y termina.\n"
            "   -[extract | embed]  Indica si se va a extraer u ocultar información.\n"
            "   -in <file>          Archivo que se va a ocultar.\n"
            "   -p <bitmap file>    Archivo bmp que será el portador.\n"
            "   -out <bitmap file>  Archivo bmp de salida, es decir, el archivo bitmapfile con la información de file incrustada.\n"
            "   -steg <LSB1 | LSB4 | LSBI>              Algoritmo de esteganografiado: LSB de 1bit, LSB de 4 bits, LSB Enhanced."
            "   -a <aes128 | aes192 | aes256 | des>     Algoritmo de encripción.\n"
            "   -m <ecb | cfb | ofb | cbc>              Modo de encripción.\n"
            "   -pass password                          Password de encripción.\n"
            "\n",
            progname);
    exit(1);
}

void
parse_args(const int argc, char **argv) {
    struct args *a = get_args();

    memset(a, 0, sizeof(*a));

    int c;

    while (true) {
        int option_index = 0;
        static struct option long_options[] = {
                { "embed",      no_argument,       0, 0xD001 },
                { "extract",    no_argument,       0, 0xD002 },
                { "in",         required_argument, 0, 0xD003 },
                { "out",        required_argument, 0, 0xD004 },
                { "steg",       no_argument,       0, 0xD005 },
                { "pass",       required_argument, 0, 0xD006 },
                { 0,            0,                 0, 0      }
        };

        c = getopt_long(argc, argv, "p:a:m:", long_options, &option_index);

        if (c == -1)
            break;

        switch (c) {
            case 'h':
                usage(argv[0]);
                break;
            case 'p':
                strcpy(a->bitmap_file, optarg);
                break;
            case 'a':
                strcpy(a->cipher_algorithm, optarg);
                break;
            case 'm':
                strcpy(a->mode, optarg);
                break;
            case 0xD001:    // embed
                if(a->embed_or_extract != 0) {
                    fprintf(stderr, "Utilizar flag -embed o -extract, pero no ambos.\n");
                    exit(1);
                }
                a->embed_or_extract = 1;
                break;
            case 0xD002:    // extract
                a->embed_or_extract = 2;
                break;
            case 0xD003:    // in
                strcpy(a->in_file, optarg);
                break;
            case 0xD004:    // out
                strcpy(a->out_file, optarg);
                break;
            case 0xD005:    // steg
                strcpy(a->steg_algorithm, optarg);
                break;
            case 0xD006:    // pass
                strcpy(a->password, optarg);
                break;
            default:
                fprintf(stderr, "Argumento desconocido %d.\n", c);
                exit(1);
        }

        if(a->embed_or_extract == 0) {
            fprintf(stderr, "Utilizar el flag -embed o -extract para indicar si se desea ocultar o extraer información.\n");
            exit(1);
        }
    }
    if (optind < argc) {
        fprintf(stderr, "Argumento inválido: ");
        while (optind < argc) {
            fprintf(stderr, "%s ", argv[optind++]);
        }
        fprintf(stderr, "\n");
        exit(1);
    }
}

struct args *get_args(){
    return &args;
}