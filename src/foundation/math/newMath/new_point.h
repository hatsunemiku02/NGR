
#pragma once
#include "math/float4.h"
#include "math/vector.h"

//------------------------------------------------------------------------------
namespace Math
{
class point;
// win32 VC++ does not support passing aligned objects as value so far
// here is a bug-report at microsoft saying so:
// http://connect.microsoft.com/VisualStudio/feedback/ViewFeedback.aspx?FeedbackID=334581
typedef const point& __PointArg;
 

class point : public float4
{
public:
    /// default constructor
    point();
    /// construct from components
    point(scalar x, scalar y, scalar z);
    /// construct from float4
    point(const float4& rhs);
    /// !!!! copy constructor forbidden, otherwise passing point's to a function
    /// !!!! via Registers doesnt work
    //point(const point& rhs);
	/// construct from Ogre::Vector4
	point(Ogre::Vector4 rhs);
    /// return a point at the origin (0, 0, 0)
    static point origin();
    /// assignment operator
    void operator=(const point& rhs);
    /// assign Ogre::Vector4
    void operator=(Ogre::Vector4 rhs);
    /// inplace add vector
    void operator+=(const vector& rhs);
    /// inplace subtract vector
    void operator-=(const vector& rhs);
    /// add point and vector
    point operator+(const vector& rhs) const;
    /// subtract vectors from point
    point operator-(const vector& rhs) const;
    /// subtract point from point into a vector
    vector operator-(const point& rhs) const;
    /// equality operator
    bool operator==(const point& rhs) const;
    /// inequality operator
    bool operator!=(const point& rhs) const;
    /// set components
    void set(scalar x, scalar y, scalar z);
};

//------------------------------------------------------------------------------
/**
*/
__forceinline
point::point() :
    float4(0.0f, 0.0f, 0.0f, 1.0f)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
__forceinline
point::point(scalar x, scalar y, scalar z) :
    float4(x, y, z, 1.0f)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
__forceinline
point::point(const float4& rhs) :
    float4(rhs)
{
    this->set_w(1.0f);
}

//------------------------------------------------------------------------------
/**
*/
__forceinline
point::point(Ogre::Vector4 rhs) :
    float4(rhs)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
__forceinline point
point::origin()
{
    return point(0.0f, 0.0f, 0.0f);
}

//------------------------------------------------------------------------------
/**
*/
__forceinline void
point::operator=(const point& rhs)
{
    this->vec = rhs.vec;
}

//------------------------------------------------------------------------------
/**
*/
__forceinline void
point::operator+=(const vector& rhs)
{
	this->vec+= rhs.vec;
}

//------------------------------------------------------------------------------
/**
*/
__forceinline void
point::operator-=(const vector& rhs)
{
	this->vec-= rhs.vec;
}

//------------------------------------------------------------------------------
/**
*/
__forceinline point
point::operator+(const vector& rhs) const
{
	return this->vec + rhs.vec;
}

//------------------------------------------------------------------------------
/**
*/
__forceinline point
point::operator-(const vector& rhs) const
{
	return this->vec - rhs.vec;
}

//------------------------------------------------------------------------------
/**
*/
__forceinline vector
point::operator-(const point& rhs) const
{
	return this->vec - rhs.vec;
}

//------------------------------------------------------------------------------
/**
*/
__forceinline bool
point::operator==(const point& rhs) const
{
	return vec == rhs.vec;
}

//------------------------------------------------------------------------------
/**
*/
__forceinline bool
point::operator!=(const point& rhs) const
{
	return vec != rhs.vec;
}    

//------------------------------------------------------------------------------
/**
*/
__forceinline void
point::set(scalar x, scalar y, scalar z)
{
    float4::set(x, y, z, 1.0f);
}
} // namespace Math
//------------------------------------------------------------------------------
