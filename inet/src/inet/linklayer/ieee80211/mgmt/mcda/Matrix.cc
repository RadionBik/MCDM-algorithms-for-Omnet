#include "Matrix.h"

/*

This files contains methods, overloaded operators and functions
    for Matrix class

*/
namespace McdaAlg {};
namespace McdaAlg
{
//constructor and init of 2d vector
Matrix::Matrix(int alter,int crit) : decisionMtx(alter,Alternative(crit,0))
{
    m_rows=alter;   //obtaining size of the input matrix
    m_columns=crit;
}
// access method
double& Matrix::at (int i, int j)
{
    return decisionMtx[i][j];
}

// method for printing out a matrix
void Matrix::print()
{
    for (int i=0; i<m_rows; i++)
    {
        for (int j=0; j<m_columns; j++)
        {
            std::cout<<std::setw(10)<<decisionMtx[i][j]; // <<"\t";
        }
        std::cout<<"\n";
    }
    std::cout<<"\n";
}

//get the size of matrix, transmitting desired dimensionality
int Matrix::size(int dim) const
{
    assert(dim<=2);
    if (dim==1)
        return m_rows;
    else
        return m_columns;
}

//fill up a matrix with random numbers generated in a given range
void Matrix::random(int min, int max)
{
    static const double fraction = 1.0 / (static_cast<double>(RAND_MAX) + 1.0);  // static used for efficiency, so we only calculate this value once
    for (int i=0; i<m_rows; i++)
        for (int j=0; j<m_columns; j++)
        {
            decisionMtx[i][j]=static_cast<int>(rand() * fraction * (max - min + 1) + min);
        }
}

//overload  operator (+)
Matrix operator+( Matrix mtx1,  Matrix mtx2)
{
    assert((mtx1.Matrix::size(1)-mtx2.Matrix::size(1))==0); // check whether number of rows is the same
    assert((mtx1.size(2)-mtx2.size(2))==0); // check whether number of columns is the same
    Matrix Mtx(mtx1.size(1),mtx1.size(2));
    for (int i=0; i<Mtx.size(1); i++)
    {
        for (int j=0; j<Mtx.size(2); j++)
        {
            Mtx.at(i,j)=mtx1.at(i,j)+mtx2.at(i,j);
        }
    }
    return Mtx;
}

//overload  operator (-)
Matrix operator-( Matrix mtx1,  Matrix mtx2)
{
    assert((mtx1.Matrix::size(1)-mtx2.Matrix::size(1))==0); // check whether number of rows is the same
    assert((mtx1.size(2)-mtx2.size(2))==0); // check whether number of columns is the same
    Matrix Mtx(mtx1.size(1),mtx1.size(2));
    for (int i=0; i<Mtx.size(1); i++)
    {
        for (int j=0; j<Mtx.size(2); j++)
        {
            Mtx.at(i,j)=mtx1.at(i,j)-mtx2.at(i,j);
        }
    }
    return Mtx;
}

//overload  operator (*)
Matrix operator*( Matrix mtx1,  Matrix mtx2)
{
    assert((mtx1.size(2)-mtx2.size(1))==0); // check whether number of columns of the 1 is = to number of rows of the 2
    Matrix Mtx(mtx1.size(1),mtx2.size(2));
    for (int i=0; i<Mtx.size(1); i++)           //rows of the 1
        for (int j=0; j<Mtx.size(2); j++)       //columns of the 2
            for(int k=0; k<mtx1.size(2); k++)      //columns of the 1
            {
                Mtx.at(i,j)+=mtx1.at(i,k)*mtx2.at(k,j);
            }
    return Mtx;
}

//overload  operator (*)
Matrix operator*( double a,  Matrix mtx)
{
    Matrix Mtx(mtx.size(1),mtx.size(2));
    for (int i=0; i<Mtx.size(1); i++)           //rows of the 1
        for (int j=0; j<Mtx.size(2); j++)       //columns of the 2
            Mtx.at(i,j) = a*mtx.at(i,j);
    return Mtx;
}

// here "/" operator is overloaded only when the 2 mtx is 1x1.
Matrix operator/(Matrix mtx1, Matrix mtx2)
{
    assert( (mtx2.size(1) == 1) && (mtx2.size(2) == 1));
    Matrix Mtx(mtx1.size(1),mtx1.size(2));
    for (int i=0; i<Mtx.size(1); i++)           //rows of the 1
        for (int j=0; j<Mtx.size(2); j++)       //columns of the 2
            Mtx.at(i,j) = mtx1.at(i,j)/ mtx2.at(0,0);
    return Mtx;
}

Matrix operator/(Matrix mtx, double a)
{
    Matrix Mtx(mtx.size(1),mtx.size(2));
    for (int i=0; i<Mtx.size(1); i++)           //rows of the 1
        for (int j=0; j<Mtx.size(2); j++)       //columns of the 2
            Mtx.at(i,j) = mtx.at(i,j)/a;
    return Mtx;
}

}
