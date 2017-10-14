#ifndef MATRIX_H
#define MATRIX_H

#include <vector>
#include <iostream>
#include <assert.h>
#include <cstdlib> // for rand() and srand()
#include <iomanip>

namespace McdaAlg {};
namespace McdaAlg
{

class Matrix
{
private:
    typedef std::vector<double> Alternative;
    // folded vector
    std::vector<Alternative> decisionMtx;
    int m_rows;
    int m_columns;
public:
 //constructors
    Matrix()
    {
    };
    Matrix(int alter,int crit);

// access method
    double& at (int i, int j);

//fill up a matrix with random numbers generated in a given range
    void random(int min, int max);

// method for printing out a matrix
    void print();
//get the size of matrix, transmitting desired dimensionality
    int size(int dim) const;

//overload operator (+) using friend function to gain possibility to apply to 2 objects
    friend Matrix operator+( Matrix mtx1, Matrix mtx2);
    friend Matrix operator-(Matrix mtx1, Matrix mtx2);
    friend Matrix operator*(Matrix mtx1, Matrix mtx2);
    friend Matrix operator*(double a, Matrix mtx);
    friend Matrix operator/(Matrix mtx, double a);
    friend Matrix operator/(Matrix mtx1, Matrix mtx2);
};

}

#endif // MATRIX_H
