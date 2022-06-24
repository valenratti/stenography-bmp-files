<<<<<<< HEAD
#include <stdio.h>

int main(){
    printf("hola");
=======
#include <stdlib.h>

#include "include/args.h"

int main(int argc , char *argv[]) {

    parse_args(argc, argv);
    struct args *args = get_args();

    args->a = "aes128";
    args->m = "cbc";
    args->pass = "lautaro";

    // encrypt
    // print
    // decrypt
    // print

    return 0;
>>>>>>> d18771aaf0b49d75dc230cac2d4de94107cbb1fc
}