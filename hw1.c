#include <stdio.h>


typedef struct IR{
    int Op;
    int L;
    int M;
} IR;


int main(int argc, char **argv) {
printf("start");
if(argc != 2) {
    printf("Usage <./hw1><input#.txt>");
    return 1;
}
char **input = argv[1];
FILE *File;
int stack[500];
File = fopen(&input, "r");
if(File == NULL) {
    perror("invalid file name");
    return 2;
}
printf("end");
}