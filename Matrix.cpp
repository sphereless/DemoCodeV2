//
//  Matrix.cpp
//  Maths
//
//  Created by Ashish Aring on 13.05.21.
//  Copyright © 2021 Sphereless. All rights reserved.
//

#include "Matrix.hpp"



// constructor
template <typename T>
Matrix<T>::Matrix			(unsigned int nRows, unsigned int nCols, T value)
:	Vector<T>	(nRows*nCols, value),
_nRows		(nRows),
_nCols		(nCols),
_fPtrIndex	(&Matrix::indexColVec)
{
	/* empty */
}
template <typename T>
Matrix<T>::Matrix			(const Matrix<T>& matrix)
:	Vector<T>	(matrix),
_nRows		(matrix._nRows),
_nCols		(matrix._nCols),
_fPtrIndex	(matrix.isColVector() ?  (&Matrix::indexColVec) : (&Matrix::indexRowVec))
{
	/* empty */
}
template <typename T>
Matrix<T>::~Matrix			()
{
	// delete _fPtrIndex;
}



// gets
template <typename T>
unsigned int				Matrix<T>::getNumRows	()	const	{ return _nRows;	}
template <typename T>
unsigned int				Matrix<T>::getNumCols	()	const	{ return _nCols;	}
template <typename T>
bool						Matrix<T>::isSquareMat	()	const	{ return _nRows == _nCols;}
template <typename T>
bool						Matrix<T>::hasSameSize	(const Matrix<T> &mat) const	{ return (mat._nRows == _nRows && mat._nCols == _nCols);}
template <typename T>
bool						Matrix<T>::isMultiplyOk	(const Matrix<T> &mat) const	{ return _nCols == mat._nRows;		}
template <typename T>
bool						Matrix<T>::isMultiplyOk	(const Vector<T> &vec) const	{ return _nCols == vec.getLen();	}



// operators

template <typename T>
Matrix<T>&					Matrix<T>::operator =	(const Matrix<T> &mat)
{
	if (hasSameSize(mat))
		Vector<T>::operator = (mat);
	return *this;
}
template <typename T>
T&							Matrix<T>::operator	()	(unsigned int iRow, unsigned int iCol) const
{
	return Vector<T>::operator [] ((this->*_fPtrIndex)(iRow, iCol));
}
template <typename T>
bool						Matrix<T>::operator	==	(const Matrix<T>& mat) const
{
	if (!Vector<T>::operator == (mat))
		return false;
	return hasSameSize(mat);
}
template <typename T>
bool						Matrix<T>::operator	!=	(const Matrix<T>& mat) const				{	return !operator==(mat);		}


template <typename T>
Matrix<T>&					Matrix<T>::operator +=	(const Matrix<T>& mat)
{
	if (hasSameSize(mat))
		Vector<T>::operator += (mat);
	return *this;
}
template <typename T>
Matrix<T>&					Matrix<T>::operator -=	(const Matrix<T>& mat)
{
	if (hasSameSize(mat))
		Vector<T>::operator -= (mat);
	return *this;
}
template <typename T>
Matrix<T>&					Matrix<T>::operator *=	(const Matrix<T>& mat)
{
	if (isSquareMat() && isMultiplyOk(mat))
	{
		Matrix<T> copy(*this);
		this->reset();
		
		for (unsigned int i=0; i < _nRows; ++i)
			for (unsigned int j=0; j < mat._nCols; ++j)
				for (unsigned int k=0; k < _nCols; ++k)
					(*this)(i,j) += (copy)(i,k) * (mat)(k,j);
	}
	return *this;
}

template <typename T>
Matrix<T>&					Matrix<T>::operator *=	(int factor)
{
	Vector<T>::operator *= (factor);
	return *this;
}
template <typename T>
Matrix<T>&					Matrix<T>::operator *=	(double factor)
{
	Vector<T>::operator *= (factor);
	return *this;
}
template <typename T>
Matrix<T>&					Matrix<T>::operator /=	(int factor)
{
	Vector<T>::operator /= (factor);
	return *this;
}
template <typename T>
Matrix<T>&					Matrix<T>::operator /=	(double factor)
{
	Vector<T>::operator /= (factor);
	return *this;
}



// methods

template <typename T>
unsigned int				Matrix<T>::indexColVec	(unsigned int iRow, unsigned int iCol) const
{
	// Spaltenvektor (Nx1)
	return (iCol * _nRows + iRow);
}

template <typename T>
unsigned int				Matrix<T>::indexRowVec	(unsigned int iRow, unsigned int iCol) const
{
	// Zeilenvektor (1xN)
	return (iRow * _nCols + iCol);
}

template <typename T>
void						Matrix<T>::reset		(T value)									{	Vector<T>::reset(value);		}

template <typename T>
Matrix<T>&					Matrix<T>::transpose	()
{
	Vector<T>::transpose();
	
	// re-assign fPtr
	_fPtrIndex = Vector<T>::isColVector() ? (&Matrix::indexColVec) : (&Matrix::indexRowVec);
	
	// swap rows, cols
	unsigned int temp = _nRows;
	_nRows = _nCols;
	_nCols = temp;
	
	return *this;
}

template <typename T>
void						Matrix<T>::makeUnity	()
{
	reset();
	for (unsigned int i=0; i < (_nRows<_nCols ? _nRows:_nCols); ++i)
		(*this)(i,i) = (T)1;
}

template <typename T>
void						Matrix<T>::print		() const
{
	std::cout << std::right << std::setprecision(3) << std::setfill(' ');
	for (unsigned int i=0; i < _nRows; ++i)
	{
		for (unsigned int j=0; j < _nCols; ++j)
			std::cout << std::setw(10) << Vector<T>::operator [] ((this->*_fPtrIndex)(i, j));
		std::cout << std::endl;
	}
	std::cout << std::endl;
}



// static methods

template <typename T>
void			Matrix<T>::getRotX		(double radians, Matrix<double>& m)
{
	if (m.getNumRows() != 3 || m.getNumCols() != 3)
		return;
	
	double sinRad = sin(radians);
	double cosRad = cos(radians);
	
	m.makeUnity();
	m(1,1) = cosRad;
	m(2,2) = cosRad;
	m(1,2) = sinRad;
	m(2,1) =-sinRad;
}
template <typename T>
void			Matrix<T>::getRotY		(double radians, Matrix<double>& m)
{
	if (m.getNumRows() != 3 || m.getNumCols() != 3)
		return;
	
	double sinRad = sin(radians);
	double cosRad = cos(radians);
	
	m.makeUnity();
	m(0,0) = cosRad;
	m(2,2) = cosRad;
	m(0,2) =-sinRad;
	m(2,0) = sinRad;
}
template <typename T>
void			Matrix<T>::getRotZ		(double radians, Matrix<double>& m)
{
	if (m.getNumRows() != 3 || m.getNumCols() != 3)
		return;
	
	double sinRad = sin(radians);
	double cosRad = cos(radians);
	
	m.makeUnity();
	m(0,0) = cosRad;
	m(1,1) = cosRad;
	m(0,1) = sinRad;
	m(1,0) =-sinRad;
}

template <typename T>
void			Matrix<T>::getRotXDot		(double radians, double omega, Matrix<double>& m)
{
	if (m.getNumRows() != 3 || m.getNumCols() != 3)
		return;
	
	double sinRad = sin(radians);
	double cosRad = cos(radians);
	
	m.reset();
	m(1,1) =-sinRad;
	m(2,2) =-sinRad;
	m(1,2) = cosRad;
	m(2,1) =-cosRad;
	m *= omega;
}
template <typename T>
void			Matrix<T>::getRotYDot		(double radians, double omega, Matrix<double>& m)
{
	if (m.getNumRows() != 3 || m.getNumCols() != 3)
		return;
	
	double sinRad = sin(radians);
	double cosRad = cos(radians);
	
	m.reset();
	m(0,0) =-sinRad;
	m(2,2) =-sinRad;
	m(0,2) =-cosRad;
	m(2,0) = cosRad;
	m *= omega;
}
template <typename T>
void			Matrix<T>::getRotZDot		(double radians, double omega, Matrix<double>& m)
{
	if (m.getNumRows() != 3 || m.getNumCols() != 3)
		return;
	
	double sinRad = sin(radians);
	double cosRad = cos(radians);
	
	m.reset();
	m(0,0) =-sinRad;
	m(1,1) =-sinRad;
	m(0,1) = cosRad;
	m(1,0) =-cosRad;
	m *= omega;
}

template class Matrix<unsigned int>;
template class Matrix<int>;
template class Matrix<double>;

