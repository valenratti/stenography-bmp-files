#include <stdlib.h>

#include "include/args.h"
#include "include/bmp_parser.h"
#include "include/payload.h"
#include "include/controller.h"
#include <string.h>

int main(int argc , char *argv[]) {
    parse_args(argc, argv);
    struct args *args = get_args();
    struct args aux_args = {1, "./assets/valen.heic", "./assets/titanic1.bmp", "test.bmp", "LSB4", "nothing", "nothing", "" };

    if(aux_args.embed_or_extract == 1){
        embed_using_args(aux_args);
    }else{
//        extract_using_args(*args);
    }
    return 0;
}
