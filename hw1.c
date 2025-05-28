#include <stdio.h>


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

int pas[500];
int num;
int i = 10;

while(fscanf(fp,"%d",&num) == 1) {
    pas[i++] = num;
}

for(int k = 10; k < i; k++) {
    printf("%d ", pas[k]);
}
}