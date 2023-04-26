#ifndef MATRIX_HPP
#define MATRIX_HPP

#include "Vector.hpp"
#include <iostream>

class Matrix
{
    public:
        Matrix(int rows, int columns);
        ~Matrix();

        float& operator()(int row, int col);
        const float& operator()(int row, int col) const;

        Matrix operator*(const Matrix& other) const;
        Vector operator*(const Vector& v) const;

        int rows() const;
        int columns() const;

		friend std::ostream& operator<<(std::ostream& os, const Matrix& m);

    private:
        int m_rows;
        int m_columns;
        float* m_data;
};

#endif