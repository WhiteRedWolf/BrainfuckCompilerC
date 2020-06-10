#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "generalcompiler.h"
#include "optcompiler.h"

char* ReadFile(char* filename){
    FILE* handle = fopen(filename, "rt");

    assert(handle);

    fseek(handle, 0, SEEK_END);
    long length = ftell(handle);

    fseek(handle, 0, SEEK_SET);

    char* buffer = (char*)malloc(length + 1);
    buffer[length] = '\0';

    fread(buffer, 1, length, handle);
    fclose(handle);

    return buffer;
}

int main(int argc, char** argv){
    if(argc < 5){
        printf("Usage: \n./compiler-linux <filename> <output file> <bufsize (auto)> <optimize code? (o/n)> <compile with gcc? (y/n)>\n");
        printf("Example: \n./compiler-linux program.bf out auto o y\n");
        exit(0);
    }
    int bufsize;
    int binary = 0;
    //Parse Commandline Args
    char* input = ReadFile(argv[1]);
    char* output = argv[2];

    if(!strcmp(argv[5],"y")){
        binary = 1;
    }

    if(!strcmp(argv[3], "auto")){
        printf("Determening Automatic Buffer Size..\n");
        size_t count_left = 0;
        while(*input) if (*input++ == '<') ++count_left;
        size_t count_right = 0;
        while(*input) if (*input++ == '>') ++count_right;

        bufsize = (count_left - count_right) + 89;
        printf("Automatic Buffer size: %d\n",bufsize);
    }else {
        bufsize = atoi(argv[3]);
    }

    input = ReadFile(argv[1]);
    if(!strcmp(argv[4], "n")){
        printf("Starting Brainfuck Compilation using the Standard Setting...\n");
        libbfc_compile_bf(input, bufsize, output, binary);
    }
    else if(!strcmp(argv[4], "o")){
        printf("Starting Brainfuck Compilation using the Optimized Setting...\n");
        libbfc_compile_bf_opt(input,bufsize, output, binary);

    }
    
}