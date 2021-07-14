//
//  Matrix.hpp
//  Maths
//
//  Created by Ashish Aring on 13.05.21.
//  Copyright © 2021 Sphereless. All rights reserved.
//

#ifndef Matrix_hpp
#define Matrix_hpp


#include "Vector.hpp"



template <typename T>
class	Matrix	:	private Vector<T>
{
private:
	
	// typedefs
	typedef			unsigned int (Matrix::*IndexingFct) (unsigned int, unsigned int) const;
	
	// members
	unsigned int	_nRows;
	unsigned int	_nCols;
	IndexingFct		_fPtrIndex;
	
	// methods
	unsigned int	indexColVec	(unsigned int iRow, unsigned iCol)	const;
	unsigned int	indexRowVec	(unsigned int iRow, unsigned iCol)	const;
	
public:
	
	// constructor
	Matrix		(unsigned int nRows, unsigned int nCols, T value = (T)0);
	Matrix		(const Matrix<T>& matrix);
	~Matrix		();
	
	// gets
	unsigned int	getNumRows	() const;
	unsigned int	getNumCols	() const;
	bool			isSquareMat	() const;
	bool			hasSameSize	(const Matrix<T>&) const;
	bool			isMultiplyOk(const Matrix<T>&) const;
	bool			isMultiplyOk(const Vector<T>&) const;
	
	
	// operators
	Matrix<T>&		operator =	(const Matrix<T>&);
	T&				operator ()	(unsigned int iRow, unsigned iCol)	const;
	bool			operator == (const Matrix<T>&)					const;
	bool			operator != (const Matrix<T>&)					const;
	
	Matrix<T>&		operator +=	(const Matrix<T>&);
	Matrix<T>&		operator -=	(const Matrix<T>&);
	Matrix<T>&		operator *=	(const Matrix<T>&);
	Matrix<T>&		operator *=	(int factor);
	Matrix<T>&		operator *=	(double factor);
	Matrix<T>&		operator /=	(int factor);
	Matrix<T>&		operator /=	(double factor);
	
	// methods
	void			reset		(T value = (T)0);
	void			makeUnity	();
	Matrix<T>&		transpose	();
	void			print		()									const;
	
	// static methods
	static void		getRotX		(double radians, Matrix<double>& m);
	static void		getRotY		(double radians, Matrix<double>& m);
	static void		getRotZ		(double radians, Matrix<double>& m);
	static void		getRotXDot	(double radians, double omega, Matrix<double>& m);
	static void		getRotYDot	(double radians, double omega, Matrix<double>& m);
	static void		getRotZDot	(double radians, double omega, Matrix<double>& m);
};



// non-member operators


template <typename T>
Matrix<T>&			operator *	(const Matrix<T>& mat1, int factor)
{													// if factor < 0, prevent multiplication for <T> == <unsigned int>
	Matrix<T>* res = new Matrix<T>(mat1);			// '*=' is NOT const, cannot call 'mat1 *= factor' !
	*res *= factor;									// Should work for i == 1
	return *res;
}
template <typename T>
Matrix<T>&			operator *	(const Matrix<T>& mat1, double factor)
{													// prevent multiplication for <T> == <unsigned int>
	Matrix<T>* res = new Matrix<T>(mat1);			// '*=' is NOT const, cannot call 'mat1 *= factor' !
	*res *= factor;
	return *res;
}

template <typename T>
Matrix<T>&			operator *	(int factor, const Matrix<T>& mat1)
{
	return mat1*factor;												// Should work for i == 1
}
template <typename T>
Matrix<T>&			operator *	(double factor, const Matrix<T>& mat1)
{
	return mat1*factor;
}

template <typename T>
Matrix<T>&			operator /	(const Matrix<T>& mat1, int divisor)
{																	// Should work for i == 1
	return mat1*(1./(double)divisor);
}
template <typename T>
Matrix<T>&			operator /	(const Matrix<T>& mat1, double divisor)
{
	return mat1*(1./divisor);
}



template <typename T>
Matrix<T>&			operator *	(const Matrix<T>& mat1, const Matrix<T>& mat2)
{
	Matrix<T>* res = nullptr;
	
	if (mat1.isMultiplyOk(mat2))
	{
		unsigned int m1rows = mat1.getNumRows();
		unsigned int m1cols = mat1.getNumCols();
		unsigned int m2cols = mat2.getNumCols();
		res = new Matrix<T>(m1rows, m2cols);
		
		for (unsigned int i=0; i < m1rows; ++i)
			for (unsigned int j=0; j < m2cols; ++j)
				for (unsigned int k=0; k < m1cols; ++k)
					(*res)(i,j) += mat1(i,k) * mat2(k,j);
	}
	return *res;
}

template <typename T>
Vector<T>&			operator *	(const Matrix<T>& mat, const Vector<T>& vec)
{
	Vector<T>* res = nullptr;
	
	if (mat.isMultiplyOk(vec))
	{
		res = new Vector<T>(mat.getNumRows());
		for (unsigned int i=0; i < mat.getNumRows(); ++i)
			for (unsigned int j=0; j < mat.getNumCols(); ++j)
				(*res)[i] += mat(i,j) * vec[j];
	}
	return *res;
}

template <typename T>
Matrix<T>&			operator ^	(const Matrix<T>& mat, unsigned int exponent)
{
	Matrix<T>* res = nullptr;
	
	// "power"-operation exclusively for square matrices
	if (mat.isSquareMat())
	{
		res = new Matrix<T>(mat);
		res->makeUnity();
		
		for (unsigned int i=0; i < exponent; ++i)
			(*res) *= mat;
	}
	return *res;
}

#endif /* Matrix_hpp */
