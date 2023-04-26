#include "../include/Matrix.hpp"

Matrix::Matrix(int rows, int columns)
    : m_rows(rows), m_columns(columns)
{
    m_data = new float[m_rows*m_columns];
    for (int i = 0; i < m_rows; i++)
    {
        for (int j = 0; j < m_columns; j++)
        {
            (*this)(i, j) = j+i*m_rows;
        }
    }
}

float& Matrix::operator()(int row, int col)
{
    return m_data[row*m_columns+col];
}

const float& Matrix::operator()(int row, int col) const
{
    return m_data[row*m_columns+col];
}

Matrix Matrix::operator*(const Matrix& other) const
{
    if (m_columns != other.m_rows)
    {
        throw std::invalid_argument("Matrix dimensions are not compatible");
    }
    Matrix result(m_rows, other.m_columns);
    for (int i = 0; i < m_rows; i++)
    {
        for (int j = 0; j < other.m_columns; j++)
        {
            float sum = 0;
            for (int k = 0; k < m_columns; k++)
            {
                sum += (*this)(i, k) * other(k, j);
            }
            result(i, j) = sum;
        }
    }
    return result;
}

Vector Matrix::operator*(const Vector& v) const
{
    Vector result;

    result.x = (*this)(0, 0) * v.x + (*this)(0, 1) * v.y;
    result.y = (*this)(1, 0) * v.x + (*this)(1, 1) * v.y;
    
    return result;
}

int Matrix::rows() const
{
    return m_rows;
}

int Matrix::columns() const
{
    return m_columns;
}

std::ostream& operator<<(std::ostream& os, const Matrix& m)
{
    for (int i = 0; i < m.m_columns; i++)
    {
        printf("| ");
        for (int j = 0; j < m.m_rows; j++)
        {
            printf("%f ", m.m_data[j+m.m_rows*i]);
        }
        printf("|\n");
    }
	return os;
}

Matrix::~Matrix()
{
    delete m_data;
}