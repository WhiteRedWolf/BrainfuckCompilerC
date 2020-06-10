#include "generalcompiler.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void libbfc_compile_bf(char* bfcode, int bufsize, char* output, int gcc){
    FILE* out = fopen(output, "w");

    //For putc() and getc()
    fprintf(out, "#include <stdio.h>\n");

    //Main Function, Required for Compilation
    fprintf(out, "int main(){");
    //Brainfuck Memory Buffer
    fprintf(out, "unsigned char buf[%d];", bufsize);
    //Brainfuck Memory Pointer
    fprintf(out, "int ptr = 0;");

    

    printf("Size: %ld", strlen(bfcode));
    
    while(*bfcode){
        char c = *bfcode++;

        //printf("i: %d; c: %c\n", i, c);

        switch(c) {
            case '>':
                fprintf(out, "ptr++;");
                break;
            case '<':
                fprintf(out, "ptr--;");
                break;
            case '+':
                fprintf(out, "buf[ptr]++;");
                break;
            case '-':
                fprintf(out, "buf[ptr]--;");
                break;
            case '.':
                fprintf(out, "putchar(buf[ptr]);");
                break;
            case ',':
                fprintf(out, "buf[ptr] = getchar();");
                break;
            case '[':
                fprintf(out, "while(buf[ptr] != 0){");
                break;
            case ']':
                fprintf(out, "}");
                break;
        }

        //i++;
        //sc = bfcode[i];
    }

    fprintf(out, "}");
    fclose(out);

    if(gcc == 1){
        printf("\nStarting Compilation into a linux binary..");
        char* command = malloc(64 + 16);
        sprintf(command, "gcc out.c -o %s -O3", output);
        system(command);
    }
}