
#pragma once
#ifndef MATH_RECTANGLE_H
#define MATH_RECTANGLE_H
//------------------------------------------------------------------------------
/**
    @class Math::rectangle

    A 2d rectangle class.
*/
#include "core/types.h"
#include "clipstatus.h"

//------------------------------------------------------------------------------
namespace Math
{
template<class TYPE> class rectangle
{
public:
    /// default constructor
    rectangle();
    /// constructor 1
    rectangle(TYPE l, TYPE t, TYPE r, TYPE b);
    /// set content
    void set(TYPE l, TYPE t, TYPE r, TYPE b);
    /// return true if point is inside
    bool inside(TYPE x, TYPE y) const;
    /// return width
    TYPE width() const;
    /// return height
    TYPE height() const;
    /// return center x
    TYPE centerX() const;
    /// return center y
    TYPE centerY() const;

	ClipStatus::Type clipstatus(const rectangle& other) const;

    TYPE left;
    TYPE top;
    TYPE right;
    TYPE bottom;
};

//------------------------------------------------------------------------------
/**
*/
template<class TYPE>
ClipStatus::Type Math::rectangle<TYPE>::clipstatus(const rectangle& rh) const
{
	if(left>rh.right||right<rh.left||bottom>rh.top||top<rh.bottom)
	{
		return ClipStatus::Outside;
	}
	else if(left<rh.left&&right>rh.right&&bottom<rh.bottom&&top>rh.top)
	{
		return ClipStatus::Inside;
	}
	else
	{
		return ClipStatus::Clipped;
	}
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE>
rectangle<TYPE>::rectangle()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE>
rectangle<TYPE>::rectangle(TYPE l, TYPE t, TYPE r, TYPE b) :
    left(l),
    top(t),
    right(r),
    bottom(b)
{
    assert(this->left <= this->right);
    assert(this->top <= this->bottom);
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE> void
rectangle<TYPE>::set(TYPE l, TYPE t, TYPE r, TYPE b)
{
    n_assert(l <= r);
    n_assert(t <= b);
    this->left = l;
    this->top = t;
    this->right = r;
    this->bottom = b;
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE> bool
rectangle<TYPE>::inside(TYPE x, TYPE y) const
{
    return (this->left <= x) && (x <= this->right) && (this->top <= y) && (y <= this->bottom);
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE> TYPE
rectangle<TYPE>::width() const
{
    return this->right - this->left;
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE> TYPE
rectangle<TYPE>::height() const
{
    return this->bottom - this->top;
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE> TYPE
rectangle<TYPE>::centerX() const
{
    return (this->left + this->right) / 2;
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE> TYPE
rectangle<TYPE>::centerY() const
{
    return (this->top + this->bottom) / 2;
}
} // namespace Math
//------------------------------------------------------------------------------
#endif



