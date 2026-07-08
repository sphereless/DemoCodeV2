//
//  Vector.hpp
//  Maths
//
//  Created by Ashish Aring on 13.05.21.
//  Copyright © 2021 Sphereless. All rights reserved.
//

#ifndef Vector_hpp
#define Vector_hpp


#include <iostream>
#include <sstream>
#include <iomanip>
#include <cmath>

#include "Constants.hpp"
#include "LoggerFactory.hpp"


// forward declarations
template <typename T> class Vector;
template <typename T> Vector<T>&  operator +            (const Vector<T>& vec1,
                                                         const Vector<T>& vec2);
template <typename T> Vector<T>&  operator -            (const Vector<T>& vec1,
                                                         const Vector<T>& vec2);
template <typename T> Vector<T>&  operator *            (const Vector<T>& vec1,
                                                         T factor);
template <typename T> Vector<T>&  operator *            (T factor,
                                                         const Vector<T>& vec1);
template <typename T> Vector<T>&  operator *            (const Vector<T>& vec1,
                                                         const Vector<T>& vec2);
template <typename T> Vector<T>&  operator /            (const Vector<T>& vec1,
                                                         T factor);
template <typename T> Vector<T>&  operator /            (const Vector<T>& vec1,
                                                         const Vector<T>& vec2);
template <typename T> std::ostream& operator <<         (std::ostream& os,
                                                         const Vector<T>& vec);
/**
 *		Implements 1D-array supporting vector-operations.
 *
 *		Objects are modifiable via plus, minus, mult, divide operators and also via special
 *		operators transpose ~, cross-product mult , dot product |.
 *      Access indiviual vector-elements via brackets-operator []  for reading and writing.
 *
 *		Matching sizes and orientation are required for assignment via equals operator (=).
 *		On-the-fly adjustment of orientation (only) is permitted if property 'allowReorient()' is true.
 *		That is, for square matrices. Not permitted by default.
 *
 *		Dynamically allocates memory upon construction.
 *
 *		Exceptions:	std::length_error, std::out_of_range, ...
 *
 *		TODO:	-	try using std::vector<T>
 *				-	add operator * (Vector, Vector) to return Matrix?
 *				-	add 'toMatrix()' method returning Matrix<T>&
 *				-	avoid call to non-member operator for derived class ?!
 */
template <typename T>
class Vector
{
private:
	// static members
    static const bool   _sIsTypeDouble;
    static const bool   _sIsNotTypeDouble;
    static const bool   _sIsTypeUInt;
    static const bool   _sIsNotTypeUInt;
    
	// members
	T*					_data;					// have this declared first, to reduce memory offset
	bool				_isColumn;
	bool				_permitReorient;
	bool				_hasReoriented;
	const unsigned int	_size;
	const bool			_isDivisible;
	const bool			_isDottable;
	const bool			_isCrossable;
	
    // mutable members
    mutable Vector<T>   *_mAdd;
    mutable Vector<T>   *_mSubstr;
    mutable Vector<T>   *_mMult;
    mutable Vector<T>   *_mCross;
    mutable Vector<T>   *_mExtraction;
    
    // friend operators
    friend Vector<T>&   operator +    <>    (const Vector<T>& vec1,
                                             const Vector<T>& vec2);
    friend Vector<T>&   operator -    <>    (const Vector<T>& vec1,
                                             const Vector<T>& vec2);
    friend Vector<T>&   operator *    <>    (const Vector<T>& vec,
                                             T factor);
    friend Vector<T>&   operator *    <>    (T factor,
                                             const Vector<T>& vec);
    friend Vector<T>&   operator *    <>    (const Vector<T>& vec1,
                                             const Vector<T>& vec2);
    friend Vector<T>&   operator /    <>    (const Vector<T>& vec,
                                             T factor);
    friend Vector<T>&   operator /    <>    (const Vector<T>& vec1,
                                             const Vector<T>& vec2);
    friend std::ostream& operator <<  <>    (std::ostream& os,
                                             const Vector<T>& vec);

    // constructor
                        Vector              (bool isDivisible,
                                             bool isUnsignedInt,
                                             unsigned int len,
                                             bool isColumn,
                                             T value);
    
	// methods
	void				verifySizeOrThrow	(const Vector<T>& vec, std::string operation) const;
	void				verifyOrientOrThrow	(const Vector<T>& vec, std::string operation) const;
	void				verifyBothOrThrow	(const Vector<T>& vec, std::string operation) const;
	void				adjustOrientation	(bool isColumn);
	
protected:
	
	ILogger*			_logger;
	
public:

	// constructor
						Vector				(unsigned int len,
                                             bool isColumn = true,
                                             T value = (T)0);
						Vector				(const Vector<T>& vec);
	virtual			    ~Vector				();
	
	// operators
	T&					operator []			(unsigned int index)	const;
	Vector<T>&			operator =			(const Vector<T>&);					// may transpose, if permitted
	Vector<T>&			operator ~			();									// transpose (on purpose)
	
	bool				operator ==			(const Vector<T>&)		const;
	bool				operator ==			(const T&		 )		const;
	bool				operator !=			(const Vector<T>&)		const;
	bool				operator !=			(const T&		 )		const;
	
	Vector<T>&			operator +=			(const Vector<T>&);
	Vector<T>&			operator -=			(const Vector<T>&);
	Vector<T>&			operator *=			(const T factor);
	Vector<T>&			operator /=			(const T divisor);
    Vector<T>&          operator /=         (const Vector<T>& divisor);
	
	// gets
	unsigned int		getLen				() const;
    double              getNorm             ();                                    // euclidean length
	bool				isColVector			() const;
	bool				isDivisible			() const;
	bool				isDottable			() const;
	bool				isCrossable			() const;
	bool				isSameSize			(const Vector<T>& vec) const;
	bool				isSameOrient		(const Vector<T>& vec) const;
	bool				isSameSizeAndOrient	(const Vector<T>& vec) const;
	bool				isReorientPermitted	() const;
	bool				hasReoriented		() const;
	
	// methods
	void				allowReorient		();
    void                normalise           ();
	virtual void		reset				(T value = (T)0);
	virtual void		print				(std::ostream& os = std::cout,
											 unsigned int indent = Constants::Outputs::TEXTINDENT,
											 unsigned int precision = Constants::Outputs::LOWPRECISION) const;
};


//	Non-member operators

/**
 *		Implements non-member plus operator +
 */
template <typename T>
inline Vector<T>&			operator +			(const Vector<T>& vec1,
                                                 const Vector<T>& vec2)
{
//    if (!vec1.isSameSizeAndOrient(vec2))
//        throw std::
    if (vec1._mAdd == nullptr)
        vec1._mAdd = new Vector<T>(vec1);
    
    *vec1._mAdd = vec1;
    return (*vec1._mAdd += vec2);
}

/**
 *		Implements non-member minus operator -
 *
 *		with UInt specialisation
 */
template <typename T>
inline Vector<T>&			operator -			(const Vector<T>& vec1,
                                                 const Vector<T>& vec2)
{
    if (vec1._mSubstr == nullptr)
        vec1._mSubstr = new Vector<T>(vec1);
    
    *vec1._mSubstr = vec1;
    return (*vec1._mSubstr -= vec2);
}

template <>
inline Vector<unsigned int>&	operator -		(const Vector<unsigned int>& vec1,
                                                 const Vector<unsigned int>& vec2)
{
	throw std::bad_typeid();
}

/**
 *		Implements non-member multiply operator *
 */
template <typename T>
inline Vector<T>&			operator *			(const Vector<T>& vec,
                                                 T factor)
{
    if (vec._mMult == nullptr)
        vec._mMult = new Vector<T>(vec);
    *vec._mMult = vec;
	return (*vec._mMult *= factor);
}

template <typename T>
inline Vector<T>&			operator *			(T factor,
                                                 const Vector<T>& vec)
{
	return (operator * (vec, factor));
}

/**
 *		Implements non-member divide operator /
 *
 *		Important:
 *		Only permitted for template T == double !
 *
 *		Currently has two different implementations.
 *		1. Divide each element by one common divisor
 *		2. Divide element-wise with individual divisors
 */
template <typename T>
inline Vector<T>&			operator /			(const Vector<T>& vec,
                                                 T divisor)
{
	throw std::bad_typeid();
}

template <>
inline Vector<double>&		operator /			(const Vector<double>& vec,
                                                 double divisor)
{
	Vector<double>* res = new Vector<double>(vec);
	return ((*res) /= divisor);
}

template <typename T>
inline Vector<T>&			operator /			(const Vector<T>& vec,
                                                 Vector<T>& divisor)
{
	throw std::bad_typeid();
}

template <>
inline Vector<double>&		operator /			(const Vector<double>& vec,
                                                 Vector<double>& divisor)
{
	// verify same length
	if (vec.getLen() != divisor.getLen()) {
		std::ostringstream os;
		os << "Division vector has incompatible length " << divisor.getLen() << " (expected: " << vec.getLen() << ")";
		throw std::invalid_argument(os.str());
	}
	
	Vector<double>* res = new Vector<double>(vec);
	for (unsigned int i=0; i < vec.getLen(); i++)
		(*res)[i] /= (divisor)[i];
	
	return *res;
}

/**
 *		Implements non-member operator multiply for cross-products
 *
 *		Important:
 *		Not permitted for UInts !
 */
template <typename T>
inline Vector<T>&			operator *			(const Vector<T>& vec1,
                                                 const Vector<T>& vec2)
{
	if (!vec1.isCrossable())
		throw std::invalid_argument("First vector incompatible for cross-product, "+std::to_string(vec1.getLen())+".");
	if (!vec2.isCrossable())
		throw std::invalid_argument("Second vector incompatible for cross-product, "+std::to_string(vec2.getLen())+".");
	
    if (vec1._mCross == nullptr)
        vec1._mCross = new Vector<T>(vec1);

	(*vec1._mCross)[0] = vec1[1] * vec2[2] - vec1[2] * vec2[1];
	(*vec1._mCross)[1] = vec1[2] * vec2[0] - vec1[0] * vec2[2];
	(*vec1._mCross)[2] = vec1[0] * vec2[1] - vec1[1] * vec2[0];
	
	return *vec1._mCross;
}

template <>
inline Vector<unsigned int>&	operator *		(const Vector<unsigned int>& vec1,
                                                 const Vector<unsigned int>& vec2)
{
	throw std::bad_typeid();
}

/**
 *		Implements non-member dot-product operator |
 */
template <typename T>
inline T					operator |			(const Vector<T>& vec1,
                                                 const Vector<T>& vec2)
{
	if (vec1.getLen() != vec2.getLen())
		throw std::invalid_argument("Lengths of vectors incompatible for dot-product, "+std::to_string(vec1.getLen())+"/"+std::to_string(vec2.getLen())+".");
	if (!vec1.isColVector())
		throw std::invalid_argument("First vector is not column-vector, required for dot-product.");
	if (!vec2.isColVector())
		throw std::invalid_argument("Second vector is not column-vector, required for dot-product.");
	
	T res = (T)0;
	for (unsigned int i=0; i < vec1.getLen(); i++)
		res += vec1[i]*vec2[i];
	
	return res;
}

/**
 *	Implements non-member extraction-operator <<
 *
 *  Pre-adjusts orientation of extraction helper-vector to argument-vector, if needed.
 *  Post-adjustment to column-vector is currently disabled (as it may not be needed, unclear though).
 */
template <typename T>
inline std::ostream&		operator <<			(std::ostream& os,
                                                 const Vector<T>& vec) {
	if (vec._mExtraction == nullptr)
        vec._mExtraction = new Vector<T>(vec);
    
	if (vec._mExtraction->isColVector() != vec.isColVector()) ~(*vec._mExtraction);
    *vec._mExtraction = vec;
    if (vec._mExtraction->isColVector()) ~(*vec._mExtraction);
    
    std::ostringstream osPrint;
    vec._mExtraction->print(osPrint,
                            Constants::Outputs::TEXTINDENT,          // indent
                            Constants::Outputs::LOWPRECISION);
	os << osPrint.str();
	
	return os;
}

#endif /* Vector_hpp */
