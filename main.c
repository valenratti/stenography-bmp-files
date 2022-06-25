#include <stdlib.h>

#include "include/args.h"
#include "include/bmp_parser.h"
#include "include/payload.h"
#include "include/controller.h"
#include <string.h>

int main(int argc , char *argv[]) {
    parse_args(argc, argv);
    struct args *args = get_args();
    struct args aux_args = {1, "./assets/valen.txt", "./assets/kings.bmp", "test.bmp", "LSB1", "nothing", "nothing", "" };
    struct args aux_args2 = {0, "./assets/valen.heic", "test.bmp", "salida", "LSB1", "nothing", "nothing", "" };

    if(aux_args2.embed_or_extract == 1){
        embed_using_args(aux_args);
    }else{
        extract_using_args(aux_args2);
    }
    return 0;
}
