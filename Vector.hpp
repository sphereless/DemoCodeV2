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
#include <iomanip>
#include <cmath>

#include "Constants.hpp"


template <typename T>
class Vector
{
private:
	
	// members
	T*								_data;					// have this declared first, to reduce memory offset
	bool							_isColVec;
	const unsigned int				_len;
	const bool						_isUInt;
	const bool						_isInt;
	
	// methods
	bool				isCrossable	() const;
	
public:
	
	// constructor
						Vector			(unsigned int len, T value = (T)0, bool isCol = true);
						Vector			(const Vector<T>& vec);
						~Vector			();
	
	// gets
	unsigned int		getLen			() const;
	bool				isColVector		() const;
	bool				isDivisible		() const;
	
	
	// operator
	Vector<T>&			operator =		(const Vector<T>&);
	T&					operator []		(unsigned int index)	const;
	
	bool				operator ==		(const Vector<T>&)		const;
	bool				operator !=		(const Vector<T>&)		const;
	
	Vector<T>&			operator +=		(const Vector<T>&);
	Vector<T>&			operator -=		(const Vector<T>&);
	//Vector<T>&		operator *=		(const Vector<T>&);		// to add ??? unless dot-product or len==1, Vectors cannot be multiplied.
	//Matrix&			operator *=		(const Vector<T>&);		// to add ???
	Vector<T>&			operator *=		(int factor);
	Vector<T>&			operator *=		(double factor);
	Vector<T>&			operator /=		(int divisor);
	Vector<T>&			operator /=		(double divisir);
	
	// methods
	virtual void		reset			(T value = (T)0);
	virtual void		print			() const;
	
	virtual Vector<T>&	transpose		();
	virtual Vector<T>&	x				(const Vector<T>&);
};


// non-member operators

template <typename T>
Vector<T>&			operator *			(Vector<T> vec, int factor)
{
	Vector<T>* res = new Vector<T>(vec.getLen());
	for (unsigned int i=0; i < vec.getLen(); i++)
		(*res)[i] = (vec[i]) * factor;
	return *res;
}

template <typename T>
Vector<T>&			operator *			(int factor, Vector<T> vec)
{
	Vector<T>* res = new Vector<T>(vec.getLen());
	for (unsigned int i=0; i < vec.getLen(); i++)
		(*res)[i] = (vec[i]) * factor;
	return *res;
}


#endif /* Vector_hpp */
