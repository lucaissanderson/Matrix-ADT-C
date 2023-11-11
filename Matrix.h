
#ifndef MATRIX_H_INCLUDE_
#define MATRIX_H_INCLUDE_
#endif

// Exported types
typedef struct MatrixObj* Matrix;

// Constructors/Destructors
Matrix newMatrix(int n);
void freeMatrix(Matrix* pM);

// Access functions
int size(Matrix M);
int NNZ(Matrix M);
int equals(Matrix A, Matrix B);

// Manipulation procedures
void makeZero(Matrix M);
void changeEntry(Matrix M, int i, int j, double x);

// Arithmetic operations
Matrix copy(Matrix A);
Matrix transpose(Matrix A);
Matrix scalarMult(double x, Matrix A);
Matrix sum(Matrix A, Matrix B);
Matrix diff(Matrix A, Matrix B);
Matrix product(Matrix A, Matrix B);

// other functions
void printMatrix(FILE* out, Matrix M);
