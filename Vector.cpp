//
//  Vector.cpp
//  Maths
//
//  Created by Ashish Aring on 13.05.21.
//  Copyright © 2021 Sphereless. All rights reserved.
//

#include "Vector.hpp"


// static members

template <typename T>
const bool      Vector<T>::_sIsTypeDouble    = true;    // divisible for type double only
template <typename T>
const bool      Vector<T>::_sIsNotTypeDouble = false;
template <typename T>
const bool      Vector<T>::_sIsTypeUInt      = true;    // crossable only if not(!) an UInts
template <typename T>
const bool      Vector<T>::_sIsNotTypeUInt   = false;


// constructor

template <typename T>
Vector<T>::Vector               (bool isDivisble,
                                 bool isUnsignedInt,
                                 unsigned int len,
                                 bool isCol,
                                 T value)
            :   _data           (nullptr),
                _isColumn       (isCol),
                _permitReorient (false),
                _hasReoriented  (false),
                _size           (len),
                _isDivisible    (isDivisble),
                _isDottable     (_size == 3),
                _isCrossable    (!isUnsignedInt && _size==3),
                _mAdd           (nullptr),
                _mSubstr        (nullptr),
                _mMult          (nullptr),
                _mCross         (nullptr),
                _mExtraction    (nullptr),
                _logger         (nullptr)
{
    if (_size == 0)
        throw std::invalid_argument("Size of Vector must be greater than 0.");
    
    try {
        _logger = &LoggerFactory::getLoggerInstance();
        _data = new T[len];
    } catch (const std::runtime_error& e) {
        throw std::bad_alloc();
    } catch (const std::bad_alloc e) {
        _logger->error(typeid(this).name(), "Could not allocate Vector of length "+std::to_string(len));
        delete this;
        throw std::bad_alloc();
    }
    
    this->reset(value);
}

// specialised constructor for type double

template <>
Vector<double>::Vector          (unsigned int len,
                                 bool isCol,
                                 double value)
            :   Vector          (_sIsTypeDouble,
                                 _sIsNotTypeUInt,
                                 len,
                                 isCol,
                                 value)
{
    /* empty */
}

// specialised constructor for type unsigned int

template <>
Vector<unsigned int>::Vector    (unsigned int len,
                                 bool isCol,
                                 unsigned int value)
                    : Vector    (_sIsNotTypeDouble,
                                 _sIsTypeUInt,
                                 len,
                                 isCol,
                                 value)
{
    /* empty */
}

// generic constructor

template <typename T>
Vector<T>::Vector               (unsigned int len,
                                 bool isCol,
                                 T value)
                    : Vector    (_sIsNotTypeDouble,
                                 _sIsNotTypeUInt,
                                 len,
                                 isCol,
                                 value)
{
    /* empty */
}

template <typename T>
Vector<T>::Vector			(const Vector<T>& vec)
         :	Vector		    (vec._size,
                             vec._isColumn,
                             (T)0)
{
	operator = (vec);
}


template <typename T>
Vector<T>::~Vector			()
{
    if (_data)  delete _data;
    
	if (_mAdd)   delete _mAdd;
    if (_mSubstr)delete _mSubstr;
    if (_mMult)  delete _mMult;
    if (_mCross) delete _mCross;
    if (_mExtraction) delete _mExtraction;
    
    _data = nullptr;
    _mAdd = nullptr;
    _mSubstr = nullptr;
    _mMult = nullptr;
    _mCross = nullptr;
    _mExtraction = nullptr;
	_logger = nullptr;
}



// gets

template <typename T>
unsigned int	Vector<T>::getLen				() const	{ return _size; }
template <typename T>
bool			Vector<T>::isColVector			() const	{ return _isColumn; }
template <typename T>
bool			Vector<T>::isDivisible			() const	{ return _isDivisible; }
template <typename T>
bool			Vector<T>::isDottable			() const	{ return _isDottable; }
template <typename T>
bool			Vector<T>::isCrossable			() const	{ return (_isCrossable && _isColumn);	}
template <typename T>
bool			Vector<T>::isSameSize			(const Vector<T> &vec)	const	{ return _size == vec._size; }
template <typename T>
bool			Vector<T>::isSameOrient			(const Vector<T> &vec)	const	{ return _isColumn == vec._isColumn; }
template <typename T>
bool			Vector<T>::isSameSizeAndOrient	(const Vector<T> &vec)	const	{ return (isSameSize(vec) && isSameOrient(vec)); }
template <typename T>
bool			Vector<T>::isReorientPermitted	() const	{ return _permitReorient;	}
template <typename T>
bool			Vector<T>::hasReoriented		() const	{ return _hasReoriented;	}
template <typename T>
double            Vector<T>::getNorm                    ()
{
    double norm = 0.;
    for (unsigned int i=0; i < _size; i++)
        norm += pow(_data[i], 2.);
    
    return sqrt(norm);
}


//	operators

template <typename T>
T&				Vector<T>::operator	[]			(unsigned int index)	const
{
	if (index >= _size)
		throw std::out_of_range("Vector element-index out-of-bounds.");
	
	return _data[index];
}

template <typename T>
Vector<T>&		Vector<T>::operator	=			(const Vector<T>& vec)
{
	// align orientations, if allowed
	this->adjustOrientation(vec._isColumn);
	this->verifyBothOrThrow(vec, "=");
	
	for (unsigned int i=0; i < _size; ++i)
		_data[i] = vec._data[i];
	return *this;
}

template <typename T>
Vector<T>&		Vector<T>::operator	~			()
{
	_isColumn = !_isColumn;
	return *this;
}

/**
 *		Equals== template operators
 *
 *		Specialised implementation for Doubles.
 */
template <typename T>
bool			Vector<T>::operator	==			(const Vector<T>& vec)	const
{
	if (!this->isSameSizeAndOrient(vec))
		return false;
	
	bool isEq = true;
	for (unsigned int i=0; i < _size && isEq; i++)
		isEq = vec._data[i] == _data[i];
	return isEq;
}
template <>
bool			Vector<double>::operator ==		(const Vector<double>& vec)	const
{
	if (!this->isSameSizeAndOrient(vec))
		return false;
	
	bool isEq = true;
	for (unsigned int i=0; i < _size && isEq; i++)
		isEq = fabs(vec._data[i] - _data[i]) < Constants::Accuracy::EPS;
	return isEq;
}
template <typename T>
bool			Vector<T>::operator	==			(const T& val)	const
{
	bool isEq = true;
	for (unsigned int i=0; i < _size && isEq; i++)
		isEq = _data[i] == val;
	return isEq;
}
template <>
bool			Vector<double>::operator==		(const double& val)	const
{
	bool isEq = true;
	for (unsigned int i=0; i < _size && isEq; i++)
		isEq = fabs(_data[i]-val) < Constants::Accuracy::EPS;
	return isEq;
}


/**
 *		Unequals!= template operators
 */
template <typename T>
bool			Vector<T>::operator	!=			(const Vector<T>& vec)	const
{
	return !(operator == (vec));
}

template <typename T>
bool			Vector<T>::operator	!=			(const T& val)	const
{
	return !(operator == (val));
}


/**
 *		PlusEquals+= template-operator
 */
template <typename T>
Vector<T>&		Vector<T>::operator	+=			(const Vector<T>& vec)
{
    this->verifyBothOrThrow(vec, "+=");
	
	for (unsigned int i=0; i < _size; ++i)
		_data[i] += vec._data[i];
	return *this;
}

/**
 *		MinusEquals-= template-operator
 *
 *		Not allowed for UInt
 */
template <typename T>
Vector<T>&		Vector<T>::operator	-=			(const Vector<T>& vec)
{
    this->verifyBothOrThrow(vec, "-=");
	
	for (unsigned int i=0; i < _size; ++i)
		_data[i] -= vec._data[i];
	return *this;
}


template <>
Vector<unsigned int>&	Vector<unsigned int>::operator-=	(const Vector<unsigned int>& vec)
{
	throw std::bad_typeid();
}


/**
 *		MultiplyEquals*= template-operator
 */
template <typename T>
Vector<T>&		Vector<T>::operator	*=			(const T factor)
{
	for (unsigned int i=0; i < _size; ++i)
		_data[i] *= factor;
	return *this;
}


/**
 *		DivideEquals/= template-operator
 *
 *		Only allowed for doubles
 */
template <typename T>
Vector<T>&			Vector<T>::operator			/=		(const T divisor)
{
	if ((divisor >= -Constants::Accuracy::EPS)
        && (divisor < Constants::Accuracy::EPS))
		throw std::invalid_argument("Division by zero");
	
	for (unsigned int i=0; i < _size; i++)
		_data[i] /= divisor;
	return *this;
	
}

template <>
Vector<unsigned int>&	Vector<unsigned int>::operator	/=		(const unsigned int divisor)
{
	throw std::bad_typeid();
}

template <>
Vector<int>&		Vector<int>::operator	/=		(const int divisor)
{
	throw std::bad_typeid();
}

/**
 *        DivideEquals /= template-operator
 *
 *        Not allowed for UInts, Ints
 */
template <typename T>
Vector<T>&                  Vector<T>::operator             /=        (const Vector<T>& vec)
{
    this->verifyBothOrThrow(vec, "/=");
    
    for (unsigned int i=0; i < _size; ++i)
        _data[i] /= vec._data[i];
    return *this;
}

template <>
Vector<unsigned int>&       Vector<unsigned int>::operator  /=        (const Vector<unsigned int>& vec)
{
    throw std::bad_typeid();
}

template <>
Vector<int>&                Vector<int>::operator           /=        (const Vector<int>& vec)
{
    throw std::bad_typeid();
}


// methods

template <typename T>
void			Vector<T>::adjustOrientation		(bool isColumn)	{
	if (!_permitReorient) return;
	
	_hasReoriented = (_isColumn != isColumn);
	_isColumn = isColumn;
	
	if (_hasReoriented)
		_logger->deep(typeid(this).name(), "Adjusted vector-orientation");
}
template <typename T>
void			Vector<T>::verifySizeOrThrow	    (const Vector<T> &vec,
                                                     std::string operation) const
{
	if (!this->isSameSize(vec)) {
        std::ostringstream msg;
        msg << "Incompatible vector-length in operation (" << operation << "): " << this->_size << " vs " << vec.getLen();
        _logger->error(typeid(this).name(), msg.str());
        throw std::invalid_argument(msg.str());
	}
}

template <typename T>
void			Vector<T>::verifyOrientOrThrow	(const Vector<T> &vec,
                                                 std::string operation) const
{
	if (!this->isSameOrient(vec)) {
        std::ostringstream msg;
        msg << "Incompatible Vector-orientation in operation (" << operation << "): " << _isColumn << " vs " << !_isColumn;
		_logger->error(typeid(this).name(), msg.str());
		throw std::invalid_argument(msg.str());
	}
}

template <typename T>
void			Vector<T>::verifyBothOrThrow	(const Vector<T> &vec,
                                                 std::string operation) const
{
	// Important to keep this order, changes orientation only if sizes match
    this->verifySizeOrThrow(vec, operation);
    this->verifyOrientOrThrow(vec, operation);
}

template <typename T>
void			Vector<T>::allowReorient		()	{
	_permitReorient = true;
	_hasReoriented = false;
}

template <typename T>
void            Vector<T>::normalise            () {
    this->operator /= (this->getNorm());
}

template <typename T>
void			Vector<T>::reset				(T value)
{
	for (unsigned int i=0; i < _size; ++i)
		_data[i] = value;
}
template <typename T>
void			Vector<T>::print				(std::ostream& os,
												 unsigned int indent,
												 unsigned int precision) const
{
	os << std::right << std::setfill(' ') << std::fixed << std::setprecision(precision);
	
	if (_isColumn)
		for (unsigned int i=0; i < _size; ++i) {
			os << std::setw(indent) << Constants::Outputs::INTROCHAR << " "
				<< std::setw(Constants::Outputs::WIDTHVALUE) << _data[i];
			if (i < _size-1) os << std::endl;
		}
	else
	{
		os << std::setw(indent) << Constants::Outputs::INTROCHAR << " ";
		for (unsigned int i=0; i < _size; ++i)
			os << std::setw(Constants::Outputs::WIDTHVALUE) << _data[i] << " ";
	}
}

template class Vector<unsigned int>;
template class Vector<int>;
template class Vector<double>;


