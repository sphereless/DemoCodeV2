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
Matrix<T>::Matrix			(unsigned int nRows,
                             unsigned int nCols,
                             T value)
			:	Vector<T>	(nRows*nCols, true, value),
				_nRows		(nRows),
				_nCols		(nCols),
				_indexMappingFct(&Matrix::mapFctColumn),                                 // TODO: verify !
                _dimString  (""),
                _add        (nullptr),
                _substr     (nullptr),
                _mult       (nullptr),
                _pow        (nullptr),
                _mMultByVec (nullptr)
{
	// permit re-orientation only for square matrices
	if (this->isSquare())
		Vector<T>::allowReorient();
	
	this->updateDimString();
}
template <typename T>
Matrix<T>::Matrix			(const Matrix<T>& matrix)
			:	Matrix		(matrix._nRows,
                             matrix._nCols)
{
	_indexMappingFct =	this->isColVector()
                        ? &Matrix::mapFctColumn
                        : &Matrix::mapFctRow;		                                            // TODO: verify !
	*this = matrix;
}
template <typename T>
Matrix<T>::~Matrix			()
{
    /*
     *  All members deleted by Vector,
     *  including helpers.
     */
}



// gets

template <typename T>
unsigned int				Matrix<T>::getNumRows		()	const	{ return _nRows;	}
template <typename T>
unsigned int				Matrix<T>::getNumCols		()	const	{ return _nCols;	}
template <typename T>
std::string					Matrix<T>::getDimString		()	const	{ return _dimString;	}
template <typename T>
bool						Matrix<T>::isSquare			()	const	{ return _nRows == _nCols;}
template <typename T>
bool						Matrix<T>::isSameDimension	(const Matrix<T> &mat) const	{ return (mat._nRows == _nRows && mat._nCols == _nCols);}
template <typename T>
bool						Matrix<T>::isMultiplyOk		(const Matrix<T> &mat) const	{ return _nCols == mat._nRows;		}
template <typename T>
bool						Matrix<T>::isMultiplyOk		(const Vector<T> &vec) const	{ return _nCols == vec.getLen();	}



// operators

template <typename T>
T&							Matrix<T>::operator	()		(unsigned int iRow, unsigned int iCol) const
{
	return Vector<T>::operator [] ((this->*_indexMappingFct)(iRow, iCol));
}

template <typename T>
Matrix<T>&					Matrix<T>::operator =		(const Matrix<T> &mat)
{
    this->verifySameDimOrThrow(mat, "=");
	
	Vector<T>::operator = (mat);
	if (Vector<T>::hasReoriented()) this->updateMapFunction();
	
	return *this;
}

/**
 *	Transpose-operator
 */
template <typename T>
Matrix<T>&					Matrix<T>::operator ~		()
{
	Vector<T>::operator ~();
	updateMapFunction();
	
	// swap rows, cols
	unsigned int temp = _nRows;
	_nRows = _nCols;
	_nCols = temp;
	this->updateDimString();
	
	return *this;
}


template <typename T>
bool						Matrix<T>::operator	==		(const Matrix<T>& mat) const
{
	if (!this->isSameDimension(mat))	return false;
	
	return (Vector<T>::operator == (mat));
}
template <typename T>
bool						Matrix<T>::operator ==		(const T& value)	   const
{
	return (Vector<T>::operator == (value));
}
template <typename T>
bool						Matrix<T>::operator	!=		(const Matrix<T>& mat) const
{
	return !(operator == (mat));
}


template <typename T>
Matrix<T>&					Matrix<T>::operator +=		(const Matrix<T>& mat)
{
    this->verifySameDimOrThrow(mat, "+=");
	
	Vector<T>::operator += (mat);
	return *this;
}

template <typename T>
Matrix<T>&					Matrix<T>::operator -=		(const Matrix<T>& mat)
{
    this->verifySameDimOrThrow(mat, "-=");
	
	Vector<T>::operator -= (mat);
	return *this;
}

template <typename T>
Matrix<T>&					Matrix<T>::operator *=		(T factor)
{
	Vector<T>::operator *= (factor);
	return *this;
}

/**
 *	In operator *= (Matrix&), 'this'-object must be square,
 *	to retain its dimension before and after multiplication.
 *
 *		this *= arg
 *	<=>	this(pre)  *  arg   = this(post)
 *		---------    -----    -----------
 *		rect.[3,5] * [5,3]	= [3,3]		-> different dimension!
 *		sq.	 [3,3] * [3,3]	= [3,3]		-> same dimension!
 */
template <typename T>
Matrix<T>&					Matrix<T>::operator *=		(const Matrix<T>& mat)
{
    this->verifySquareOrThrow("*=");
	
	*this = *this * mat;
	return *this;
}

template <typename T>
Matrix<T>&					Matrix<T>::operator /=		(T factor)
{
	Vector<T>::operator /= (factor);
	return *this;
}

template <typename T>
Matrix<T>&					Matrix<T>::operator ^=		(unsigned int exponent)
{
	this->verifySquareOrThrow("^=");
	
	if (exponent > 1)
	{
        if (_pow == nullptr) _pow = new Matrix<T>(*this);
		for (unsigned int i=2; i <= exponent; i++)
			*this = (*this) * (*_pow);
	}
	return *this;
}



// methods

template <typename T>
void						Matrix<T>::verifySameDimOrThrow		(const Matrix<T> &mat, std::string operation)	const
{
	if (!this->isSameDimension(mat)) {
		this->_logger->error(typeid(this).name(), "Matrix-argument has incompatible dimension "+mat.getDimString()+" for ("+operation+").");
		throw std::invalid_argument("Matrix-argument has incompatible dimension "+mat.getDimString()+" for ("+operation+").");
	}
}

template <typename T>
void						Matrix<T>::verifySquareOrThrow		(std::string operation)	const
{
	if(!this->isSquare()) {
		this->_logger->error(typeid(this).name(), "Non-square Matrix prevents self-assigning result to 'this' in ("+operation+").");
		throw std::domain_error("Non-square Matrix prevents self-assigning result to 'this' in ("+operation+").");
	}
}

template <typename T>
void						Matrix<T>::verifyMultiplyOrThrow	(const Matrix<T> &mat, std::string operation)	const
{
	if (!this->isMultiplyOk(mat)) {
		this->_logger->error(typeid(this).name(), "Matrix-argument has incompatible dimension "+mat.getDimString()+" for ("+operation+").");
		throw std::invalid_argument("Matrix-argument has incompatible dimension "+mat.getDimString()+" for ("+operation+").");
	}
}

template <typename T>
void						Matrix<T>::updateDimString			()
{
	std::ostringstream os;
	os << _nRows << "x" << _nCols;
	_dimString = os.str();
}

template <typename T>
void						Matrix<T>::updateMapFunction			()
{
	// re-assign fPtr
	_indexMappingFct = Vector<T>::isColVector()
                    ? &Matrix::mapFctColumn
                    : &Matrix::mapFctRow;
 /*
    _add->updateMapFunction(_add->isColVector()
                    ? _add->Matrix::mapFctColumn
                    : _add->Matrix::mapFctRow);
  */
}

/**
 *		Mapping functions (column, row)
 */
template <typename T>
unsigned int				Matrix<T>::mapFctColumn		(unsigned int iRow, unsigned int iCol) const
{
	// Spaltenvektor (Nx1)
	return (iCol * _nRows + iRow);
}

template <typename T>
unsigned int				Matrix<T>::mapFctRow		(unsigned int iRow, unsigned int iCol) const
{
	// Zeilenvektor (1xN)
	return (iRow * _nCols + iCol);
}


template <typename T>
void						Matrix<T>::makeUnity	()
{
    this->reset();
	for (unsigned int i=0; i < std::min(_nRows, _nCols); ++i)
		(*this)(i,i) = (T)1;
}

template <typename T>
void						Matrix<T>::reset		(T value)
{
	Vector<T>::reset(value);
}

template <typename T>
void						Matrix<T>::print		(std::ostream& os,
													 unsigned int indent,
													 unsigned int precision)	const
{
	os << std::right << std::setfill(' ') << std::fixed;
	
	for (unsigned int i=0; i < _nRows; ++i) {
		os << std::setw(indent) << Constants::Outputs::INTROCHAR << " ";
		os << std::setprecision(precision);
		for (unsigned int j=0; j < _nCols; ++j)
			os << std::setw(Constants::Outputs::WIDTHVALUE) << Vector<T>::operator [] ((this->*_indexMappingFct)(i, j));
		os << std::endl;
	}
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

