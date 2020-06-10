#include "optcompiler.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Entry* AddEntry(struct Entry* head, struct Entry* insert){
    struct Entry* newnode = malloc(sizeof(struct Entry));
    newnode->Times = insert->Times;
    newnode->Operation = insert->Operation;
    newnode->next = NULL;
}

void libbfc_compile_bf_opt(char* bfcode, int bufsize, char* output, int gcc){
    FILE* out;

    if(gcc == 0){
        out = fopen(output, "w");
    }else {
        out = fopen("out.c", "w");
    }
    //strrev(bfcode);
    //For putc() and getc()
    fprintf(out, "#include <stdio.h>\n");

    //Main Function, Required for Compilation
    fprintf(out, "int main(){");
    //Brainfuck Memory Buffer
    fprintf(out, "unsigned char buf[%d];", bufsize);
    //Brainfuck Memory Pointer
    fprintf(out, "int ptr = 0;");
    
    //Optimizer

    struct Entry* LinkedList = NULL;

    char OldCharacter;
    int Count = 1;
    
    while(*bfcode){
        char c = *bfcode++;
        struct Entry* NewEntry = malloc(sizeof(struct Entry));

        if(c != OldCharacter){
            NewEntry->Operation = OldCharacter;
            NewEntry->Times = Count;
            NewEntry->next = LinkedList;

            AddEntry(LinkedList, NewEntry);
            LinkedList = NewEntry;

            Count = 1;
        }else {
            Count++;
            
        }
        OldCharacter = c;
    }

    
    //Reverse List
    struct Entry* prevNode, *curNode;

    if(LinkedList != NULL){
        prevNode = LinkedList;
        curNode = LinkedList->next;
        LinkedList = LinkedList->next;

        prevNode->next = NULL;

        while(LinkedList != NULL) {
            LinkedList = LinkedList->next;
            curNode->next = prevNode;

            prevNode = curNode;
            curNode = LinkedList;
        }

        LinkedList = prevNode;
    }
    
    {
        struct Entry* temp = LinkedList;
        while(temp != NULL){
            if(temp->Operation > 31){
                
                switch(temp->Operation){
                    case '+':
                        fprintf(out, "buf[ptr]+=%d;",temp->Times);
                        break;
                    case '-':
                        fprintf(out, "buf[ptr]-=%d;",temp->Times);
                        break;
                    case '>':
                        fprintf(out, "ptr+=%d;",temp->Times);
                        break;
                    case '<':
                        fprintf(out, "ptr-=%d;",temp->Times);
                        break;
                    case '[':
                        for(int i = 0; i != temp->Times; i++){
                            fprintf(out, "while(buf[ptr] != 0){");
                        }
                        break;
                    case ']':
                        for(int i = 0; i != temp->Times; i++){
                            fprintf(out, "}");
                        }
                        break;
                    case '.':
                        for(int i = 0; i != temp->Times; i++){
                            fprintf(out, "putchar(buf[ptr]);");
                        }
                        break;
                    case ',':
                        for(int i = 0; i != temp->Times; i++){
                            fprintf(out, "buf[ptr] = getchar();");
                        }
                        break;
                }
            }
            //if(temp->next!=NULL)
                temp = temp->next;
        }
    }
    
    fprintf(out, "}");
    fclose(out);

    if(gcc == 1){
        printf("\nStarting Compilation into a linux binary..\n");
        char* command = malloc(64 + 16);
        sprintf(command, "gcc out.c -o %s -O3", output);
        //printf("command: %s", command);
        system(command);
        system("rm out.c");
    }
}