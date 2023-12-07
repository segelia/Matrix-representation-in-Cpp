#include "Matrix.h"

Matrix::Matrix(int rows, int cols) : rows(rows), cols(cols), refCount(new int(1))
{
    allocateMemory();
}

Matrix::Matrix(const Matrix &other) : rows(other.rows), cols(other.cols), data(other.data), refCount(other.refCount)
{
    increaseRefCount();
}

Matrix::~Matrix()
{
    decreaseRefCount();
}

Matrix &Matrix::operator=(const Matrix &other)
{
    if (this != &other)
    {
        decreaseRefCount();
        rows = other.rows;
        cols = other.cols;
        data = other.data;
        refCount = other.refCount;
        increaseRefCount();
    }
    return *this;
}

std::ostream &operator<<(std::ostream &os, const Matrix &matrix)
{
    for (int i = 0; i < matrix.rows; ++i)
    {
        for (int j = 0; j < matrix.cols; ++j)
        {
            os << matrix.data[i][j] << " ";
        }
        os << std::endl;
    }
    return os;
}

Matrix Matrix::operator+(const Matrix &other) const
{
    if (rows != other.rows || cols != other.cols)
    {
        throw MatrixError("Matrix dimensions do not match for addition");
    }

    Matrix result(rows, cols);
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
        {
            result.data[i][j] = data[i][j] + other.data[i][j];
        }
    }

    return result;
}

Matrix Matrix::operator-(const Matrix &other) const
{
    if (rows != other.rows || cols != other.cols)
    {
        throw MatrixError("Matrix dimensions do not match for subtraction");
    }

    Matrix result(rows, cols);
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
        {
            result.data[i][j] = data[i][j] - other.data[i][j];
        }
    }

    return result;
}

Matrix Matrix::operator*(const Matrix &other) const
{
    if (cols != other.rows)
    {
        throw MatrixError("Matrix dimensions are not compatible for multiplication");
    }

    Matrix result(rows, other.cols);
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < other.cols; ++j)
        {
            result.data[i][j] = 0;
            for (int k = 0; k < cols; ++k)
            {
                result.data[i][j] += data[i][k] * other.data[k][j];
            }
        }
    }

    return result;
}

Matrix &Matrix::operator+=(const Matrix &other)
{
    *this = *this + other;
    return *this;
}

Matrix &Matrix::operator-=(const Matrix &other)
{
    *this = *this - other;
    return *this;
}

Matrix &Matrix::operator*=(const Matrix &other)
{
    *this = *this * other;
    return *this;
}

bool Matrix::operator==(const Matrix &other) const
{
    if (rows != other.rows || cols != other.cols)
    {
        return false;
    }

    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
        {
            if (data[i][j] != other.data[i][j])
            {
                return false;
            }
        }
    }

    return true;
}

bool Matrix::operator!=(const Matrix &other) const
{
    return !(*this == other);
}

void Matrix::readFromFile(const std::string &filename)
{
    std::ifstream file(filename);
    if (!file)
    {
        throw MatrixFileError();
    }

    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
        {
            if (!(file >> data[i][j]))
            {
                throw MatrixFileError();
            }
        }
    }
}

void Matrix::swap(Matrix &mat)
{
	std::swap(rows, mat.rows);
	std::swap(cols, mat.cols);
	std::swap(data, mat.data);
	std::swap(refCount, mat.refCount);
}

int Matrix::ReadCell(int row, int col) const
{
    if (row < 0 || row >= rows || col < 0 || col >= cols)
    {
        throw MatrixOutOfBoundsError();
    }
    return data[row][col];
}

void Matrix::WriteCell(int row, int col, int value)
{
	// Wszystko robimy tylko wtedy gdy refCount > 1. Bo jeśli
	// obiekt klasy ma tylko jeden egzemplarz to nie musimy
	// robić kopii tylko modyfikujemy oryginał.
	
	// Matrix new_mat(rows, cols);
	// Kopiujemy dane do new_mat.
	// Zamieniamy nasze pola klasy z new_mat.
	// Dzięki temu w nas będzie już osobna kopia 
	// macierzy i możemy teraz ją dowolnie modyfikować.
	// Z chwilą kiedy WriteCell zakończy się, new_mat
	// zostanie zniszczone, co spowoduje zmniejszenie 
	// licznika referencji dotychczasowej macierzy o 1.
    if (row < 0 || row >= rows || col < 0 || col >= cols)
    {
        throw MatrixOutOfBoundsError();
    }
    
    if(*refCount > 1)
    {
		Matrix tmp(rows, cols);
		for(int i = 0; i<rows; i++)
		{
			for(int j = 0; j<cols; j++)
			{
				tmp.data[i][j] = data[i][j];
			} 
		}
		tmp.swap(*this);
	}
	
	data[row][col] = value;
}

CellRef Matrix::operator()(int row, int col)
{
    if (row < 0 || row >= rows || col < 0 || col >= cols)
    {
        throw MatrixOutOfBoundsError();
    }
    return CellRef(*this, row, col);
}

ConstCellRef Matrix::operator()(int row, int col) const
{
    if (row < 0 || row >= rows || col < 0 || col >= cols)
    {
        throw MatrixOutOfBoundsError();
    }
    return ConstCellRef(*this, row, col);
}

void Matrix::allocateMemory()
{
    data = new int *[rows];
    for (int i = 0; i < rows; ++i)
    {
        data[i] = new int[cols];
    }
}

void Matrix::deallocateMemory()
{
    for (int i = 0; i < rows; ++i)
    {
        delete[] data[i];
    }
    delete[] data;
}

void Matrix::increaseRefCount()
{
    (*refCount)++;
}

void Matrix::decreaseRefCount()
{
    (*refCount)--;
    if (*refCount == 0)
    {
        deallocateMemory();
        delete refCount;
    }
}
