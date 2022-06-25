#include <stdlib.h>

#include "include/args.h"
#include "include/bmp_parser.h"
#include "include/payload.h"
#include "include/controller.h"
#include <string.h>

int main(int argc , char *argv[]) {
    parse_args(argc, argv);
    struct args *args = get_args();
//    struct args aux_args = {1, "./assets/ss-tp5.pdf", "./assets/kings.bmp", "ss_oculto.bmp", "LSB4", "aes256", "OFB", "pass" };
//    struct args aux_args2 = {0, "./assets/valen.heic", "ss_oculto.bmp", "saliobienss", "LSB4", "des", "OFB", "pass" };

    if(args->embed_or_extract == 1){
        embed_using_args(*args);
    }else{
        extract_using_args(*args);
    }
    return 0;
}
