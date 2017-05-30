#pragma once

#include <iostream>
#include <vector>
#include <GL/glew.h>

namespace cagd
{
// forward declaration of template class Matrix
template <typename T>
class Matrix;

// forward declaration of template class RowMatrix
template <typename T>
class RowMatrix;

// forward declaration of template class ColumnMatrix
template <typename T>
class ColumnMatrix;

// forward declaration of template class TriangularMatrix
template <typename T>
class TriangularMatrix;

// forward declarations of overloaded and templated input/output from/to stream operators
template <typename T>
std::ostream &operator<<(std::ostream &lhs, const Matrix<T> &rhs);

template <typename T>
std::istream &operator>>(std::istream &lhs, Matrix<T> &rhs);

template <typename T>
std::istream &operator>>(std::istream &lhs, TriangularMatrix<T> &rhs);

template <typename T>
std::ostream &operator<<(std::ostream &lhs, const TriangularMatrix<T> &rhs);

//----------------------
// template class Matrix
//----------------------
template <typename T>
class Matrix
{
    friend std::ostream &cagd::operator<<<T>(std::ostream &, const Matrix<T> &rhs);
    friend std::istream &cagd::operator>><T>(std::istream &, Matrix<T> &rhs);

  protected:
    GLuint _row_count;
    GLuint _column_count;
    std::vector<std::vector<T>> _data;

  public:
    // special constructor (can also be used as a default constructor)
    Matrix(GLuint row_count = 1, GLuint column_count = 1) : _row_count(row_count),
							    _column_count(column_count),
							    _data(_row_count, std::vector<T>(column_count))
    {
	;
    }

    // copy constructor
    Matrix(const Matrix &m) : _row_count(m._row_count),
			      _column_count(m._column_count),
			      _data(m._data)
    {
	;
    }

    // assignment operator
    Matrix &operator=(const Matrix &m)
    {
	if (this != &m)
	{
	    _row_count = m._row_count;
	    _column_count = m._column_count;
	    _data = m._data;
	}
	return *this;
    }

    // get element by reference
    T &operator()(GLuint row, GLuint column);

    // get copy of an element
    T operator()(GLuint row, GLuint column) const;

    // get dimensions
    GLuint GetRowCount() const;
    GLuint GetColumnCount() const;

    // set dimensions
    virtual GLboolean ResizeRows(GLuint row_count);
    virtual GLboolean ResizeColumns(GLuint column_count);

    // update
    GLboolean SetRow(GLuint index, const RowMatrix<T> &row);
    GLboolean SetColumn(GLuint index, const ColumnMatrix<T> &column);

    // destructor
    virtual ~Matrix()
    {
	_row_count = _column_count = 0;
	_data.clear();
    }
};

//-------------------------
// template class RowMatrix
//-------------------------
template <typename T>
class RowMatrix : public Matrix<T>
{
  public:
    // special constructor (can also be used as a default constructor)
    RowMatrix(GLuint column_count = 1) : Matrix<T>(1, column_count) { ; }

    // get element by reference
    T &operator()(GLuint column);
    T &operator[](GLuint column);

    // get copy of an element
    T operator()(GLuint column) const;
    T operator[](GLuint column) const;

    // a row matrix consists of a single row
    GLboolean ResizeRows(GLuint row_count)
    {
	return row_count == 1;
    }
};

//----------------------------
// template class ColumnMatrix
//----------------------------
template <typename T>
class ColumnMatrix : public Matrix<T>
{
  public:
    // special constructor (can also be used as a default constructor)
    ColumnMatrix(GLuint row_count = 1) : Matrix<T>(row_count, 1) { ; }

    // get element by reference
    T &operator()(GLuint row);
    T &operator[](GLuint row);

    // get copy of an element
    T operator()(GLuint row) const;
    T operator[](GLuint row) const;

    // a column matrix consists of a single column
    GLboolean ResizeColumns(GLuint column_count)
    {
	return column_count == 1;
    }
};

//--------------------------------
// template class TriangularMatrix
//--------------------------------
template <typename T>
class TriangularMatrix
{
    friend std::istream &cagd::operator>><T>(std::istream &, TriangularMatrix<T> &rhs);
    friend std::ostream &cagd::operator<<<T>(std::ostream &, const TriangularMatrix<T> &rhs);

  protected:
    GLuint _row_count;
    std::vector<std::vector<T>> _data;

  public:
    // special constructor (can also be used as a default constructor)
    TriangularMatrix(GLuint row_count = 1);

    // get element by reference
    T &operator()(GLuint row, GLuint column);

    // get copy of an element
    T operator()(GLuint row, GLuint column) const;

    // get dimension
    GLuint GetRowCount() const;

    // set dimension
    GLboolean ResizeRows(GLuint row_count);
};

//--------------------------------------------------
// homework: implementation of template class Matrix
//--------------------------------------------------
template <typename T>
T &Matrix<T>::operator()(GLuint row, GLuint column)
{
    return _data[row][column];
}
template <typename T>
T Matrix<T>::operator()(GLuint row, GLuint column) const
{
    return _data[row][column];
}
template <typename T>
GLuint Matrix<T>::GetRowCount() const
{
    return _row_count;
}
template <typename T>
GLuint Matrix<T>::GetColumnCount() const
{
    return _column_count;
}
template <typename T>
GLboolean Matrix<T>::ResizeRows(GLuint row_count)
{
    if (_row_count != row_count)
    {
	_data.resize(row_count);
	for (GLuint i = _row_count; i < row_count; ++i)
	{
	    _data[i].resize(_column_count);
	}
	_row_count = row_count;
	return true;
    }
    return false;
}
template <typename T>
GLboolean Matrix<T>::ResizeColumns(GLuint column_count)
{
    if (_column_count != column_count)
    {
	for (GLuint i = 0; i < _row_count; ++i)
	{
	    _data[i].resize(column_count);
	}
	_column_count = column_count;
	return true;
    }
    return false;
}
template <typename T>
GLboolean Matrix<T>::SetRow(GLuint index, const RowMatrix<T> &row)
{
    if (index >= _row_count || _column_count != row._column_count)
    {
	return GL_FALSE;
    }
    _data[index] = row._data[0];
    return GL_TRUE;
}
template <typename T>
GLboolean Matrix<T>::SetColumn(GLuint index, const ColumnMatrix<T> &column)
{
    if (index >= _column_count || _row_count != column._row_count)
    {
	return GL_FALSE;
    }
    for (GLuint i = 0; i < _row_count; ++i)
    {
	_data[i][index] = column._data[i][0];
    }
    return GL_TRUE;
}
//-----------------------------------------------------
// homework: implementation of template class RowMatrix
//-----------------------------------------------------
template <typename T>
T &RowMatrix<T>::operator()(GLuint column)
{
    return _data[0][column];
}
template <typename T>
T &RowMatrix<T>::operator[](GLuint column)
{
    return _data[0][column];
}
template <typename T>
T RowMatrix<T>::operator()(GLuint column) const
{
    return _data[0][column];
}
template <typename T>
T RowMatrix<T>::operator[](GLuint column) const
{
    return _data[0][column];
}

//--------------------------------------------------------
// homework: implementation of template class ColumnMatrix
//--------------------------------------------------------

template <typename T>
T &ColumnMatrix<T>::operator()(GLuint row)
{
    return _data[row][0];
}
template <typename T>
T &ColumnMatrix<T>::operator[](GLuint row)
{
    return _data[row][0];
}
template <typename T>
T ColumnMatrix<T>::operator()(GLuint row) const
{
    return _data[row][0];
}
template <typename T>
T ColumnMatrix<T>::operator[](GLuint row) const
{
    return _data[row][0];
}

//------------------------------------------------------------
// homework: implementation of template class TriangularMatrix
//------------------------------------------------------------
template <typename T>
TriangularMatrix<T>::TriangularMatrix(GLuint row_count) : _row_count(row_count), _data(_row_count)
{
    /*for(int i=0;i<row_count;++i)
		{
			_data[i].resize(i+1);
		}*/
    GLuint column_count = 1;
    //		for(typename vector<vector<T>>::_ITERATOR_DEBUG_LEVEL row=_data.begin();row!=_data.end();++row, ++column_count)
    //		{
    //			row.resize(column_count);
    //		}
    for (int i = 0; i < row_count; i++)
    {
	_data[i].resize(i + 1);
    }
    _row_count = row_count;
}

template <typename T>
T &TriangularMatrix<T>::operator()(GLuint row, GLuint column)
{
    //	return &_data[row][column];
    return this->_data[row][column];
}

template <typename T>
T TriangularMatrix<T>::operator()(GLuint row, GLuint column) const
{
    //	return &_data[row][column];
    return this->_data[row][column];
}

template <typename T>
GLuint TriangularMatrix<T>::GetRowCount() const
{
    return _row_count;
}

template <typename T>
GLboolean TriangularMatrix<T>::ResizeRows(GLuint row_count)
{
    if (_row_count != row_count)
    {
	_data.resize(row_count);
	_row_count = row_count;
	GLuint column_count = 1;
	for (typename std::vector<std::vector<T>>::iterator row = _data.begin(); row != _data.end(); row++, column_count++)
	{
	    row->resize(column_count);
	}
	return true;
    }
    return false;
}

//------------------------------------------------------------------------------
// definitions of overloaded and templated input/output from/to stream operators
//------------------------------------------------------------------------------

// output to stream
template <typename T>
std::ostream &operator<<(std::ostream &lhs, const Matrix<T> &rhs)
{
    lhs << rhs._row_count << " " << rhs._column_count << std::endl;
    for (typename std::vector<std::vector<T>>::const_iterator row = rhs._data.begin();
	 row != rhs._data.end(); ++row)
    {
	for (typename std::vector<T>::const_iterator column = row->begin();
	     column != row->end(); ++column)
	    lhs << *column << " ";
	lhs << std::endl;
    }
    return lhs;
}

// input from stream
template <typename T>
std::istream &operator>>(std::istream &lhs, Matrix<T> &rhs)
{
    // homework
    lhs >> rhs._row_count >> rhs._column_count;
    rhs._data.resize(rhs._row_count);
    for (typename std::vector<std::vector<T>>::iterator row = rhs._data.begin();
	 row != rhs._data.end(); ++row)
    {
	row->resize(rhs._column_count);
	for (typename std::vector<T>::iterator column = row->begin();
	     column != row->end(); ++column)
	    lhs >> *column;
    }
    return lhs;
}
template <typename T>
std::istream &operator>>(std::istream &lhs, TriangularMatrix<T> &rhs)
{
    lhs >> rhs._row_count;
    rhs._data.resize(rhs._row_count);
    unsigned int i = 1;
    for (typename std::vector<std::vector<T>>::const_iterator row = rhs._data.begin();
	 row != rhs._data.end(); ++row)
    {
	row->resize(i);
	for (typename std::vector<T>::const_iterator column = row->begin();
	     column != row->end(); ++column)
	    lhs >> *column;
	++i;
    }
    return lhs;
}
template <typename T>
std::ostream &operator<<(std::ostream &lhs, const TriangularMatrix<T> &rhs)
{
    lhs << rhs._row_count << std::endl;
    for (typename std::vector<std::vector<T>>::const_iterator row = rhs._data.begin();
	 row != rhs._data.end(); ++row)
    {
	for (typename std::vector<T>::const_iterator column = row->begin();
	     column != row->end(); ++column)
	    lhs << *column << " ";
	lhs << std::endl;
    }
    return lhs;
}
}
