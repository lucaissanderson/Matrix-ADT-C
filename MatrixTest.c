/****************************
  MatrixTest.c
  Lucais Sanderson
  lzsander
  **************************/

#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include "Matrix.h"

int main(void){
    // assuring construct/destruct passes valgrind
    Matrix m = newMatrix(4);
    Matrix m2 = newMatrix(4);
    Matrix m3 = newMatrix(2);
    freeMatrix(&m);

    // access/manipulation functions
    m = newMatrix(4);
    assert(size(m)==4);
    assert(NNZ(m)==0);
    changeEntry(m, 1, 2, 15);
    changeEntry(m, 1, 1, 20);
    changeEntry(m, 2, 2, 30);
    changeEntry(m, 4, 4, 20);
    assert(NNZ(m)==4);
    changeEntry(m2, 1, 2, 15);
    changeEntry(m2, 1, 1, 20);
    changeEntry(m2, 2, 2, 30);
    changeEntry(m2, 4, 4, 20);
    assert(equals(m, m2));
    printMatrix(stdout, m);
    printf("\n");
    changeEntry(m, 1, 1, 0);
    changeEntry(m, 2, 2, 0);
    changeEntry(m, 1, 2, 0);
    changeEntry(m, 3, 3, 10.1);
    changeEntry(m, 2, 1, 5.5);
    changeEntry(m3, 1, 1, 5.5);
    changeEntry(m3, 2, 1, 5.5);
    changeEntry(m3, 2, 1, 5.5);

    assert(NNZ(m)==3);
    printMatrix(stdout, m);
    printf("\n");
    assert(!equals(m,m3));
    assert(!equals(m,m2));

    makeZero(m);
    printf("%d\n", NNZ(m));
    printMatrix(stdout, m);
    printf("\n");

    freeMatrix(&m);
    freeMatrix(&m2);
    freeMatrix(&m3);

    // arithmetic ops
    m = newMatrix(10);
    changeEntry(m, 1, 2, 10);
    changeEntry(m, 1, 5, 11);
    changeEntry(m, 1, 8, 1);
    changeEntry(m, 3, 9, 5);
    changeEntry(m, 3, 1, 20);
    changeEntry(m, 5, 9, 100);
    changeEntry(m, 5, 9, 1);
    changeEntry(m, 7, 6, 3);
    changeEntry(m, 7, 5, 3);
    changeEntry(m, 3, 2, 2);
    changeEntry(m, 3, 8, 2);
    changeEntry(m, 8, 3, 10);
    m2 = copy(m);
    assert(equals(m,m2));

    m3 = transpose(m);

    printMatrix(stdout, m);
    printf("\n");
    printMatrix(stdout, m3);
    printf("\n");

    freeMatrix(&m2);
    m2 = scalarMult(2.0, m3);

    printMatrix(stdout, m3);
    printf("\n");
    printMatrix(stdout, m2);
    printf("\n");

    freeMatrix(&m);
    m = sum(m2,m3);
    printMatrix(stdout, m);
    printf("\n");

    freeMatrix(&m);
    freeMatrix(&m3);
    m = transpose(m2);
    printf("A:\n");
    printMatrix(stdout, m2);
    printf("\nA^t\n");
    printMatrix(stdout, m);
    m3 = sum(m, m2);
    printf("\nA + A^t:\n");
    printMatrix(stdout, m3);
    printf("\n");
    freeMatrix(&m3);
    m3 = diff(m2,m);
    printf("A - A^t\n");
    printMatrix(stdout, m3);
    printf("\n");

    // matrix product
    freeMatrix(&m);
    freeMatrix(&m2);
    freeMatrix(&m3);

    m = newMatrix(2);
    m2 = newMatrix(2);

    changeEntry(m, 1, 2, 2.0);
    changeEntry(m, 2, 1, 1.0);
    changeEntry(m, 2, 2, 3.0);
    changeEntry(m2, 1, 1, 3.0);
    changeEntry(m2, 2, 1, 4.0);
    changeEntry(m2, 2, 2, 5.0);

    printf("A:\n");
    printMatrix(stdout, m);
    printf("\nB:\n");
    printMatrix(stdout, m2);
    printf("\n");

    m3 = product(m, m2);
    printf("A * B\n");
    printMatrix(stdout, m3);
    printf("\n");


    freeMatrix(&m);
    freeMatrix(&m2);
    freeMatrix(&m3);

    // tests from exmaples!!
   int n=100000;
   Matrix A = newMatrix(n);
   Matrix B = newMatrix(n);
   Matrix C, D, E, F, G, H;

   changeEntry(A, 1,1,1); changeEntry(B, 1,1,1);
   changeEntry(A, 1,2,2); changeEntry(B, 1,2,0);
   changeEntry(A, 1,3,3); changeEntry(B, 1,3,1);
   changeEntry(A, 2,1,4); changeEntry(B, 2,1,0);
   changeEntry(A, 2,2,5); changeEntry(B, 2,2,1);
   changeEntry(A, 2,3,6); changeEntry(B, 2,3,0);
   changeEntry(A, 3,1,7); changeEntry(B, 3,1,1);
   changeEntry(A, 3,2,8); changeEntry(B, 3,2,1);
   changeEntry(A, 3,3,9); changeEntry(B, 3,3,1);

   printf("%d\n", NNZ(A));
   printMatrix(stdout, A);
   printf("\n");

   printf("%d\n", NNZ(B));
   printMatrix(stdout, B);
   printf("\n");

   C = scalarMult(1.5, A);
   printf("%d\n", NNZ(C));
   printMatrix(stdout, C);
   printf("\n");

   D = sum(A, B);
   printf("%d\n", NNZ(D));
   printMatrix(stdout, D);
   printf("\n");

   E = diff(A, A);
   printf("%d\n", NNZ(E));
   printMatrix(stdout, E);
   printf("\n");

   F = transpose(B);
   printf("%d\n", NNZ(F));
   printMatrix(stdout, F);
   printf("\n");

   G = product(B, B);
   printf("%d\n", NNZ(G));
   printMatrix(stdout, G);
   printf("\n");

   H = copy(A);
   printf("%d\n", NNZ(H));
   printMatrix(stdout, H);
   printf("\n");

   printf("%s\n", equals(A, H)?"true":"false" );
   printf("%s\n", equals(A, B)?"true":"false" );
   printf("%s\n", equals(A, A)?"true":"false" );

   makeZero(A);
   printf("%d\n", NNZ(A));
   printMatrix(stdout, A);

   freeMatrix(&A);
   freeMatrix(&B);
   freeMatrix(&C);
   freeMatrix(&D);
   freeMatrix(&E);
   freeMatrix(&F);
   freeMatrix(&G);
   freeMatrix(&H);

    return 0;
}


/*
1: (1, 20.0) (2, 15.0) 
2: (2, 30.0) 
4: (4, 20.0) 

2: (1, 5.5) 
3: (3, 10.1) 
4: (4, 20.0) 

0

1: (2, 10.0) (5, 11.0) (8, 1.0) 
3: (1, 20.0) (2, 2.0) (8, 2.0) (9, 5.0) 
5: (9, 1.0) 
7: (5, 3.0) (6, 3.0) 
8: (3, 10.0) 

1: (3, 20.0) 
2: (1, 10.0) (3, 2.0) 
3: (8, 10.0) 
5: (1, 11.0) (7, 3.0) 
6: (7, 3.0) 
8: (1, 1.0) (3, 2.0) 
9: (3, 5.0) (5, 1.0) 

1: (3, 20.0) 
2: (1, 10.0) (3, 2.0) 
3: (8, 10.0) 
5: (1, 11.0) (7, 3.0) 
6: (7, 3.0) 
8: (1, 1.0) (3, 2.0) 
9: (3, 5.0) (5, 1.0) 

1: (3, 40.0) 
2: (1, 20.0) (3, 4.0) 
3: (8, 20.0) 
5: (1, 22.0) (7, 6.0) 
6: (7, 6.0) 
8: (1, 2.0) (3, 4.0) 
9: (3, 10.0) (5, 2.0) 

1: (3, 60.0) 
2: (1, 30.0) (3, 6.0) 
3: (8, 30.0) 
5: (1, 33.0) (7, 9.0) 
6: (7, 9.0) 
8: (1, 3.0) (3, 6.0) 
9: (3, 15.0) (5, 3.0) 

A:
1: (3, 40.0) 
2: (1, 20.0) (3, 4.0) 
3: (8, 20.0) 
5: (1, 22.0) (7, 6.0) 
6: (7, 6.0) 
8: (1, 2.0) (3, 4.0) 
9: (3, 10.0) (5, 2.0) 

A^t
1: (2, 20.0) (5, 22.0) (8, 2.0) 
3: (1, 40.0) (2, 4.0) (8, 4.0) (9, 10.0) 
5: (9, 2.0) 
7: (5, 6.0) (6, 6.0) 
8: (3, 20.0) 

A + A^t:
1: (2, 20.0) (3, 40.0) (5, 22.0) (8, 2.0) 
2: (1, 20.0) (3, 4.0) 
3: (1, 40.0) (2, 4.0) (8, 24.0) (9, 10.0) 
5: (1, 22.0) (7, 6.0) (9, 2.0) 
6: (7, 6.0) 
7: (5, 6.0) (6, 6.0) 
8: (1, 2.0) (3, 24.0) 
9: (3, 10.0) (5, 2.0) 

A - A^t
1: (2, -20.0) (3, 40.0) (5, -22.0) (8, -2.0) 
2: (1, 20.0) (3, 4.0) 
3: (1, -40.0) (2, -4.0) (8, 16.0) (9, -10.0) 
5: (1, 22.0) (7, 6.0) (9, -2.0) 
6: (7, 6.0) 
7: (5, -6.0) (6, -6.0) 
8: (1, 2.0) (3, -16.0) 
9: (3, 10.0) (5, 2.0) 

A:
1: (2, 2.0) 
2: (1, 1.0) (2, 3.0) 

B:
1: (1, 3.0) 
2: (1, 4.0) (2, 5.0) 

A * B
1: (1, 8.0) (2, 10.0) 
2: (1, 15.0) (2, 15.0) 

9
1: (1, 1.0) (2, 2.0) (3, 3.0) 
2: (1, 4.0) (2, 5.0) (3, 6.0) 
3: (1, 7.0) (2, 8.0) (3, 9.0) 

6
1: (1, 1.0) (3, 1.0) 
2: (2, 1.0) 
3: (1, 1.0) (2, 1.0) (3, 1.0) 

9
1: (1, 1.5) (2, 3.0) (3, 4.5) 
2: (1, 6.0) (2, 7.5) (3, 9.0) 
3: (1, 10.5) (2, 12.0) (3, 13.5) 

9
1: (1, 2.0) (2, 2.0) (3, 4.0) 
2: (1, 4.0) (2, 6.0) (3, 6.0) 
3: (1, 8.0) (2, 9.0) (3, 10.0) 

0

6
1: (1, 1.0) (3, 1.0) 
2: (2, 1.0) (3, 1.0) 
3: (1, 1.0) (3, 1.0) 

7
1: (1, 2.0) (2, 1.0) (3, 2.0) 
2: (2, 1.0) 
3: (1, 2.0) (2, 2.0) (3, 2.0) 

9
1: (1, 1.0) (2, 2.0) (3, 3.0) 
2: (1, 4.0) (2, 5.0) (3, 6.0) 
3: (1, 7.0) (2, 8.0) (3, 9.0) 

true
false
true
0
*/
