/***************************
*  pa4 : Matrix.c
*  Lucais Sanderson
*  lzsander
***************************/

#include <stdio.h>
#include <stdlib.h>
#include "List.h"
#include "Matrix.h"

// private helpers
void vectorSum(Matrix A, Matrix B, Matrix C, int i);
void vectorDiff(Matrix A, Matrix B, Matrix C, int i);
double vectorDot(List A, List B);

// structs -----------------------------------------------

// define private types
typedef struct EntryObj {
    // index at column-row(List)
    int col;
    // entry value
    double val;
} EntryObj;

typedef EntryObj* Entry;

// define matrix objects
typedef struct MatrixObj {
    int size;
    // non-zero elements
    int elements;
    // array of Lists (row)
    List *row;
} MatrixObj;

// Constructors-Destructors -----------------------------

// initialize an Entry (private)
Entry newEntry(int c, double v){
    Entry e = malloc(sizeof(struct EntryObj));
    if(e==NULL){
        fprintf(stderr, "Entry Error: newEntry: error creating Entry\n");
        exit(EXIT_FAILURE);
    }
    e->col = c;
    e->val = v;
    return e;
}

// free an Entry (private)
void freeEntry(Entry* pE){
    if(pE != NULL && *pE != NULL){
        free(*pE);
        *pE = NULL;
        free(pE);
        pE = NULL;
    }
}

// initialize matrix of size nxn
Matrix newMatrix(int n){
    Matrix M = malloc(sizeof(MatrixObj));
    M->row = calloc(n+1, sizeof(List));
    for(int i=1; i<=n; i++){
        M->row[i] = newList();
    }
    M->size = n;
    M->elements = 0;
    return M;
}

// free matrix
void freeMatrix(Matrix* pM){
    // check if M already free
    if(pM==NULL || *pM==NULL) return;
    // free every list and entry so long M not empty
    if(size(*pM)){
        // visit each list, free entries then list
        for(int i=1; i<=size(*pM); i++){
            // ensure list isn't empty
            if(length((*pM)->row[i])){
                moveFront((*pM)->row[i]);
                while(index((*pM)->row[i])>=0){
                    freeEntry((Entry*)get((*pM)->row[i]));
                    moveNext((*pM)->row[i]);
                }
            }
            freeList(&((*pM)->row[i]));
        }
    }
    freeList(&((*pM)->row[0]));
    free((*pM)->row);
    // free matrix obj
    free(*pM);
}

// access functions --------------------------------

// return size of nxn matrix
int size(Matrix M){
    return M->size;
}

//return number of non-zero elements
int NNZ(Matrix M){
    return M->elements;
}

// reset M to zero state
void makeZero(Matrix M){
    // visit each row
    for(int i=1; i<=size(M); i++){
        // skip if row empty
        if(!length(M->row[i])) continue;
        // visit and destroy each entry
        for(moveFront(M->row[i]);
            index(M->row[i])>=0;
            moveFront(M->row[i])){
            freeEntry((Entry*)get(M->row[i]));
            delete(M->row[i]);
        }
    }
    M->elements = 0;
}

// return 1 if equal, else 0
int equals(Matrix A, Matrix B){
    // check sizes
    if(size(A)!=size(B)) return 0;
    int eq = 1;
    for(int i=1; i<=size(A); i++){
        if(length(A->row[i])!=length(B->row[i])) return 0;
        for(moveFront(A->row[i]), moveFront(B->row[i]);
            index(A->row[i])>=0;
            moveNext(A->row[i]), moveNext(B->row[i])){
            if(((*(Entry*)get(A->row[i]))->col != (*(Entry*)get(B->row[i]))->col) ||
               ((*(Entry*)get(A->row[i]))->val != (*(Entry*)get(B->row[i]))->val)){
                return 0;
            }
        }
    }
    return eq;
}

// set/change entry of M at i,j with x
// pre: 1<=i<=size(M), 1<=j<=size(M)
void changeEntry(Matrix M, int i, int j, double x){
    if(i < 1 || i > size(M) || j < 1 || j > size(M)){
        fprintf(stderr, "Matrix Error: calling changeEntry() with invalid range\n");
        exit(EXIT_FAILURE);
    }
    // instantiate new entry
    Entry* pE = malloc(sizeof(Entry));
    *pE = newEntry(j, x);
    // if empty, just append entry
    if(!length(M->row[i])){
        if(x){
            append(M->row[i], pE);
            M->elements++;
            return;
        }
        freeEntry(pE);
        return;
    }
    // ensure correct ordering
    for(moveFront(M->row[i]);
        (*(Entry*)get(M->row[i]))->col < j && index(M->row[i]) < length(M->row[i])-1;
        moveNext(M->row[i]));
    // see if entry exists
    if((*(Entry*)get(M->row[i]))->col == j){
        freeEntry(pE);
        // check if setting entry to zero (maintain sparseness)
        if(!x){
            freeEntry((Entry*)get(M->row[i]));
            delete(M->row[i]);
            M->elements--;
            return;
        }
        (*(Entry*)get(M->row[i]))->val = x;
    } else if((*(Entry*)get(M->row[i]))->col > j){
        // if x is 0, don't add entry
        if(x){
            insertBefore(M->row[i], pE);
            M->elements++;
            return;
        }
        freeEntry(pE);
    } else if((*(Entry*)get(M->row[i]))->col < j){
        // if x is 0, don't add entry
        if(x){
            insertAfter(M->row[i], pE);
            M->elements++;
            return;
        }
        freeEntry(pE);
    }
}

// arithmetic operations ------------------------------------------

// return deep copy of M
Matrix copy(Matrix M){
    Matrix cM = newMatrix(size(M));
    for(int i=1; i<=size(M); i++){
        for(moveFront(M->row[i]); index(M->row[i])>=0; moveNext(M->row[i])){
            changeEntry(cM, i, (*(Entry*)get(M->row[i]))->col,
                        (*(Entry*)get(M->row[i]))->val);
        }
    }
    return cM;
}

// return transpose of M
Matrix transpose(Matrix M){
    Matrix tM = newMatrix(size(M));
    for(int i=1; i<=size(M); i++){
        for(moveFront(M->row[i]); index(M->row[i])>=0; moveNext(M->row[i])){
            changeEntry(tM, (*(Entry*)get(M->row[i]))->col, i,
                        (*(Entry*)get(M->row[i]))->val);
        }
    }
    return tM;
}

// scalar multiply M by x (xM)
Matrix scalarMult(double x, Matrix M){
    Matrix xM = newMatrix(size(M));
    // if x==0, return new empty matrix
    if(!x){
        return xM;
    }
    for(int i=1; i<=size(M); i++){
        for(moveFront(M->row[i]); index(M->row[i])>=0; moveNext(M->row[i])){
            changeEntry(xM, i, (*(Entry*)get(M->row[i]))->col,
                        x * (*(Entry*)get(M->row[i]))->val);
        }
    }
    return xM;
}

// BEGIN CITATION (NOT ORIGINAL WORK) ===================================
// Credit: pseudo from "smiles" on class discord. message link:
// https://discord.com/channels/1087620525024624652/1087621263125647410/1102676115685900360

// add matrix A and B
// pre: size(A)==size(B)
Matrix sum(Matrix A, Matrix B){
    if(size(A)!=size(B)){
        fprintf(stderr, "Matrix Error: calling sum() with inconsistent sizes\n");
        exit(EXIT_FAILURE);
    }
    if(A==B) return scalarMult(2.0, A);
    Matrix sM = newMatrix(size(A));
    for(int i=1; i<=size(A); i++){
        vectorSum(A, B, sM, i);
    }
    return sM;
}

// computes vector addition of ith row of A and B, set to C
void vectorSum(Matrix A, Matrix B, Matrix C, int i){
    moveFront(A->row[i]);
    moveFront(B->row[i]);
    // if A or B's row empty, just fill in C's with the other's
    if(!length(A->row[i])){
        while(index(B->row[i])>=0){
            changeEntry(C, i, (*(Entry*)get(B->row[i]))->col,
                        (*(Entry*)get(B->row[i]))->val);
            moveNext(B->row[i]);
        }
        return;
    }
    if(!length(B->row[i])){
        while(index(A->row[i])>=0){
            changeEntry(C, i, (*(Entry*)get(A->row[i]))->col,
                        (*(Entry*)get(A->row[i]))->val);
            moveNext(A->row[i]);
        }
        return;
    }
    // main loop
    while(index(A->row[i])>=0 && index(B->row[i])>=0){
        if((*(Entry*)get(A->row[i]))->col > (*(Entry*)get(B->row[i]))->col){
            changeEntry(C, i, (*(Entry*)get(B->row[i]))->col,
                        (*(Entry*)get(B->row[i]))->val);
            moveNext(B->row[i]);
        } else if((*(Entry*)get(A->row[i]))->col < (*(Entry*)get(B->row[i]))->col){
            changeEntry(C, i, (*(Entry*)get(A->row[i]))->col,
                        (*(Entry*)get(A->row[i]))->val);
            moveNext(A->row[i]);
        } else {
            //((*(Entry*)get(A->row[i]))->col == (*(Entry*)get(B->row[i]))->col){
            changeEntry(C, i, (*(Entry*)get(A->row[i]))->col,
                        (*(Entry*)get(A->row[i]))->val + (*(Entry*)get(B->row[i]))->val);
            moveNext(A->row[i]);
            moveNext(B->row[i]);
        }
    }
    // flush any remaining entries
    if(index(A->row[i])>=0){
        while(index(A->row[i])>=0){
            changeEntry(C, i, (*(Entry*)get(A->row[i]))->col,
                        (*(Entry*)get(A->row[i]))->val);
            moveNext(A->row[i]);
        }
    }
    if(index(B->row[i])>=0){
        while(index(B->row[i])>=0){
            changeEntry(C, i, (*(Entry*)get(B->row[i]))->col,
                        (*(Entry*)get(B->row[i]))->val);
            moveNext(B->row[i]);
        }
    }
}

// compute matrix differnce (A-B)
// pre: size(A)==size(B)
Matrix diff(Matrix A, Matrix B){
    if(size(A)!=size(B)){
        fprintf(stderr, "Matrix Error: calling diff() with inconsistent sizes\n");
        exit(EXIT_FAILURE);
    }
    Matrix sM = newMatrix(size(A));
    for(int i=1; i<=size(A); i++){
        vectorDiff(A, B, sM, i);
    }
    return sM;
}

// computes vector difference of ith row of A and B (A-B), set to C
void vectorDiff(Matrix A, Matrix B, Matrix C, int i){
    moveFront(A->row[i]);
    moveFront(B->row[i]);
    // if A or B's row empty, just fill in C's with the other's
    if(!length(A->row[i])){
        while(index(B->row[i])>=0){
            changeEntry(C, i, (*(Entry*)get(B->row[i]))->col,
                        -1.0 * (*(Entry*)get(B->row[i]))->val);
            moveNext(B->row[i]);
        }
        return;
    }
    if(!length(B->row[i])){
        while(index(A->row[i])>=0){
            changeEntry(C, i, (*(Entry*)get(A->row[i]))->col,
                        (*(Entry*)get(A->row[i]))->val);
            moveNext(A->row[i]);
        }
        return;
    }
    // main loop
    while(index(A->row[i])>=0 && index(B->row[i])>=0){
        if((*(Entry*)get(A->row[i]))->col > (*(Entry*)get(B->row[i]))->col){
            changeEntry(C, i, (*(Entry*)get(B->row[i]))->col,
                        -1.0 * (*(Entry*)get(B->row[i]))->val);
            moveNext(B->row[i]);
        } else if((*(Entry*)get(A->row[i]))->col < (*(Entry*)get(B->row[i]))->col){
            changeEntry(C, i, (*(Entry*)get(A->row[i]))->col,
                        (*(Entry*)get(A->row[i]))->val);
            moveNext(A->row[i]);
        } else {
            //((*(Entry*)get(A->row[i]))->col == (*(Entry*)get(B->row[i]))->col){
            changeEntry(C, i, (*(Entry*)get(A->row[i]))->col,
                        (*(Entry*)get(A->row[i]))->val - (*(Entry*)get(B->row[i]))->val);
            moveNext(A->row[i]);
            moveNext(B->row[i]);
        }
    }
    // flush any remaining entries
    if(index(A->row[i])>=0){
        while(index(A->row[i])>=0){
            changeEntry(C, i, (*(Entry*)get(A->row[i]))->col,
                        (*(Entry*)get(A->row[i]))->val);
            moveNext(A->row[i]);
        }
    }
    if(index(B->row[i])>=0){
        while(index(B->row[i])>=0){
            changeEntry(C, i, (*(Entry*)get(B->row[i]))->col,
                        -1.0 * (*(Entry*)get(B->row[i]))->val);
            moveNext(B->row[i]);
        }
    }
}
// END CITATION ====================================================

// compute dot product of vector A and B
double vectorDot(List A, List B){
    double sum = 0;
    moveFront(A);
    moveFront(B);
    while(index(A)>=0 && index(B)>=0){
        if((*(Entry*)get(A))->col < (*(Entry*)get(B))->col){
            moveNext(A);
        } else if((*(Entry*)get(A))->col > (*(Entry*)get(B))->col){
            moveNext(B);
        } else{
            sum += ( (*(Entry*)get(A))->val * (*(Entry*)get(B))->val );
            moveNext(A);
            moveNext(B);
        }
    }
    return sum;
}

// compute matrix product between two matrices: A and B
// pre: size(A)==size(B)
Matrix product(Matrix A, Matrix B){
    if(size(A)!=size(B)){
        fprintf(stderr, "Matrix Error: calling product() with inconsistent sizes\n");
        exit(EXIT_FAILURE);
    }
    // acquire traspose of B
    Matrix Bt = transpose(B);
    // new output matrix (product)
    Matrix C = newMatrix(size(A));
    for(int i=1; i<=size(A); i++){
        if(!length(A->row[i])) continue;
        for(int j=1; j<=size(A); j++){
            if(!length(Bt->row[j])) continue;
            double dot=vectorDot(A->row[i], Bt->row[j]);
            // only add entry to C if dot prod. not 0
            if(dot){
                changeEntry(C, i, j, dot);
            }
        }
    }
    freeMatrix(&Bt);
    return C;
}

// print the matrix
void printMatrix(FILE* out, Matrix M){
    for(int i=1; i<=size(M); i++){
        if(length(M->row[i])){
            fprintf(out, "%d: ", i);
            for(moveFront(M->row[i]);
                index(M->row[i])>=0;
                moveNext(M->row[i])){
                fprintf(out, "(%d, %.1f) ",
                        (*(Entry*)get(M->row[i]))->col,
                        (*(Entry*)get(M->row[i]))->val);
            }
            fprintf(out, "\n");
        }
    }
}
