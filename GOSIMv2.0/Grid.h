#ifndef GRID_H_INCLUDED
#define GRID_H_INCLUDED

#include "Matrix.h"

///------------------------------------------------------------------------------

//-----------------------------------------------------------------------------

//I need a 4-dimensional matrix
namespace Numeric_lib {


template<class T> class Matrix<T,4> : public Matrix_base<T> {
    const Index d1;
    const Index d2;
    const Index d3;
    const Index d4;

protected:
    // for use by Row:
    Matrix(Index n1, Index n2, Index n3, Index n4, T* p) : Matrix_base<T>(n1*n2*n3*n4,p), d1(n1), d2(n2), d3(n3), d4(n4)
    {
        // std::cerr << "construct 3D Matrix from data\n";
    }

public:

    int length;
    int width;
    int height;
    int varnum;

    Matrix(Index n1, Index n2, Index n3, Index n4) : Matrix_base<T>(n1*n2*n3*n4), d1(n1), d2(n2), d3(n3), d4(n4),
    length(n1), width(n2), height(n3), varnum(n4) { }


    // copy constructor: let the base do the copy:
    Matrix(const Matrix& a) : Matrix_base<T>(a.size(),0), d1(a.d1), d2(a.d2), d3(a.d3), d4(a.d4)
    {
        // std::cerr << "copy ctor\n";
        this->base_copy(a);
    }

    template<int n1, int n2, int n3, int n4>
    Matrix(const T (&a)[n1][n2][n3][n4]) : Matrix_base<T>(n1*n2), d1(n1), d2(n2), d3(n3), d4(n4)
        // deduce "n1", "n2", "n3" (and "T"), Matrix_base allocates T[n1*n2*n3]
    {
        // std::cerr << "matrix ctor\n";
        for (Index i = 0; i<n1; ++i)
            for (Index j = 0; j<n2; ++j)
                for (Index k = 0; k<n3; ++k)
                    for (Index m = 0; m<n4; ++m)
                    this->elem[i*n2*n3*n4+j*n3*n4+k*n4+m]=a[i][j][k][m];
    }

    template<class F> Matrix(const Matrix& a, F f) : Matrix_base<T>(a.size()), d1(a.d1), d2(a.d2), d3(a.d3), d4(a.d4)
        // construct a new Matrix with element's that are functions of a's elements:
        // does not modify a unless f has been specifically programmed to modify its argument
        // T f(const T&) would be a typical type for f
    {
        for (Index i = 0; i<this->sz; ++i) this->elem[i] = f(a.elem[i]);
    }

    template<class F, class Arg> Matrix(const Matrix& a, F f, const Arg& t1) : Matrix_base<T>(a.size()), d1(a.d1), d2(a.d2), d3(a.d3), d4(a.d4)
        // construct a new Matrix with element's that are functions of a's elements:
        // does not modify a unless f has been specifically programmed to modify its argument
        // T f(const T&, const Arg&) would be a typical type for f
    {
        for (Index i = 0; i<this->sz; ++i) this->elem[i] = f(a.elem[i],t1);
    }

    Matrix& operator=(const Matrix& a)
        // copy assignment: let the base do the copy
    {
        // std::cerr << "copy assignment (" << this->size() << ',' << a.size()<< ")\n";
        if (d1!=a.d1 || d2!=a.d2 || d3!=a.d3 || d4!=a.d4) error("length error in 3D =");
        this->base_assign(a);
        return *this;
    }

    ~Matrix() { }

    Index dim1() const { return d1; }    // number of elements in a row
    Index dim2() const { return d2; }    // number of elements in a column
    Index dim3() const { return d3; }    // number of elements in a depth
    Index dim4() const { return d4; }    // My new dimension

    Matrix xfer()    // make an Matrix to move elements out of a scope
    {
        Matrix x(dim1(),dim2(),dim3(),dim4(),this->data()); // make a descriptor
        this->base_xfer(x);            // transfer (temporary) ownership to x
        return x;
    }

    void range_check(Index n1, Index n2, Index n3, Index n4) const
    {
        // std::cerr << "range check: (" << d1 << "," << d2 << "): " << n1 << " " << n2 << "\n";
        if (n1<0 || d1<=n1) error("4D range error: dimension 1");
        if (n2<0 || d2<=n2) error("4D range error: dimension 2");
        if (n3<0 || d3<=n3) error("4D range error: dimension 3");
        if (n4<0 || d4<=n4) error("4D range error: dimension 4");
    }

    // subscripting:
          T& operator()(Index n1, Index n2, Index n3, Index n4)       { range_check(n1,n2,n3,n4); return this->elem[d2*d3*d4*n1+d3*d4*n2+d4*n3+n4]; };
    const T& operator()(Index n1, Index n2, Index n3, Index n4) const { range_check(n1,n2,n3,n4); return this->elem[d2*d3*d4*n1+d3*d4*n2+d4*n3+n4]; };


    // Column<T,2> column(Index n); // not (yet) implemented: requies strides and operations on columns

    // element-wise operations:
    template<class F> Matrix& apply(F f)            { this->base_apply(f);   return *this; }
    template<class F> Matrix& apply(F f,const T& c) { this->base_apply(f,c); return *this; }

    Matrix& operator=(const T& c)  { this->base_apply(Assign<T>(),c);       return *this; }

    Matrix& operator*=(const T& c) { this->base_apply(Mul_assign<T>(),c);   return *this; }
    Matrix& operator/=(const T& c) { this->base_apply(Div_assign<T>(),c);   return *this; }
    Matrix& operator%=(const T& c) { this->base_apply(Mod_assign<T>(),c);   return *this; }
    Matrix& operator+=(const T& c) { this->base_apply(Add_assign<T>(),c);   return *this; }
    Matrix& operator-=(const T& c) { this->base_apply(Minus_assign<T>(),c); return *this; }

    Matrix& operator&=(const T& c) { this->base_apply(And_assign<T>(),c);   return *this; }
    Matrix& operator|=(const T& c) { this->base_apply(Or_assign<T>(),c);    return *this; }
    Matrix& operator^=(const T& c) { this->base_apply(Xor_assign<T>(),c);   return *this; }

    Matrix operator!() { return xfer(Matrix(*this,Not<T>())); }
    Matrix operator-() { return xfer(Matrix(*this,Unary_minus<T>())); }
    Matrix operator~() { return xfer(Matrix(*this,Complement<T>()));  }

    template<class F> Matrix apply_new(F f) { return xfer(Matrix(*this,f)); }

};

}


//Use a simplified notation -- Grid
typedef Numeric_lib::Matrix<float, 4> Grid;
typedef Numeric_lib::Matrix<float, 2> Grid2D;



#endif // GRID_H_INCLUDED
