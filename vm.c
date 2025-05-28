#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

//Name : Erik Mayes (Solo)
//Class: 3402 System Software 
//Semester: Summer 2025
//Teacher: Euripides Montagne

#define MAX_ARRAY_SIZE 500
int bp;
int sp;
int pc = 10;
int pas[MAX_ARRAY_SIZE];
int eop = 1;

typedef struct IR
{
    int op; // Operation code
    int l;  // lexicographical level
    int m;  // Depedning on operations (number, program address, data address, arithmetic/relation operation)
} IR;

// Print function called after each instruction to output the results of the pmachine to the user
void printStack(struct IR ins, int pc, int bp, int sp, int stack [], int locals[]) 
{
    int bar = INT_MAX;
    int idx  = 0;
    if (locals[0] == -1) 
    {
        bar = locals[idx + 1];
    }
    //print out the IR L and M values
    printf("%d %d \t\t", ins.l, ins.m);
    //print the PC, BP and SP values
    printf("%d\t%d\t%d\t", pc, bp, sp);
    // print contents of stack
    for (int i = 499; i >= sp; i--) 
    {
        printf("%d  ", stack[i]);
        bar --;
        if (bar == 0) 
        {
            printf("| ");
            bar = locals[idx + 1];
        }
    }
    printf("\n");
}

int base( int BP, int L)
    {
        int arb = BP; // arb = activation record base
        while ( L > 0) //find base L levels down
        {
            arb = pas[arb];
            L--;
        }
        return arb;
    }

int main(int argc, char **argv) {

    // Initalize variables
    int sp = 500;
    int bp = sp - 1;
    int num;
    int inputSize = 0;
    int locals[20];
    int calls = 0;
    locals[0] = -1;
    IR ins;
    //set entire array stack to all 0's
    memset(pas, 0, sizeof(pas));

    // Check for proper input from user to run progream
    if(argc != 2) 
    {
        printf("Usage: \"<program> <input file>\n");
    }
    // Load instructions FILE from the second command line prompt
    FILE *instructions = fopen(argv[1], "r");
    // Check if instructions loaded properly, print an error if not
    if(!instructions) 
    {
        perror("input file not found.\n");
        exit(1);
    }
    // scan instructions FILE one number at a time and store the number in the stack "text" segment starting at index 10
    while (fscanf(instructions, "%d", &num) == 1) 
    {
        pas[pc++] = num;
        inputSize++;  
    }
    // reset PC to 10
    pc = 10;
    
    fclose(instructions);
    
    printf("\t\t\tPC\tBP\tSP\t\nInitial values: \t%d\t%d\t%d\n\n", pc, bp, sp);
    // cycle through "text" field or instruction set until all instructions have been processed
    for (int i = 0; i < inputSize + 1; i ++) 
    {
        // grab instructions 3 array index's at a time and store in the appropriate IR fields
        ins.op = pas[pc];
        ins.l  = pas[pc + 1];
        ins.m  = pas[pc + 2];
        //increment PC to point at next instruction
        pc = pc + 3;
    // based on opcode value execute the appropriate instruction
    switch(ins.op) 
    {
        case 1: // LIT put variable in ins.m on "top"  of stack in this implimentation the stack grows down towards 0 to adding will generally decriment the SP and popping will increase the SP.
            printf("LIT ");
            sp--;
            pas[sp]  = ins.m;
            break;
        case 2: // Return / arithmetic operations
            switch(ins.m) 
            {
                case 0: // return SP goes to ahead of base pointer and the BP grabs the stored dynamic link. PC counter grabs the stored return address.
                    printf("RTN ");
                    sp = bp + 1;
                    bp = pas[sp - 2];
                    pc = pas[sp - 3];
                    break;
                case 1: // Add. add the top 2 variables on the stack and pop
                    printf("ADD ");
                    pas[sp + 1] = pas[sp + 1] + pas[sp];
                    sp++;
                    break;
                case 2: // SUB. subtract the top 2 variables off the stack and pop
                    printf("SUB ");
                    pas[sp + 1] = pas[sp + 1] - pas[sp];
                    sp++;
                    break;
                case 3: // MUL. multiply top 2 variables and pop
                    printf("MUL ");
                    pas[sp + 1] = pas[sp + 1] * pas[sp];
                    sp++;
                    break;
                case 4: // DIV. divide top 2 variables and pop
                    printf("DIV ");
                    pas[sp + 1] = pas[sp + 1] / pas[sp];
                    sp++;
                    break;
                case 5: // EQL. compare if equal and pop
                    printf("EQL ");
                    pas[sp + 1] = pas[sp + 1] == pas[sp];
                    sp++;
                    break;
                case 6: // NEQ. compare if not equal and pop
                    printf("NEQ ");
                    pas[sp + 1] = pas[sp + 1] != pas[sp];
                    sp++;
                    break;
                case 7: // LSS. compare if lower if less the upper and pop
                    printf("LSS ");
                    pas[sp + 1] = pas[sp + 1] < pas[sp];
                    sp++;
                    break;
                case 8: // LEQ. compare if less than or equal and pop
                    printf("LEQ ");
                    pas[sp + 1] = pas[sp + 1] <= pas[sp];
                    sp++;
                    break;
                case 9: // GTR. compare if greater than and pop
                    printf("GTR ");
                    pas[sp + 1] = pas[sp + 1] > pas[sp];
                    sp++;
                    break;
                case 10: // GEQ. compare if greater than or equal and pop
                    printf("GEQ ");
                    pas[sp + 1] = pas[sp + 1] >= pas[sp];
                    sp ++;
            }
            break;
        case 3: // LOD (Load) add a spot and load from base offset - IR.m
            printf("LOD ");
            sp--;
            pas[sp] = pas[base(bp, ins.l) - ins.m]; // Load from the correct memory location
            break;
        case 4: // STO. Store value on top of stack at the off set of base - IR.m 
            printf("STO ");
            pas[base(bp, ins.l)- ins.m] = pas[sp];
            sp++;
            break;
        case 5: // Call, creating new AR store the static link, dynamic link and return address
            printf("CAL ");
            pas[sp - 1] = base(bp, ins.l); // static link
            pas[sp - 2] = bp; // dynamic link
            pas[sp - 3] = pc; // return address
            bp = sp - 1;
            pc = ins.m;
            break;
        case 6: // INC allocate locals. create space in the stack 
            printf("INC ");
            sp = sp - ins.m;
            if (locals[0] == -1) {
                locals[++calls] = ins.m - ins.l;
            }
            locals[0] = -1;
            break;
        case 7: // JUMP. jump to the address in IR.m
            printf("JMP ");
            pc = ins.m;
            break;
        case 8: // JPC jump conditionally. Jump to IR.m if the value in the stack[SP] is 0 always pop either way
            if(pas[sp] == 0) {
                pc = ins.m;
            }
            sp++;
            printf("JPC ");
            break;
        case 9: // SYS output and pop
            switch(ins.m) {
                case 1: // tell user the ouput of the call and pop
                    printf("Ouput result is : %d\n", pas[sp]);
                    printf("SYS ");
                    sp++;
                    break;
                case 2: // create space for an int and have the user type in the value to be stored there
                    sp--;
                    printf("Please enter an Integer: ");
                    scanf("%d", &pas[sp]);
                    printf("SYS ");
                    break;
                case 3: // halt program ********
                printf("SYS ");
                printStack(ins, pc, bp, sp, pas, locals);
                    return 1; // eop flag?
            }
        }
        printStack(ins, pc, bp, sp, pas, locals);
    }
}
