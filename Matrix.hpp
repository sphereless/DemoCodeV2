//
//  Matrix.hpp
//  Maths
//
//  Created by Ashish Aring on 13.05.21.
//  Copyright © 2021 Sphereless. All rights reserved.
//

#ifndef Matrix_hpp
#define Matrix_hpp

#include <sstream>

#include "Vector.hpp"

template <typename T> class Matrix;
template <typename T> Matrix<T>&  operator +           (const Matrix<T>& mat1,
                                                        const Matrix<T>& mat2);
template <typename T> Matrix<T>&  operator -           (const Matrix<T>& mat1,
                                                        const Matrix<T>& mat2);
template <typename T> Matrix<T>&  operator *           (const Matrix<T>& mat1,
                                                        T factor);
template <typename T> Matrix<T>&  operator *           (const Matrix<T>& mat1,
                                                        const Matrix<T>& mat2);
template <typename T> Vector<T>&  operator *           (const Matrix<T>& mat,
                                                        const Vector<T>& vec);
template <typename T> Matrix<T>&  operator /           (const Matrix<T>& mat1,
                                                        T divisor);
template <typename T> Matrix<T>&  operator ^           (const Matrix<T>& mat,
                                                        unsigned int exponent);

/**
 *		Provides (m,n) numeric-array with matrix-calculus.
 *
 *		Inherits Vector privately!
 *
 *		Access to array-elements via ()-operator for read, write.
 *		Classic matrix-calculus via operators +, -, *, /, ^.
 *		Special operations via transpose ~, cross-product *.
 *
 *		Matrix-elements are mapped to Vector via indexing-function.
 *		Transposing the 'Matrix' simply adapts the indexing-function.
 *
 *		TODO:	-	When transposing Matrix, why transpose Vector-array too??
 *					Try excluding both, Vector-transpose & update map-function.
 *				-	verify operator * (Matrix&, Vector&)
 */

template <typename T>
class	Matrix	:		private Vector<T>
{
private:
	
	/**
	 *	Define type: Pointer to index-mapping member-function
	 */
	typedef				unsigned int		(Matrix::*PtrIndexingFct) (unsigned int, unsigned int) const;
	
	// members
	unsigned int		_nRows,
                        _nCols;
	PtrIndexingFct		_indexMappingFct;
	std::string			_dimString;

    // mutable members
    mutable Matrix<T>*  _add;                   // helper matrices to reduce dynamic allocations
    mutable Matrix<T>*  _substr;
    mutable Matrix<T>*  _mult;
    mutable Matrix<T>*  _pow;
    mutable Vector<T>*  _mMultByVec;

	// methods
	void				verifySameDimOrThrow    (const Matrix<T>& mat, std::string operation)	const;
	void				verifySquareOrThrow	    (std::string operation)							const;
	void				verifyMultiplyOrThrow   (const Matrix<T>& mat, std::string operation)	const;
	unsigned int		mapFctColumn	        (unsigned int iRow, unsigned iCol)	const;
	unsigned int		mapFctRow		        (unsigned int iRow, unsigned iCol)	const;
	void				updateDimString		    ();
	void				updateMapFunction	    ();

public:
	
	// constructor
						Matrix				(unsigned int nRows,
                                             unsigned int nCols,
                                             T value = (T)0);
						Matrix				(const Matrix<T>& matrix);
						~Matrix				();
	
	// gets
	unsigned int		getNumRows			() const;
	unsigned int		getNumCols			() const;
	std::string			getDimString		() const;
	bool				isSquare			() const;
	bool				isSameDimension		(const Matrix<T>&) const;
	bool				isMultiplyOk		(const Matrix<T>&) const;
	bool				isMultiplyOk		(const Vector<T>&) const;
	
    
    // friend operators
    friend Matrix<T>&   operator +    <>    (const Matrix<T>& mat1,
                                             const Matrix<T>& mat2);
    friend Matrix<T>&   operator -    <>    (const Matrix<T>& mat1,
                                             const Matrix<T>& mat2);
    friend Matrix<T>&   operator *    <>    (const Matrix<T>& mat1,
                                             T factor);
    friend Matrix<T>&   operator *    <>    (const Matrix<T>& mat1,
                                             const Matrix<T>& mat2);
    friend Vector<T>&   operator *    <>    (const Matrix<T>& mat,
                                             const Vector<T>& vec);
    friend Matrix<T>&   operator /    <>    (const Matrix<T>& mat1,
                                             T divisor);
    friend Matrix<T>&   operator ^    <>    (const Matrix<T>& mat,
                                             unsigned int exponent);


	// operators
	T&					operator ()			(unsigned int iRow,
                                             unsigned iCol)	                    const;
	Matrix<T>&			operator =			(const Matrix<T>&);
	Matrix<T>&			operator ~			();									// transpose
	
	bool				operator ==			(const Matrix<T>&)					const;
	bool				operator ==			(const T&)							const;
	bool				operator !=			(const Matrix<T>&)					const;
	
	Matrix<T>&			operator +=			(const Matrix<T>&);
	Matrix<T>&			operator -=			(const Matrix<T>&);
	Matrix<T>&			operator *=			(const Matrix<T>&);
	Matrix<T>&			operator *=			(T factor);
	Matrix<T>&			operator /=			(T factor);
	Matrix<T>&			operator ^=			(unsigned int exponent);
	
	// methods
	void				makeUnity			();
	void				reset				(T value = (T)0);
	void				print				(std::ostream& os = std::cout,
											 unsigned int indent = Constants::Outputs::TEXTINDENT,
											 unsigned int precision = Constants::Outputs::LOWPRECISION)	const;
	
	// static methods
	static void			getRotX				(double radians,
                                             Matrix<double>& m);
	static void			getRotY				(double radians,
                                             Matrix<double>& m);
	static void			getRotZ				(double radians,
                                             Matrix<double>& m);
	static void			getRotXDot			(double radians,
                                             double omega,
                                             Matrix<double>& m);
	static void			getRotYDot			(double radians,
                                             double omega,
                                             Matrix<double>& m);
	static void			getRotZDot			(double radians,
                                             double omega,
                                             Matrix<double>& m);
};



//	============== Non-member operators =============

/**
 *		Plus+ template-operators
 */
template <typename T>
inline Matrix<T>&			    operator +	(const Matrix<T>& mat1,
                                             const Matrix<T>& mat2)
{
	if (!mat1.isSameDimension(mat2))
		throw std::invalid_argument("Mismatching dimensions of matrix-arguments.");
    if (mat1._add == nullptr)
        mat1._add = new Matrix<T>(mat1.getNumRows(),
                                  mat1.getNumCols());
    
    *mat1._add = mat1;
    return (*mat1._add += mat2);
}

/**
 *		Minus- template-operators
 *
 *		Not allowed for UInts.
 */
template <typename T>
inline Matrix<T>&				operator -	(const Matrix<T>& mat1, const Matrix<T>& mat2)
{
	if (!mat1.isSameDimension(mat2))
		throw std::invalid_argument("Mismatching dimensions of matrix-arguments.");
    if (mat1._add == nullptr)
        mat1._substr = new Matrix<T>(mat1.getNumRows(),
                                     mat1.getNumCols());
    
    *mat1._substr = mat1;
	return (*mat1._substr -= mat2);
}

template <>
inline Matrix<unsigned int>&	operator -	(const Matrix<unsigned int>& mat1,
                                             const Matrix<unsigned int>& mat2)
{
	throw std::bad_typeid();
}

/**
 *		Multiply* template-operators
 *
 *		Cannot use (mat1 *= factor) because mat1 is 'const'.
 *		Allocation on heap avoids end-of-scope destructor.
 */
template <typename T>
inline Matrix<T>&			    operator *	(const Matrix<T>& mat1,
                                             T factor)
{
    if (mat1._add == nullptr)
        mat1._mult = new Matrix<T>(mat1.getNumRows(),mat1.getNumCols());
    
    *mat1._mult = mat1;
	return (*mat1._mult *= factor);
}

template <typename T>
inline Matrix<T>&			    operator *	(T factor,
                                             const Matrix<T>& mat1)
{
	return mat1*factor;
}

template <typename T>
inline Matrix<T>&			    operator *	(const Matrix<T>& mat1,
                                             const Matrix<T>& mat2)
{
	// == verifyMultOrThrow
	if (!mat1.isMultiplyOk(mat2))
		throw std::invalid_argument("Incompatible matrices for multiplication ("+mat1.getDimString()+") <!> ("+mat2.getDimString()+").");
    if (mat1._mult == nullptr)
        mat1._mult = new Matrix<T>(mat1.getNumRows(),
                                   mat2.getNumCols());
    
    mat1._mult->reset();
	for (unsigned int i=0; i < mat1.getNumRows(); ++i)
		for (unsigned int j=0; j < mat2.getNumCols(); ++j)
			for (unsigned int k=0; k < mat1.getNumCols(); ++k)
				(*mat1._mult)(i,j) += mat1(i,k) * mat2(k,j);
    
	return *mat1._mult;
}

template <typename T>
inline Vector<T>&			    operator *	(const Matrix<T>& mat,
                                             const Vector<T>& vec)
{
	if (!mat.isMultiplyOk(vec))
		throw std::invalid_argument("Incompatible pair of matrix,vector for multiplication.");
	
    if (mat._mMultByVec == nullptr)
        mat._mMultByVec = new Vector<T>(mat.getNumRows());
    else
        mat._mMultByVec->reset();
    
	for (unsigned int i=0; i < mat.getNumRows(); ++i)
		for (unsigned int j=0; j < mat.getNumCols(); ++j)
			(*mat._mMultByVec)[i] += mat(i,j) * vec[j];
	return *mat._mMultByVec;
}


/**
 *		Divide/ template-operator
 *
 *		Only permitted for Matrix<double>.
 */
template <typename T>
inline Matrix<T>&			    operator /	(const Matrix<T>& mat1,
                                             T divisor)
{
	throw std::bad_typeid();
}

template <>
inline Matrix<double>&		    operator /	(const Matrix<double>& mat1,
                                             double divisor)
{
	// see operator * for explanation
	return (mat1 * (1./divisor));
}


/**
 *		Power^ template-operator
 *
 *		Matrices must be square for this operation.
 *		Argument of type UInt prevents negative exponents.
 */
template <typename T>
inline Matrix<T>&			    operator ^	(const Matrix<T>& mat,
                                             unsigned int exponent)
{
	// == verifySquareOrThrow
	if(!mat.isSquare())
		throw std::domain_error("Non-square Matrix cannot use power^ operation.");
	
    if (mat._pow == nullptr)
        mat._pow = new Matrix<double>(mat);
	return (*mat._pow ^= exponent);
}


#endif /* Matrix_hpp */
