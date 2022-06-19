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
}