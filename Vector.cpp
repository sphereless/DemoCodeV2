//
//  Vector.cpp
//  Maths
//
//  Created by Ashish Aring on 13.05.21.
//  Copyright © 2021 Sphereless. All rights reserved.
//

#include "Vector.hpp"




// constructor

template <typename T>
Vector<T>::Vector			(unsigned int len, T val, bool isCol)
			:	_data		(nullptr),
				_isColVec	(isCol),
				_len		(len),
				_isUInt		(typeid(T) == typeid(unsigned int)),
				_isInt		(typeid(T) == typeid(int))
{
	try {
		_data = new T[len];
	}
	catch (const std::exception e) {
		std::cout << "A standard exception was caught, with message '" << e.what() << "'\n";
		delete _data;
		_data = nullptr;
	}
	
	reset(val);
}

template <typename T>
Vector<T>::Vector			(const Vector<T>& vec)
			:	_data		(nullptr),
				_isColVec	(vec._isColVec),
				_len		(vec._len),
				_isUInt		(vec._isUInt),
				_isInt		(vec._isInt)
{
	try {
		_data = new T[_len];
	}
	catch (const std::exception e) {
		std::cout << "A standard exception was caught, with message '" << e.what() << "'\n";
		delete _data;
		_data = nullptr;
	}
	
	for (unsigned int i = 0; i < _len; ++i)
		_data[i] = vec._data[i];
}


template <typename T>
Vector<T>::~Vector			()
{
	delete _data;
	_data = nullptr;
}



// gets

template <typename T>
unsigned int	Vector<T>::getLen		() const	{ return _len; }
template <typename T>
bool			Vector<T>::isColVector	() const	{ return _isColVec; }
template <typename T>
bool			Vector<T>::isDivisible	() const	{ return (!_isUInt && !_isInt); }



// operator (member)

template <typename T>
Vector<T>&		Vector<T>::operator	=	(const Vector<T>& vec)
{
	if (this->_len == vec._len)
		for (unsigned int i=0; i < _len; ++i)
			_data[i] = vec._data[i];
	return *this;
}

template <typename T>
T&				Vector<T>::operator	[]	(unsigned int index)	const
{
	if (index >= _len)
	{
		T* res = new T(0);
		return *res;
	}
	
	return _data[index];
}
template <typename T>
bool			Vector<T>::operator	==	(const Vector<T>& vec)	const
{
	if (vec._len != _len)
		return false;
	
	unsigned int i = 0;
	T sum = (T)0;
	
	while (sum < Constants::Times::eps && (i++ < _len))
		sum = (vec._data)[i] - _data[i];
	
	return (sum < Constants::Times::eps);
}
template <typename T>
bool			Vector<T>::operator	!=	(const Vector<T>& vec)	const
{
	return !(Vector<T>::operator==(vec));
}


template <typename T>
Vector<T>&		Vector<T>::operator	+=	(const Vector<T>& vec)
{
	if (vec._len == _len)
		for (unsigned int i=0; i < _len; ++i)
			_data[i] += vec._data[i];
	return *this;
}

template <typename T>
Vector<T>&		Vector<T>::operator	-=	(const Vector<T>& vec)
{
	if (vec._len == _len)
		for (unsigned int i=0; i < _len; ++i)
			_data[i] -= vec._data[i];
	return *this;
}


template <typename T>
Vector<T>&		Vector<T>::operator	*=	(int factor)
{
	/*	Experimental. Without if-branch.
	 
	 factor = _isUInt * (factor < 0) * -factor;
	 0			0			-f		= 0
	 0			1			-f		= 0
	 1			0			-f		= 0
	 1			1			-f		= -f
	 */
	
	if (!_isUInt || factor >= 0)								// was: &&-related. negative factors not surpported for Uints.
		for (unsigned int i=0; i < _len; ++i)
			_data[i] *= factor;
	return *this;
}


template <typename T>
Vector<T>&		Vector<T>::operator	*=	(double factor)
{
	if (!_isUInt && !_isInt)									// dbl-argument not supported for uints, ints
	{
		T fact = (T) factor;
		for (unsigned int i=0; i < _len; ++i)
			_data[i] *= fact;
	}
	return *this;
}

template <typename T>
Vector<T>&		Vector<T>::operator	/=	(int factor)
{
	if (!_isUInt && !_isInt)									// division not supported for uints, ints
	{
		T fact = (T) factor;
		for (unsigned int i=0; i < _len; ++i)
			_data[i] /= fact;
	}
	return *this;
}

template <typename T>
Vector<T>&		Vector<T>::operator	/=	(double factor)
{
	if (!_isUInt && !_isInt)									// division not supported for uints, ints
	{
		T fact = (T) factor;
		for (unsigned int i=0; i < _len; ++i)
			_data[i] /= fact;
	}
	return *this;
}




// methods

template <typename T>
bool			Vector<T>::isCrossable	() const	{	return (_len == 3 && _isColVec);	}

template <typename T>
void			Vector<T>::reset		(T value)
{
	for (unsigned int i=0; i < _len; ++i)
		_data[i] = value;
}
template <typename T>
void			Vector<T>::print		() const
{
	std::cout << std::right << std::setprecision(3) << std::setfill(' ');
	
	if (_isColVec)
		for (unsigned int i=0; i < _len; ++i)
			std::cout << std::setw(10) << _data[i] << std::endl;
	else
	{	for (unsigned int i=0; i < _len; ++i)
		std::cout << std::setw(10) << _data[i];
		std::cout << std::endl;
	}
	std::cout << std::endl;
}


template <typename T>
Vector<T>&		Vector<T>::transpose	()
{
	_isColVec = !_isColVec;
	return *this;
}

template <typename T>
Vector<T>&		Vector<T>::x			(const Vector<T>& vec)
{
	Vector<T>* res = nullptr;
	
	if (!_isUInt && isCrossable() && vec.isCrossable())
	{
		res = new Vector<T>(3);
		(res->_data)[0] = _data[1] * (vec._data)[2] - _data[2] * (vec._data)[1];
		(res->_data)[1] = _data[2] * (vec._data)[0] - _data[0] * (vec._data)[2];
		(res->_data)[2] = _data[0] * (vec._data)[1] - _data[1] * (vec._data)[0];
	}
	return *res;
}

template class Vector<unsigned int>;
template class Vector<int>;
template class Vector<double>;


