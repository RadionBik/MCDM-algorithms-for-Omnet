#ifndef MATRIXFUNCTIONS_H_INCLUDED
#define MATRIXFUNCTIONS_H_INCLUDED

#include "Matrix.h" //otherwise this won't work
#include <math.h>

namespace McdaAlg {};
namespace McdaAlg
{

Matrix enterMtx();

//swap given a 2 indexes of rows of a matrix
void swapRows(Matrix &mtx, int row1, int row2,int col);

//obtain the rank of a matrix
int rankOfMatrix(Matrix mtx);

//calculate determinant of a square matrix
double determinant(Matrix a, int n);

//	calculate cofactor of matrix
void coFactor(Matrix a,int n, Matrix &b);

Matrix transpose(Matrix a);

//	calculate inverse of matrix
Matrix inv(Matrix a);

//generate a matrix into 1
Matrix ones(int m, int n);

//nullify all elements but diagonal
Matrix diag(Matrix a);

//generate identity matrix
Matrix eye(int n);

//find the max element of a matrix in a given row  or column (1)
int maxIndex(Matrix a, int place, std::string dim);

//find the min element of a matrix in a given row (2) or column (1)
int minIndex(Matrix a, int place, std::string dim);

//find the sum of elements of a matrix in a given row  or column (1)
double sum(Matrix a, int place, std::string dim);

//find the max element of a matrix in a given row  or column (1)
double maxElement(Matrix a, int place, std::string dim);

//find the min element of a matrix in a given row  or column
double minElement(Matrix a, int place, std::string dim);

//find the index of a second max element of a matrix in a given row  or column (1)
int secondMaxIndex(Matrix a, int place, std::string dim);

} //end of namespace McdaAlg

#endif // MATRIXFUNCTIONS_H_INCLUDED
//
