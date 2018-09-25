
#include "stdneb.h"
#include "math/float4.h"
#include "math/matrix44.h"


namespace Math
{
	//------------------------------------------------------------------------------
	/**
	*/
	float4
		float4::transform(const matrix44 &m, __Float4Arg v)
	{
		return m.mx * v.vec;
	}

	__forceinline void
		float4::operator=(const float3 &rhs)
	{
		this->vec.x = rhs.X;
		this->vec.y = rhs.Y;
		this->vec.z = rhs.Z;
		this->vec.w = 1;
	}

} // namespace Math
