#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>
#include <fstream>
#include <stdexcept>

class CellRef;
class ConstCellRef;

class Matrix
{
public:
    // Constructor
    Matrix(int rows, int cols);

    // Copy Constructor
    Matrix(const Matrix &other);

    // Destructor
    ~Matrix();

    // Assignment Operator
    Matrix &operator=(const Matrix &other);

    // Display Operator
    friend std::ostream &operator<<(std::ostream &os, const Matrix &matrix);

    // Arithmetic Operators
    Matrix operator+(const Matrix &other) const;
    Matrix operator-(const Matrix &other) const;
    Matrix operator*(const Matrix &other) const;
    Matrix &operator+=(const Matrix &other);
    Matrix &operator-=(const Matrix &other);
    Matrix &operator*=(const Matrix &other);

    // Comparison Operators
    bool operator==(const Matrix &other) const;
    bool operator!=(const Matrix &other) const;

    // Read matrix from input file stream
    void readFromFile(const std::string &filename);

    // Single Element Access
    CellRef operator()(int row, int col);
    ConstCellRef operator()(int row, int col) const;
    
    int ReadCell(int row, int col) const;
    void WriteCell(int row, int col, int value);
    
    void swap(Matrix &mat);

private:
    int rows;
    int cols;
    int **data;
    int *refCount;

    // Helper methods
    void allocateMemory();
    void deallocateMemory();
    void increaseRefCount();
    void decreaseRefCount();
};

class CellRef
{
	private:
		Matrix &mat;
		int row;
		int col;
		
		
	public:
		CellRef(Matrix &mat, int row, int col)
			: mat(mat), row(row), col(col)
		{}
		
		// Gdy wstawiamy wartość do macierzy.
		void operator=(int value)
		{
			mat.WriteCell(row, col, value);
		}
		
		// Gdy chcemy odczytać daną z maceirzy.
		operator int() const
		{
			return mat.ReadCell(row, col);
		}
};

class ConstCellRef
{
	private:
		const Matrix &mat;
		int row;
		int col;
		
		
	public:
		ConstCellRef(const Matrix &mat, int row, int col)
			: mat(mat), row(row), col(col)
		{}
		
		// Gdy chcemy odczytać daną z maceirzy.
		operator int() const
		{
			return mat.ReadCell(row, col);
		}
};

// Custom exceptions
class MatrixError : public std::runtime_error
{
public:
    MatrixError(const std::string &message) : std::runtime_error(message) {}
};

class MatrixOutOfBoundsError : public MatrixError
{
public:
    MatrixOutOfBoundsError() : MatrixError("Matrix index out of bounds") {}
};

class MatrixFileError : public MatrixError
{
public:
    MatrixFileError() : MatrixError("Error reading from file") {}
};

#endif // MATRIX_H
