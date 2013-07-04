#ifndef MATRIX_H_INCLUDED
#define MATRIX_H_INCLUDED
#include <vector>
#include <iostream>
#include <sstream>
#include <time.h>
#include <stdlib.h>
using namespace std;
template <class T>
string to_str ( T Number )
{
    ostringstream ss;
    ss << Number;
    return ss.str();
}

enum MType{ZEROS=0,ONES,EYE};
/*
 * class for working with Matrixes
 */
template <class T>
class Matrix
{
    private:
        vector<T> array;
        string operErr;
        int n;
        int m;
    public:
        Matrix(int _n=1, int _m=1,MType type=ZEROS);
        ~Matrix();
        T& operator () (int i,int j=0);
        T operator () (int i,int j) const;
        Matrix<T> transpose() const;
        Matrix<T> eachEqual(T (*func)(T elem)) const;
        void randomize(T minVal,T maxVal);
        int getWidth() const;
        int getHeight() const;

};
/*
 * multiply Matrix
 */
template<class T>
ostream& operator << (ostream& out, const Matrix<T> &A);
template<class T>
istream& operator >> (istream& in, const Matrix<T> &A);
template <class T>
Matrix<T> operator * (T k,const Matrix<T> &A);
template <class T>
Matrix<T> operator * (const Matrix<T> &A, T k);
template <class T>
Matrix<T> operator * (const Matrix<T> &A, const Matrix<T> &B);
template <class T>
Matrix<T> operator + (const Matrix<T> &A, const Matrix<T> &B);
template <class T>
Matrix<T> operator - (const Matrix<T> &A, const Matrix<T> &B);

/*
 * create Matrix
 @param _n - number of rows
 @param _m - number of columns
 */
template <class T>
Matrix<T>::Matrix(int _n,int _m, MType type)
{
    n=_n;
    m=_m;
    switch(type)
    {
        case ZEROS:
            array.assign(n*m,0);
            break;
        case ONES:
            array.assign(n*m,1);
            break;
        case EYE:
            array.assign(n*m,0);
            for (int i=0; i<min(n,m); i++)
            {
                array[n*i+i]=1;
            }
            break;
        default:
            string err="In function Matrix::Matrix(): Type of matrix is out of range.\n";
            throw err;
            break;
    }
}


template <class T>
Matrix<T>::~Matrix()
{
    n=0;
    m=0;
}
/*
 * return link to element of matrix on the position (i,j). Indexing starts from zero
 */
template <class T>
T& Matrix<T>::operator () (int i, int j)
{
    //cerr <<"not const\n";
    if (i>=n || j>=m)
    {
        string err="In function Matrix::set(): Error. Position of value is out of range.";
        throw err;
    }
    return array[n*i+j];
}

/*
 * return value of element of matrix on the position (i,j). Indexing starts from zero
 */
template <class T>
T Matrix<T>::operator () (int i, int j) const
{

    //cerr <<"const\n";
    if (i>=n || j>=m)
    {
        string err="In class Matrix operator (): Error. Position of value is out of range.";
        throw err;
    }
    return array[n*i+j];
}


/*
 * return transposed Matrix
 */
template <class T>
Matrix<T> Matrix<T>::transpose() const
{
    Matrix<T> ans(m,n);
    for (int i=0; i<m; i++)
        for (int j=0; j<n; j++)
            ans(i,j)=(*this)(j,i);
    return ans;
}
/*
 * assign each element of resulting matirx to func of each element of current matrix
 */
template <class T>
Matrix<T> Matrix<T>::eachEqual(T (*func)(T elem)) const
{
    Matrix<T> ans(m,n);
    for (int i=0; i<n; i++)
        for (int j=0; j<m; j++)
            ans(i,j)=func((*this)(i,j));
    return ans;
}
/*
 * assign random value in range [minVal,maxVal] to each element of matrix
 */
 template <class T>
 void Matrix<T>::randomize(T minVal, T maxVal)
 {
    srand(time(NULL));
    for (int i=0; i<n; i++)
        for (int j=0; j<m; j++)
            (*this)(i,j)=T(double(rand())/RAND_MAX*(maxVal-minVal))+minVal;
 }
/*
 * return number of rows
 */
template <class T>
int Matrix<T>::getHeight() const
{
    return n;
}

/*
 * return number of columns
 */
template <class T>
int Matrix<T>::getWidth() const
{
    return m;
}

/*
 * print Matrix into the stream
 @param out - output stream
 @param A - matrix to be printed
 */
template <class T>
ostream& operator << (ostream& out, const Matrix<T>& A)
{
    out << A.getHeight() << " " << A.getWidth() << endl;
    for (int i=0; i<A.getHeight(); i++)
    {
        for (int j=0; j<A.getWidth(); j++)
            out << fixed << A(i,j) << " ";
        out << endl;
    }
    return out;
}
template <class T>
istream& operator >> (istream& in, Matrix<T>& A)
{
    for (int i=0; i<A.getHeight(); i++)
    {
        for (int j=0; j<A.getWidth(); j++)
            in >> A(i,j);
    }
}
/*
 * add other Matrix to current Matrix
 */
template <class T>
Matrix<T> operator + (const Matrix<T> &A, const Matrix<T> &B)
{
    if (B.getHeight()!=A.getHeight() || B.getWidth()!=A.getWidth())
    {
        string err="in class Matrix operator +: nonconformat arguments";
        err+="(op1 is "+to_str(A.getHeight())+"x"+to_str(B.getWidth())+", op2 is "+to_str(B.getHeight())+"x"+to_str(B.getWidth())+") ";
        throw err;
    }
    Matrix<T> res (A.getHeight(),A.getWidth());
    for (int i=0; i<A.getHeight(); i++)
        for (int j=0; j<A.getWidth(); j++)
            res(i,j)=A(i,j)+B(i,j);
    return res;
}

/*
 * subtract other Matrix from current Matrix
 */
template <class T>
Matrix<T> operator - (const Matrix<T> &A, const Matrix<T> &B)
{
    if (B.getHeight()!=A.getHeight() || B.getWidth()!=A.getWidth())
    {
        string err="in Matrix operator -: nonconformat arguments";
        err+="(op1 is "+to_str(A.getHeight())+"x"+to_str(B.getWidth())+", op2 is "+to_str(B.getHeight())+"x"+to_str(B.getWidth())+") ";
        throw err;
    }
    Matrix<T> res (A.getHeight(),A.getWidth());
    for (int i=0; i<A.getHeight(); i++)
        for (int j=0; j<A.getWidth(); j++)
            res(i,j)=A(i,j)-B(i,j);
    return res;
}

/*
 * multiply two Matrixes
 @param A - Matrix [n,m1]
 @param B - Matrix [m2,k]
 @return Matrix [n,k] = A*B or through exception
 */
template <class T>
Matrix<T> operator * (const Matrix<T> &A, const Matrix<T> &B)
{
    int n=A.getHeight();
    int m=A.getWidth();
    int k=B.getWidth();
    if (m!=B.getHeight())
    {
        string err="in class Matrix operator *: nonconformat arguments";
        err+="(op1 is "+to_str(n)+"x"+to_str(m)+", op2 is "+to_str(B.getHeight())+"x"+to_str(k)+") ";
        throw err;
    }
    Matrix<T> res(n,k);
    for (int i=0; i<n; i++)
        for (int j=0; j<k; j++)
        {
            T sum=0;
            for (int z=0; z<m; z++)
            {
                 sum+=A(i,z)*B(z,j);
            }
            res(i,j)=sum;
        }
    return res;
}

/*
 * multiply scalar on Matrix
 */
template <class T>
Matrix<T> operator * (T k, const Matrix<T> &A)
{
    int n=A.getHeight();
    int m=A.getWidth();
    Matrix<T> res (n,m);
    for (int i=0; i<n; i++)
    {
        for (int j=0; j<m; j++)
        {
            res(i,j)=A(i,j)*k;
        }
    }
    return res;
}

/*
 * multiply Matrix on scalar
 */
template <class T>
Matrix<T> operator * (const Matrix<T> &A, T k)
{
    return k*A;
}


#endif // Matrix_H_INCLUDED
