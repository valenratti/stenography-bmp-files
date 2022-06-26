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
    struct args aux_args = {1, "./assets/valen.txt", "./assets/sample.bmp", "ss_oculto_dom.bmp", "LSBI", "aes256", "OFB", "pass" };
   struct args aux_args2 = {0, "", "ss_oculto.bmp", "outtttt", "LSB4", "", "", "" };
   struct args decrypt_ss = {0, "", "ss_oculto_dom.bmp", "domingo", "LSBI", "aes256", "OFB", "pass"};
   // strcpy(args->steg_algorithm, "LSBI");
    if(decrypt_ss.embed_or_extract == 1){
        embed_using_args(aux_args);
    }else{
        extract_using_args(decrypt_ss);
    }
    return 0;
}
