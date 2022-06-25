#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "bmp_parser.h"
#include "payload.h"
#include "cipher.h"
#include "steganography.h"
#include "args.h"
#include <stdlib.h>

void embed_using_args(struct args args);
void extract_using_args(struct args args);

#endif
