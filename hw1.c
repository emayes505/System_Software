#include <stdio.h>

int BP; // Base Pointer 
int SP; // Stack Pointer
int PC; // Program Counter
int PAS[500]; // Program Address Space 0-9 unused then 10+ for text/instructions then 499 and down is stack

// Instruction Register
typedef struct IR{ 
    int Op;
    int L;
    int M;
} IR;

int main(int argc, char **argv) {

if(argc != 2) {
    printf("Usage <./hw1><input#.txt>");
    return 1;
}

FILE *fp = fopen(argv[1], "r");

if(fp == NULL) {
    perror("Error opening file");
    return 2;
}

int num;
int i = 10;

while(fscanf(fp,"%d",&num) == 1) {
    pas[i++] = num;
}

for(int k = 10; k < i; k++) {
    printf("%d ", pas[k]);
}
}