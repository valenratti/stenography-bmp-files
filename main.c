#include <stdlib.h>

#include "include/args.h"
#include "include/bmp_parser.h"
#include "include/payload.h"
#include "include/controller.h"
#include <string.h>

int main(int argc , char *argv[]) {
    // if(argc < 5) {
    //     fprintf(stderr, "Cantidad inválida de argumentos. Imprima la ayuda con -h.\n");
    //     exit(1);
    // }

    // parse_args(argc, argv);
    // struct args *args = get_args();
//    struct args aux_args = {1, "./assets/ss-tp5.pdf", "./assets/kings.bmp", "ss_oculto.bmp", "LSB4", "aes256", "OFB", "pass" };
   struct args aux_args2 = {0, "", "ss_oculto.bmp", "outt", "LSB4", "", "", "" };
    // strcpy(args->steg_algorithm, "LSBI");
    if(aux_args2.embed_or_extract == 1){
        embed_using_args(aux_args2);
    }else{
        extract_using_args(aux_args2);
    }
    return 0;
}
