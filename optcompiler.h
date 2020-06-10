#ifndef __OPTCOMPILER_H__
#define __OPTCOMPILER_H__

struct Entry {
    int Times;
    char Operation;

    struct Entry* next;
};

void libbfc_compile_bf_opt(char* bfcode, int bufsize, char* output, int gcc);

#endif