/***************************
*  pa4 : Sparce.c
*  Lucais Sanderson
*  lzsander
***************************/

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "Matrix.h"

extern int errno;

int main(int argc, char** argv){

    FILE* in;
    FILE* out;
    // size, NNZ, and intermittent row col and value
    int size, NZ_A, NZ_B, i, j, count=0, status; 
    double v;

    // check number of args
    if(argc != 3){
        fprintf(stderr, "Usage: %s <input file> <output file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    in = fopen(argv[1], "r");
    out = fopen(argv[2], "w");

    // check file errors
    if(in == NULL) {
        fprintf(stderr, "Unable to open file %s\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    if(out == NULL) {
        fprintf(stderr, "Unable to open file %s\n", argv[2]);
        exit(EXIT_FAILURE);
    }

    // get first line for size of A, B as well as NNZ for each
    if(fscanf(in, "%d %d %d\n\n", &size, &NZ_A, &NZ_B) != 3){
        fprintf(stderr, "Error reading matrix size and NNZ entries (first line not formatted correctly)\n");
        exit(EXIT_FAILURE);
    }

    Matrix A = newMatrix(size);
    Matrix B = newMatrix(size);

    // get A's entries
    while(count < NZ_A && (status=fscanf(in, "%d %d %lf\n", &i, &j, &v))!=EOF){
        changeEntry(A, i, j, v);
        count++;
    }

    count = 0;

    // process empty line between entry sets
    fscanf(in, "\n");

    // get B's entries
    while(count < NZ_B && (status=fscanf(in, "%d %d %lf\n", &i, &j, &v))!=EOF){
        changeEntry(B, i, j, v);
        count++;
    }

    // print A and B
    fprintf(out, "A has %d non-zero entries:\n", NNZ(A));
    printMatrix(out, A);
    fprintf(out, "\n");
    fprintf(out, "B has %d non-zero entries:\n", NNZ(B));
    printMatrix(out, B);
    fprintf(out, "\n");

    // (1.5)*A:
    Matrix C = scalarMult(1.5, A);
    fprintf(out, "(1.5)*A =\n");
    printMatrix(out, C);
    fprintf(out, "\n");

    // A+B:
    Matrix D = sum(A,B);
    fprintf(out, "A+B =\n");
    printMatrix(out, D);
    fprintf(out, "\n");

    // A+A:
    Matrix E = sum(A,A);
    fprintf(out, "A+A =\n");
    printMatrix(out, E);
    fprintf(out, "\n");

    // B-A:
    Matrix F = diff(B,A);
    fprintf(out, "B-A =\n");
    printMatrix(out, F);
    fprintf(out, "\n");

    // A-A:
    Matrix G = diff(A,A);
    fprintf(out, "A-A =\n");
    printMatrix(out, G);
    fprintf(out, "\n");

    // transpose(A)
    Matrix H = transpose(A);
    fprintf(out, "Transpose(A) =\n");
    printMatrix(out, H);
    fprintf(out, "\n");

    // AB:
    Matrix I = product(A,B);
    fprintf(out, "A*B =\n");
    printMatrix(out, I);
    fprintf(out, "\n");

    // B^2:
    Matrix J = product(B,B);
    fprintf(out, "B*B =\n");
    printMatrix(out, J);
    fprintf(out, "\n");

    freeMatrix(&A);
    freeMatrix(&B);
    freeMatrix(&C);
    freeMatrix(&D);
    freeMatrix(&E);
    freeMatrix(&F);
    freeMatrix(&G);
    freeMatrix(&H);
    freeMatrix(&I);
    freeMatrix(&J);

    fclose(in);
    fclose(out);

    return 0;
}
