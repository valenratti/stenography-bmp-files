#ifndef INPUT_OPTIONS_H
#define INPUT_OPTIONS_H

struct input_options {
    char* in; // file to be hidden
    char* p; // bmp file which will be used as carrier
    char* out; // name of the final bmp file
    char* steg; // estenography algorithm used
    char *a; // cypher algorithm used
    char *m; // cypher mode
    char *pass; // encryption password
};

#endif