#pragma once

#include <cmath>
#include <GL/glew.h>
#include <iostream>

namespace cagd
{
    //--------------------------------------
    // 3-dimensional homogeneous coordinates
    //--------------------------------------
    class HCoordinate3
    {
    protected:
	GLfloat _data[4]; // x, y, z, w;

    public:
	// default constructor
	HCoordinate3();

	// special constructor
	HCoordinate3(GLfloat x, GLfloat y, GLfloat z = 0.0, GLfloat w = 1.0);

	// homework: get components by value
	GLfloat operator[](GLuint rhs) const;
	GLfloat x() const;
	GLfloat y() const;
	GLfloat z() const;
	GLfloat w() const;

	// homework: get components by reference
	GLfloat& operator[](GLuint rhs);
	GLfloat& x();
	GLfloat& y();
	GLfloat& z();
	GLfloat& w();

	// add
	const HCoordinate3 operator +(const HCoordinate3& rhs) const;

	// homework: add to this
	HCoordinate3& operator +=(const HCoordinate3& rhs);

	// homework: subtract
	const HCoordinate3 operator -(const HCoordinate3& rhs) const;

	// homework: subtract from this
	HCoordinate3& operator -=(const HCoordinate3& rhs);

	// homework: dot product
	GLfloat operator *(const HCoordinate3& rhs) const;

	// homework: cross product
	const HCoordinate3 operator ^(const HCoordinate3& rhs) const;

	// homework: cross product with this
	HCoordinate3& operator ^=(const HCoordinate3& rhs);

	// homework: multiplicate with scalar from right
	const HCoordinate3 operator *(GLfloat rhs) const;

	// homework: multiplicate this with a scalar
	HCoordinate3& operator *=(GLfloat rhs);

	// homework: divide with scalar
	const HCoordinate3 operator /(GLfloat rhs) const;

	// homework: divide this with a scalar
	HCoordinate3& operator /=(GLfloat rhs);

	// homework: length of vector represented by this homogeneous coordinate
	GLfloat length() const;

	// homework: normalize
	HCoordinate3& normalize();
    };

    // default constructor
    inline HCoordinate3::HCoordinate3()
    {
	_data[0] = _data[1] = _data[2] = 0.0;
	_data[3] = 1.0;
    }

    // special constructor
    inline HCoordinate3::HCoordinate3(GLfloat x, GLfloat y, GLfloat z, GLfloat w)
    {
	_data[0] = x;
	_data[1] = y;
	_data[2] = z;
	_data[3] = w;
    }

    // get components by value
    inline GLfloat HCoordinate3::operator[](GLuint rhs) const
    {
	return _data[rhs];
    }
    inline GLfloat HCoordinate3::x() const
    {
	return _data[0];
    }
    inline GLfloat HCoordinate3::y() const
    {
	return _data[1];
    }
    inline GLfloat HCoordinate3::z() const
    {
	return _data[2];
    }
    inline GLfloat HCoordinate3::w() const
    {\
	return _data[3];
    }

    // get components by reference
    inline GLfloat& HCoordinate3::operator[](GLuint rhs)
    {
	return _data[rhs];
    }
    inline GLfloat& HCoordinate3::x()
    {
	return _data[0];
    }
    inline GLfloat& HCoordinate3::y()
    {
	return _data[1];
    }
    inline GLfloat& HCoordinate3::z()
    {
	return _data[2];
    }
    inline GLfloat&HCoordinate3:: w()
    {
	return _data[3];
    }

    // add
    inline const HCoordinate3 HCoordinate3::operator +(const HCoordinate3& rhs) const
    {
	return HCoordinate3(
		    rhs.w() * x() + w() * rhs.x(),
		    rhs.w() * y() + w() * rhs.y(),
		    rhs.w() * z() + w() * rhs.z(),
		    w() * rhs.w());
    }

    // add to this
    inline HCoordinate3& HCoordinate3::operator +=(const HCoordinate3& rhs)
    {
	_data[0] = rhs.w() * x() + w() * rhs.x();
	_data[1] = rhs.w() * y() + w() * rhs.y();
	_data[2] = rhs.w() * z() + w() * rhs.z();
	_data[3] = w() * rhs.w();
	return *this;
    }

    // subtract
    inline const HCoordinate3 HCoordinate3::operator -(const HCoordinate3& rhs) const
    {
	return HCoordinate3(
		    rhs.w() * x() - w() * rhs.x(),
		    rhs.w() * y() - w() * rhs.y(),
		    rhs.w() * z() - w() * rhs.z(),
		    w() * rhs.w());
    }

    // subtract to this
    inline HCoordinate3& HCoordinate3::operator -=(const HCoordinate3& rhs)
    {
	_data[0] = rhs.w() * x() - w() * rhs.x();
	_data[1] = rhs.w() * y() - w() * rhs.y();
	_data[2] = rhs.w() * z() - w() * rhs.z();
	_data[3] = w() * rhs.w();
	return *this;
    }

    // dot product
    inline GLfloat HCoordinate3::operator *(const HCoordinate3& rhs) const
    {
	return x() * rhs.x() + y() * rhs.y() + z() * rhs.z() + w() * rhs.w();
    }


    // cross product
    inline const HCoordinate3 HCoordinate3::operator ^(const HCoordinate3& rhs) const{
	return HCoordinate3(
		    _data[1] * rhs._data[2] - _data[2] * rhs._data[1],
		_data[2] * rhs._data[0] - _data[0] * rhs._data[2],
		_data[0] * rhs._data[1] - _data[1] * rhs._data[0],
		_data[3] * rhs._data[3]);
    }

    // cross product with this
    inline HCoordinate3& HCoordinate3::operator ^=(const HCoordinate3& rhs)
    {
	GLfloat x = _data[1] * rhs._data[2] - _data[2] * rhs._data[1];
	GLfloat y = _data[2] * rhs._data[0] - _data[0] * rhs._data[2];
	GLfloat z = _data[0] * rhs._data[1] - _data[1] * rhs._data[0];
	GLfloat w = _data[3] * rhs._data[3];

	_data[0] = x;
	_data[1] = y;
	_data[2] = z;
	_data[3] = w;
	return *this;
    }

    // multiplicate with scalar from right
    inline const HCoordinate3 HCoordinate3::operator *(GLfloat rhs) const
    {
	return HCoordinate3(_data[0] * rhs, _data[1] * rhs, _data[2] * rhs, _data[3] * rhs);
    }

    // multiplicate this with a scalar
    inline HCoordinate3& HCoordinate3::operator *=(GLfloat rhs){
	_data[0] *= rhs;
	_data[1] *= rhs;
	_data[2] *= rhs;
	_data[3] *= rhs;

	return *this;
    }

    // divide with scalar from right
    inline const HCoordinate3 HCoordinate3::operator /(GLfloat rhs) const
    {
	return HCoordinate3(_data[0] / rhs, _data[1] / rhs, _data[2] / rhs, _data[3] / rhs);
    }

    // divide this with a scalar
    inline HCoordinate3& HCoordinate3::operator /=(GLfloat rhs){
	_data[0] /= rhs;
	_data[1] /= rhs;
	_data[2] /= rhs;
	_data[3] /= rhs;
	return *this;
    }

    // homework: scale from left with a scalar
    inline const HCoordinate3 operator *(GLfloat lhs, const HCoordinate3& rhs)
    {
	return HCoordinate3(rhs[0] * lhs, rhs[1] * lhs, rhs[2] * lhs, rhs[3] * lhs);
    }

    // length of vector represented by this homogeneous coordinate
    inline GLfloat HCoordinate3::length() const
    {
	return std::sqrt((*this) * (*this));
    }

    // normalize
    inline HCoordinate3& HCoordinate3::normalize()
    {
	GLdouble l = length();

	if (l && l != 1.0)
	    *this /= l;

	return *this;
    }
    // homework: output to stream
    inline std::ostream& operator <<(std::ostream& lhs, const HCoordinate3& rhs)
    {
	return lhs << rhs[0] << " " << rhs[1] << " " << rhs[2] << " " << rhs[3];
    }
    // homework: input from stream
    inline std::istream& operator >>(std::istream& lhs, HCoordinate3& rhs)
    {
	return lhs >> rhs[0] >> rhs[1] >> rhs[2] >> rhs[3];
    }
}
