#include "MatrixFunctions.h"

namespace McdaAlg {};
namespace McdaAlg
{
Matrix enterMtx()
{
    std::cout<<"Enter dimensionality m x n: \t";
    int m, n;
    std::cin>>m>>n;
    Matrix A(m,n);
    std::cout<<"\n------"<<n<<"x"<<m<<"----------------\n";
    for(int i=0; i<m; i++)
        for(int j=0; j<n; j++)
        {
            std::cout<<" Matrix["<<i+1<<"]["<<j+1<<"] : ";
            std::cin>>A.at(i,j);
        }
    std::cout<<"\n----------------------------------------------------\n";
    return A;
}

//swap given a 2 indexes of rows of a matrix
void swapRows(Matrix &mtx, int row1, int row2,int col)
{
    for(int i=0; i<col; i++)
    {
        int temp = mtx.at(row1,i);
        mtx.at(row1,i)=mtx.at(row2,i);
        mtx.at(row2,i)=temp;
    }
}

//obtain the rank of a matrix
int rankOfMatrix(Matrix mtx)
{
    int C=mtx.size(2);
    int R=mtx.size(1);
    int rank = C;

    for (int row=0; row<rank; row++)
    {
        // Before we visit current row 'row', we make
        // sure that mat[row][0],....mat[row][row-1]
        // are 0.

        // Diagonal element is not zero
        if (mtx.at(row,row))
        {
            for (int col=0; col<R; col++)
            {
                if(col!=row)
                {
                    // This makes all entries of current
                    // column as 0 except entry 'mat[row][row]'
                    double mult = mtx.at(col,row) / mtx.at(row,row);
                    for (int i = 0; i < rank; i++)
                        mtx.at(col,i) -= mult * mtx.at(row,i);
                }
            }
        }
        // Diagonal element is already zero. Two cases
        // arise:
        // 1) If there is a row below it with non-zero
        // entry, then swap this row with that row
        // and process that row
        // 2) If all elements in current column below
        // mat[r][row] are 0, then remvoe this column
        // by swapping it with last column and
        // reducing number of columns by 1.
        else
        {
            bool reduce = true;

            /* Find the non-zero element in current
            	column */
            for (int i = row + 1; i < R; i++)
            {
                // Swap the row with non-zero element
                // with this row.
                if (mtx.at(i,row))
                {
                    swapRows(mtx, row, i,rank);
                    reduce = false;
                    break ;
                }
            }
            // If we did not find any row with non-zero
            // element in current columnm, then all
            // values in this column are 0.
            if (reduce)
            {
                // Reduce number of columns
                rank--;

                // Copy the last column here
                for (int i = 0; i < R; i ++)
                    mtx.at(i,row) = mtx.at(i,rank);
            }

            // Process this row again
            row--;
        }
    }
    return rank;
}

//calculate determinant of a square matrix
double determinant(Matrix a, int n)
{
    int i,j,j1,j2;
    int rank=rankOfMatrix(a);
    double det = 0;
    Matrix m(a.size(1),a.size(2));

    if (rank < n)   /* Error */
    {
        det =0;
    }
    else if (n == 1)     /* Shouldn't get used */
    {
        det = a.at(0,0);
    }
    else if (n == 2)
    {
        det = a.at(0,0) * a.at(1,1) - a.at(1,0) * a.at(0,1);
    }
    else
    {
        det = 0;
        for (j1=0; j1<n; j1++)
        {
            for (i=1; i<n; i++)
            {
                j2 = 0;
                for (j=0; j<n; j++)
                {
                    if (j == j1)
                        continue;
                    m.at(i-1,j2) = a.at(i,j);
                    j2++;
                }
            }
            det += pow(-1.0,j1+2.0) * a.at(0,j1) * determinant(m,n-1);
        }
    }
    return(det);
}

//	calculate cofactor of matrix
void coFactor(Matrix a,int n, Matrix &b)
{
    int i,j,ii,jj,i1,j1;
    double det;
    Matrix c(a.size(1),a.size(2));
    for (j=0; j<n; j++)
    {
        for (i=0; i<n; i++)
        {

            /* Form the adjoint a_ij */
            i1 = 0;
            for (ii=0; ii<n; ii++)
            {
                if (ii == i)
                    continue;
                j1 = 0;
                for (jj=0; jj<n; jj++)
                {
                    if (jj == j)
                        continue;
                    c.at(i1,j1) = a.at(ii,jj);
                    j1++;
                }
                i1++;
            }

            /* Calculate the determinate */
            det = determinant(c,n-1);

            /* Fill in the elements of the cofactor */
            b.at(i,j) = pow(-1.0,i+j+2.0) * det;
        }
    }
}

Matrix transpose(Matrix a)
{
    int m = a.size(1);
    int n = a.size(2);
    Matrix transp(n,m);
    for (int i=0; i<m; i++)
    {
        for (int j=0; j<n; j++)
        {
            transp.at(j,i) = a.at(i,j);
            //a.at(j,i) = tmp;
        }
    }
    return transp;
}

//	calculate inverse of matrix
Matrix inv(Matrix a)
{
    double det;
    int n=a.size(1);
    assert(a.size(1)==a.size(2));
    //std::cout<<n<<"\n";
    det=determinant(a,n);
    //std::cout<<"Determinant is: "<<det<<"\n";
    assert(abs(det) >= 0.000001);
    Matrix b(a.size(1),a.size(2));  //complementary one
    coFactor(a,n, b);
    //b.print();           //calc matrix of cofactors
    if (n == 1)
    {
        b.at(0,0) = 1;
        return b;
    }
    else
    {
        Matrix b_tr(b.size(1),b.size(2));
        b_tr = transpose(b);
        for (int i=0; i<n; i++)
        {
            for (int j=0; j<n; j++)
            {
                b_tr.at(i,j)=static_cast<double> (b_tr.at(i,j)/det);    //normalize by det to obtain inv values
            }
        }
        return b_tr;
    }
}

//gener a matrix into 1
Matrix ones(int m, int n)
{
    assert((m>0) && (n>0));
    Matrix one (m,n);
    for (int i=0; i<m; i++)
        for (int j=0; j<n; j++)
        {
            one.at(i,j)=1;
        }
    return one;
}

//nullify all elements but diagonal
Matrix diag(Matrix a)
{
    int m = a.size(1);
    int n = a.size(2);
    for (int i=0; i<m; i++)
        for (int j=0; j<n; j++)
        {
            if (i!=j)
                a.at(i,j)=0;
        }
    return a;
}

//generate identity matrix
Matrix eye(int n)
{
    assert(n>0);
    Matrix a(n,n);
    for (int i=0; i<n; i++)
        for (int j=0; j<n; j++)
        {
            if (i==j)
                a.at(i,j)=1;
        }
    return a;
}

//find the max element of a matrix in a given row  or column (1)
int maxIndex(Matrix a, int place, std::string dim)
{
    assert( dim=="row" || dim=="column" );
    int n;
    int maxi{0};
    if (dim=="column")
    {
        n=a.size(1);
        for (int i=0; i<n;i++)
        {
            if (a.at(maxi,place) <= a.at(i,place))
                maxi=i;
        }
    }
    else if (dim=="row")
    {
        n=a.size(2);
        for (int i=0; i<n;i++)
        {
            if (a.at(place,maxi) <= a.at(place,i))
                maxi=i;
        }
    }
    return maxi;
}

//find the min element of a matrix in a given row (2) or column (1)
int minIndex(Matrix a, int place, std::string dim)
{
    assert( dim=="row" || dim=="column" );
    int n;
    int mini{0};
    if (dim=="column")
    {
        n=a.size(1);
        for (int i=0; i<n;i++)
        {
            if (a.at(mini,place) >= a.at(i,place))
                mini=i;
        }
    }
    else if (dim=="row")
    {
        n=a.size(2);
        for (int i=0; i<n;i++)
        {
            if (a.at(place,mini) >= a.at(place,i))
                mini=i;
        }
    }
    return mini;
}

//find the sum of elements of a matrix in a given row  or column (1)
double sum(Matrix a, int place, std::string dim)
{
    assert( dim=="row" || dim=="column" );
    int n;
    double sum{0};
    if (dim=="column")
    {
        n=a.size(1);
        for (int i=0; i<n;i++)
        {
            sum+=a.at(i,place);
        }
    }
    else if (dim=="row")
    {
        n=a.size(2);
        for (int i=0; i<n;i++)
        {
            sum+=a.at(place,i);
        }
    }
    return sum;
}

//find the max element of a matrix in a given row  or column (1)
double maxElement(Matrix a, int place, std::string dim)
{
    assert( dim=="row" || dim=="column" );
    int n;
    double Max {0.0};
    int maxi {0};
    if (dim=="column")
    {
        n=a.size(1);
        for (int i=0; i<n; i++)
        {
            if (a.at(maxi,place) <= a.at(i,place))
                maxi=i;
            Max=a.at(maxi,place);
        }
    }
    else if (dim=="row")
    {
        n=a.size(2);
        for (int i=0; i<n; i++)
        {
            if (a.at(place,maxi) <= a.at(place,i))
                maxi=i;
            Max=a.at(place,maxi);
        }
    }
    return Max;
}

//find the max element of a matrix in a given row  or column
double minElement(Matrix a, int place, std::string dim)
{
    assert( dim=="row" || dim=="column" );
    int n;
    double Min {0.0};
    int mini {0};
    if (dim=="column")
    {
        n=a.size(1);
        for (int i=0; i<n; i++)
        {
            if (a.at(mini,place) >= a.at(i,place))
                mini=i;
            Min=a.at(mini,place);
        }
    }
    else if (dim=="row")
    {
        n=a.size(2);
        for (int i=0; i<n; i++)
        {
            if (a.at(place,mini) >= a.at(place,i))
                mini=i;
            Min=a.at(place,mini);
        }
    }
    return Min;
}


//find the index of a second max element of a matrix in a given row  or column (1)
int secondMaxIndex(Matrix a, int place, std::string dim)
{
    assert( dim=="row" || dim=="column" );
    int n;
    int maxi {0};
    if (dim=="column")
    {
        n=a.size(1);
        a.at(maxIndex(a,place,"column") ,place) = -10e8; //make small the best one
        for (int i=0; i<n; i++)
        {
            if (a.at(maxi,place) <= a.at(i,place))
                maxi=i;                         //find the second best.
        }
    }
    else if (dim=="row")
    {
        n=a.size(2);
        a.at(place,maxIndex(a,place,"row") ) = -10e8;
        for (int i=0; i<n; i++)
        {
            if (a.at(place,maxi) <= a.at(place,i))
                maxi=i;
        }
    }
    return maxi;
}

} //end of namespace McdaAlg
