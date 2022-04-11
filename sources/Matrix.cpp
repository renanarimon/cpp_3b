#include "Matrix.hpp"
#include <numeric>
#include <iostream>
#include <limits>
#include <exception>
namespace zich
{
    static void checkInput(const std::vector<double> &mat, int row, int col)
    {
        if (row < 0 || col < 0)
        {
            throw std::invalid_argument("matrix size must be positive");
        }
        if (mat.size() != row * col)
        {
            throw std::invalid_argument("matrix size dosen't match col*row");
        }
    }

    Matrix::Matrix(std::vector<double> mat, int row = 1, int col = 1)
    {
        checkInput(mat, row, col);
        std::vector<double> v;
        copy(mat.begin(), mat.end(), back_inserter(v));
        this->_mat = v;
        this->_row = row;
        this->_col = col;
    }

    Matrix::Matrix(const Matrix &other)
    {
        std::vector<double> v;
        copy(other._mat.begin(), other._mat.end(), back_inserter(v));
        this->_mat = v;
        this->_row = other._row;
        this->_col = other._col;
    }
    Matrix::~Matrix() {}

    // math_operators
    static void checkSize(const Matrix &origin, const Matrix &other)
    {
        if (origin.getRow() != other.getRow() || origin.getCol() != other.getCol())
        {
            throw std::invalid_argument("math operators allow only on equal size matrixs");
        }
    }

    Matrix Matrix::operator+(const Matrix &other)
    {
        checkSize(*this, other);
        std::vector<double> v;
        for (size_t i = 0; i < (this->_col * this->_row); i++)
        {
            v.push_back(this->_mat[i] + other._mat[i]);
        }
        Matrix ans(v, this->_row, this->_col);
        return ans;
    }
    Matrix Matrix::operator+=(const Matrix &other)
    {
        checkSize(*this, other);
        for (size_t i = 0; i < (this->_col * this->_row); i++)
        {
            this->_mat[i] += other._mat[i];
        }

        return *this;
    }
    Matrix Matrix::operator+()
    {
        return Matrix(*this);
    }

    Matrix Matrix::operator-(const Matrix &other)
    {
        checkSize(*this, other);
        std::vector<double> v;
        for (size_t i = 0; i < (this->_col * this->_row); i++)
        {
            v.push_back(this->_mat[i] - other._mat[i]);
        }
        return Matrix(v, this->_row, this->_col);
        ;
    }
    Matrix Matrix::operator-=(const Matrix &other)
    {
        checkSize(*this, other);
        for (size_t i = 0; i < (this->_col * this->_row); i++)
        {
            this->_mat[i] -= other._mat[i];
        }

        return *this;
    }
    Matrix Matrix::operator-()
    {
        std::vector<double> v;
        copy(this->_mat.begin(), this->_mat.end(), back_inserter(v));

        for (size_t i = 0; i < (this->_col * this->_row); i++)
        {
            if (v[i] != 0.0)
            {
                v[i] = v[i] * (-1);
            }
            else
            {
                v[i] = 0.0;
            }
        }
        return Matrix(v, this->_row, this->_col);
    }

    bool Matrix::operator>(const Matrix &other) const
    {
        checkSize(*this, other);
        return std::accumulate(this->_mat.begin(), this->_mat.end(), 0.0) > std::accumulate(other._mat.begin(), other._mat.end(), 0.0);
    }

    bool Matrix::operator<(const Matrix &other) const
    {
        checkSize(*this, other);
        return std::accumulate(this->_mat.begin(), this->_mat.end(), 0.0) < std::accumulate(other._mat.begin(), other._mat.end(), 0.0);
    }

    bool Matrix::operator>=(const Matrix &other) const
    {
        checkSize(*this, other);
        return std::accumulate(this->_mat.begin(), this->_mat.end(), 0.0) >= std::accumulate(other._mat.begin(), other._mat.end(), 0.0);
    }

    bool Matrix::operator<=(const Matrix &other) const
    {
        checkSize(*this, other);
        return std::accumulate(this->_mat.begin(), this->_mat.end(), 0.0) <= std::accumulate(other._mat.begin(), other._mat.end(), 0.0);
    }

    bool Matrix::operator==(const Matrix &other) const
    {
        checkSize(*this, other);
        return (
            this->_col == other._col &&
            this->_row == other._row &&
            this->_mat == other._mat);
    }
    bool Matrix::operator!=(const Matrix &other) const
    {
        return !(*this == other);
    }

    Matrix Matrix::operator++()
    { // prefix
        for (size_t i = 0; i < this->_col * this->_row; i++)
        {
            this->_mat[i]++;
        }
        return *this;
    }
    Matrix Matrix::operator++(int)
    { // postfix
        Matrix tmp(*this);
        ++*this;
        return tmp;
    }
    Matrix Matrix::operator--()
    { // prefix
        for (size_t i = 0; i < this->_col * this->_row; i++)
        {
            this->_mat[i]--;
        }
        return *this;
    }
    Matrix Matrix::operator--(int)
    { // postfix
        Matrix tmp(*this);
        --*this;
        return tmp;
    }

    static void checkMull(const Matrix &origin, const Matrix &other)
    {
        if (origin.getCol() != other.getRow())
        {
            throw std::invalid_argument("m*n x k*l: mull is ilegall if   n != k");
        }
    }

    Matrix Matrix::operator*(double scalar) const
    {
        std::vector<double> v;
        for (size_t i = 0; i < this->_col * this->_row; i++)
        {
            v.push_back((this->_mat[i]) * scalar);
        }
        return Matrix(v, this->_row, this->_col);
    }

    std::vector<std::vector<double>> Matrix::turnVec2D() const
    { // 1D vec -->2d
        size_t row = (size_t)this->_row;
        size_t col = (size_t)this->_col;
        std::vector<std::vector<double>> ans(row * col);
        ans.resize(row);
        for (size_t i = 0; i < row; i++)
        {
            ans[i].resize(col);
        }
        for (size_t j = 0; j < row * col; j++)
        {
            ans[j / col][j % col] = this->_mat[j];
        }
        return ans;
    }

    Matrix Matrix::operator*(const Matrix &other) const
    {
        checkMull(*this, other);
        size_t row = (size_t)this->_row;
        size_t col = (size_t)other._col;
        std::vector<std::vector<double>> v1 = this->turnVec2D();
        std::vector<std::vector<double>> v2 = other.turnVec2D();
        std::vector<std::vector<double>> ans(row * col);
        ans.resize(row);
        for (size_t i = 0; i < row; i++)
        {
            ans[i].resize(col);
        }

        for (size_t i = 0; i < row; i++)
        {
            for (size_t j = 0; j < col; j++)
            {
                ans[i][j] = 0;
                for (size_t k = 0; k < this->_col; k++)
                {
                    ans[i][j] += v1[i][k] * v2[k][j];
                }
            }
        }

        std::vector<double> v;
        for (size_t i = 0; i < row; i++)
        {
            for (size_t j = 0; j < col; j++)
            {
                v.push_back(ans[i][j]);
            }
        }
        return Matrix(v, row, col);
    }

    Matrix Matrix::operator*=(double scalar)
    {
        for (size_t i = 0; i < this->_col * this->_row; i++)
        {
            this->_mat[i] = (this->_mat[i]) * scalar;
        }
        return *this;
    }

    Matrix Matrix::operator*=(const Matrix &other)
    {
        checkMull(*this, other);
        Matrix tmp = (*this) * other;
        this->_mat.resize(tmp._mat.size());
        for (size_t i = 0; i < tmp._mat.size(); i++)
        {
            this->_mat[i] = tmp._mat[i];
        }
        this->_col = other._col;
        return *this;
    }

    Matrix operator*(double scalar, const Matrix &mat)
    {
        return mat * scalar;
    }

    Matrix operator*=(double scalar, Matrix &other) 
    {
        other *= scalar;
        return other;
    }

    std::ostream &operator<<(std::ostream &out, const Matrix &mat)
    {
        for (size_t i = 0, k = 0; i < mat._row; i++)
        {
            out << "[";
            for (size_t j = 0; j < mat._col; j++)
            {
                if (j == mat._col - 1)
                {
                    if (i != mat._row - 1)
                    {
                        out << mat._mat.at(k) << "]\n";
                    }
                    else
                    {
                        out << mat._mat.at(k);
                    }
                }
                else
                {
                    out << mat._mat.at(k) << " ";
                }
                k++;
            }
        }
        return out << "]";
    }

    static void checkCin(std::string st)
    {
        if (st.at(0) != '[' || st.at(st.size() - 1) != ']')
        {
            throw std::invalid_argument("invalid input");
        }
    }
    /*split string by another string
        param: st: string to split, splitBy: pattern to split by
        return: vector of splited string
    */
    static std::vector<std::string> split(std::string& st, const std::string& splitBy)
    {
        std::vector<std::string> words;
        size_t i = 0;
        while ((i = st.find(splitBy)) != std::string::npos)
        {
            words.push_back(st.substr(0, i));
            st.erase(0, i + splitBy.length());
        }
        words.push_back(st.substr(0, i));
        return words;
    }

    std::istream &operator>>(std::istream &in, Matrix &mat)
    {
        std::string st;
        std::getline(in, st); // [1 1 1 1], [1 1 1 1], [1 1 1 1]
        checkCin(st);
        std::vector<std::string> arrSplit = split(st, ", "); // {[1 1 1 1]} {[1 1 1 1]} {[1 1 1 1]}

        mat._row = arrSplit.size();
        mat._col = 0;

        std::vector<double> numbers; // all the parsed numbers

        for (const std::string& stRow : arrSplit)
        {
            checkCin(stRow);
            std::string arrSplitCol = stRow.substr(1, stRow.size() - 1); // 1 1 1 1
            std::vector<std::string> arrSplitEach = split(arrSplitCol, " ");         // {1, 1, 1, 1}
            if (mat._col != 0 && mat._col != arrSplitEach.size())
            {
                throw std::invalid_argument("cols size must be equal");
            }
            mat._col = arrSplitEach.size(); // update col size
            for (const std::string& s : arrSplitEach)
            {

                numbers.push_back(std::stod(s)); // parse end push each number (str --> double)
            }
        }
        mat._mat = numbers;
        return in;
    }
}