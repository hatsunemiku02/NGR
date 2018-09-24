/****************************************************************************
Copyright (c) 2011-2013,WebJet Business Division,CYOU
 
http://

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/

#ifndef __color_H__
#define __color_H__

#include "math/scalar.h"
#include "rendersystem/config/RenderDeviceConfig.h"
#include "newMath/new_float4.h"

namespace Math
{

	enum ColorSpace
	{
		CS_GAMMA,
		CS_LINEAR,
	};

typedef uint32 RGBA;
class Color32;

class ColorF
{
public:
	ColorF ();
	ColorF (scalar red, scalar green, scalar blue, scalar alpha = 1.0F);
	ColorF (const Color32& color);

	scalar operator [] ( const IndexT i ) const;
	scalar& operator [] ( const IndexT i );

public:
	scalar	r, g, b, a;
};

class Color32
{
public:
	Color32 ();						
	Color32 (ubyte red, ubyte green, ubyte blue, ubyte alpha);
	Color32 (RGBA color); 

	ubyte operator [] ( const IndexT i ) const;
	ubyte& operator [] ( const IndexT i );

	void SetFromRGBA(RGBA color);

	void Set(const Color32& color);

	ubyte* Ptr();	

	uint ToUIntRGBA() const; 
	
	uint HexARGB() const; 
public:

#if RENDERDEVICE_D3D9||RENDERDEVICE_D3D12
	ubyte	b, g, r, a;
#elif RENDERDEVICE_OPENGLES20 || RENDERDEVICE_OPENGLES30
	ubyte	r, g, b, a;
#endif
	
};

//------------------------------------------------------------------------
inline
ColorF::ColorF ()
{
}
//------------------------------------------------------------------------
inline 
ColorF::ColorF (scalar red, scalar green, scalar blue, scalar alpha) 
: r(red), g(green), b(blue), a(alpha) 
{
}
//------------------------------------------------------------------------
inline
ColorF::ColorF (const Color32& color)
{
	this->r = n_scalarfromByte(color.r);
	this->g = n_scalarfromByte(color.g);
	this->b = n_scalarfromByte(color.b); 
	this->a = n_scalarfromByte(color.a);
}
//------------------------------------------------------------------------
inline
scalar 
ColorF::operator [] ( const IndexT i ) const
{
	assert( i < 4 );
	return *(&r+i);
}
//------------------------------------------------------------------------
inline 
scalar& 
ColorF::operator [] ( const IndexT i )
{
	assert( i < 4 );
	return *(&r+i);
}
//------------------------------------------------------------------------
inline
Color32::Color32 ()	
{
}
//------------------------------------------------------------------------
inline
Color32::Color32 (ubyte red, ubyte green, ubyte blue, ubyte alpha)
: r(red), g(green), b(blue), a(alpha) 
{ 
}
////------------------------------------------------------------------------
inline
Color32::Color32 (RGBA color)						
{
	SetFromRGBA(color);
}
//------------------------------------------------------------------------
inline
void
Color32::SetFromRGBA (RGBA color)
{
	r = color & 0x000000FF;
	g = (color & 0x0000FF00) >> 8;
	b = (color & 0x00FF0000) >> 16;
	a = (color & 0xFF000000) >> 24;
}
//------------------------------------------------------------------------
inline 
ubyte 
Color32::operator [] ( const IndexT i ) const
{
	assert( i < 4 );
	return *(&r+i);
}
//------------------------------------------------------------------------
inline
ubyte&
Color32::operator [] ( const IndexT i )
{
	assert( i < 4 );
	return *(&r+i);
}
//------------------------------------------------------------------------
inline 
void 
Color32::Set(const Color32& color)
{
	this->r = color.r;
	this->g = color.g;
	this->b = color.b;
	this->a = color.a;
}
//------------------------------------------------------------------------
inline
ubyte* 
Color32::Ptr ()		
{
	return &r;
}
//------------------------------------------------------------------------
inline
uint 
Color32::ToUIntRGBA() const 
{
	//return *((uint*)(&r));
	return N_ARGB(a, b, g, r);
}
//------------------------------------------------------------------------
inline
uint 
Color32::HexARGB() const
{
	uint R = r;
	uint G = g;
	uint B = b;
	uint A = a;

#if RENDERDEVICE_D3D9
	return N_ARGB(A,R,G,B);
#elif RENDERDEVICE_OPENGLES20 || RENDERDEVICE_OPENGLES30
	return N_ARGB(A,B,G,R);
#endif
	
}


inline float GammaToLinearSpace (float value)
{
	if (value <= 0.04045F)
		return value / 12.92F;
	else if (value < 1.0F)
		return n_pow((value + 0.055F)/1.055F, 2.4F);
	else
		return n_pow(value, 2.4F);
}

inline Math::float4 GammaToLinearSpace(const Math::float4& srcColor)
{
	return Math::float4(GammaToLinearSpace(srcColor.x()), 
						GammaToLinearSpace(srcColor.y()),
						GammaToLinearSpace(srcColor.z()),
						GammaToLinearSpace(srcColor.w()));
}

inline float LinearToGammaSpace (float value)
{
	if (value <= 0.0F)
		return 0.0F;
	else if (value <= 0.0031308F)
		return 12.92F * value;
	else if (value <= 1.0F)
		return 1.055F * n_pow(value, 0.41666F) - 0.055F;
	else
		return n_pow(value, 0.41666F);
}

inline Math::float4 LinearToGammaSpace(const Math::float4& srcColor)
{
	return Math::float4(LinearToGammaSpace(srcColor.x()), 
						LinearToGammaSpace(srcColor.y()),
						LinearToGammaSpace(srcColor.z()),
						LinearToGammaSpace(srcColor.w()));
}

}	//	namespace Math




#endif // __color_H__
