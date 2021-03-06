

#pragma once
#include <random>
#include "core/types.h"

namespace Math
{
typedef float scalar;

const scalar LN_2 = 0.693147180559945f;

#ifndef PI
#define PI (3.1415926535897932384626433832795028841971693993751)
#endif
// the half circle
#ifndef N_PI
#define N_PI (Math::scalar(PI))
#endif

//------------------------------------------------------------------------------
/**
    Return a pseudo random number between 0 and 1.
*/
__forceinline scalar 
n_rand()
{
    return scalar(rand()) / scalar(RAND_MAX);
}

//------------------------------------------------------------------------------
/**
    Return a pseudo random number between min and max.
*/
__forceinline scalar 
n_rand(scalar min, scalar max)
{
	scalar unit = scalar(rand()) / RAND_MAX;
	scalar diff = max - min;
	return min + unit * diff;
}

//------------------------------------------------------------------------------
/**
*/
__forceinline scalar
n_sin(scalar x)
{
	return sinf(x);
}

//------------------------------------------------------------------------------
/**
*/
__forceinline scalar
n_cos(scalar x)
{
	return cosf(x);
}

//------------------------------------------------------------------------------
/**
*/
__forceinline scalar
n_tan(scalar x)
{
    return tanf(x);
}

//------------------------------------------------------------------------------
/**
*/
__forceinline scalar
n_asin(scalar x)
{
	return asinf(x);
}

//------------------------------------------------------------------------------
/**
*/
__forceinline scalar
n_acos(scalar x)
{
	return acosf(x);
}

//------------------------------------------------------------------------------
/**
*/
__forceinline scalar
n_atan(scalar x)
{
    return atanf(x);
}
//------------------------------------------------------------------------------
/**
*/
__forceinline scalar
	n_atan2(scalar x, scalar y)
{
	return atan2f(x, y);
}

//------------------------------------------------------------------------------
/**
*/
__forceinline scalar 
n_sqrt(scalar x)
{
    return sqrtf(x);
}

//------------------------------------------------------------------------------
/**
    Chop float to int.
*/
__forceinline int 
n_fchop(scalar f)
{
    /// @todo type cast to int is slow!
    return int(f);
}

//------------------------------------------------------------------------------
/**
    log2() function.
*/
__forceinline scalar 
n_log2(scalar f) 
{ 
    return logf(f) / LN_2; 
}

//------------------------------------------------------------------------------
/**
*/
__forceinline scalar
n_exp(scalar x)
{
    return expf(x);
}

//------------------------------------------------------------------------------
/**
    Round float to integer.
*/
__forceinline int 
n_frnd(scalar f)
{
    return n_fchop(floorf(f + 0.5f));
}

//------------------------------------------------------------------------------
/**
*/
__forceinline scalar
n_fmod(scalar x, scalar y)
{
    return fmodf(x, y);
}

//------------------------------------------------------------------------------
/**
*/
__forceinline scalar
n_pow(scalar x, scalar y)
{
    return powf(x, y);
}

//------------------------------------------------------------------------------
/**
	get logarithm of x
*/
__forceinline scalar
n_log(scalar x)
{
    return logf(x);
}

//------------------------------------------------------------------------------
/**
	get sign of x
*/
__forceinline scalar
n_sign (scalar x)
{
	if ( x > 0.0 )
		return 1.0;

	if ( x < 0.0 )
		return -1.0;
	return 0.0;
}

__forceinline int
	n_nexPowerOfTwo (int x)
{
	int base = 1;

	while( base < x)
	{
		base = base << 1;
	}
	return base;
}

__forceinline bool
	n_IsPowerOfTwo(int x)
{
	return !(x&(x-1));
}

__forceinline int
	n_logTwoInt (int x)
{
	int n = 0;

	while (x > 1)
	{
		x = x >> 1;
		n++;
	}

	return n;
}

} // namespace Math
//------------------------------------------------------------------------------



    