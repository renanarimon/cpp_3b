#pragma once
#include<iostream>
#include <vector>
#include <exception>
namespace zich{
    class Matrix
    {
    private:
        std::vector<double> _mat;
        int _row;
        int _col;

        friend std::istream & operator >> (std::istream& in,  Matrix& mat); //cin
        friend std::ostream& operator<<(std::ostream& out, const Matrix& mat); //cout
        
        friend Matrix operator*(double scalar, const Matrix& mat); //mat2 = scalar*mat1
        friend Matrix operator*=(double scalar, Matrix& other); //mat1 = scalar*mat1

    public:
        Matrix(std::vector<double> mat, int row, int col); //constructor
        Matrix(const Matrix& other); // copy constructor
        ~Matrix(); //distructor

        int getRow()const{return this->_row;}
        int getCol()const{return this->_col;}
        std::vector<double> getMat()const{return this->_mat;}

        Matrix operator+(const Matrix& other); // mat = this+other
        Matrix operator+=(const Matrix& other); // this = this+other
        Matrix operator+(); // unary: this = +(this)

        Matrix operator-(const Matrix& other);// mat = this-other
        Matrix operator-=(const Matrix& other);// this = this-other
        Matrix operator-();// unary: mat = -(this)


        bool operator>(const Matrix& other) const; //this > other --> true
        bool operator<(const Matrix& other) const; //this < other --> true
        bool operator>=(const Matrix& other) const; //this >= other --> true
        bool operator<=(const Matrix& other) const; //this <= other --> true
        bool operator==(const Matrix& other) const; //this == other --> true
        bool operator!=(const Matrix& other) const; //this != other --> true

        Matrix operator++(); // ++this
        Matrix operator++(int); // this++
        Matrix operator--(); // --this
        Matrix operator--(int); // this--

        Matrix operator*(double scalar) const; // mat = this*scalar
        Matrix operator*(const Matrix& other) const; // mat = this*other
        Matrix operator*=(double scalar); // this = this*scalar
        Matrix operator*=(const Matrix& other); // this = this*other

        std::vector<std::vector<double>> turnVec2D() const;

    };

    
}
