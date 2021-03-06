

#include "stdneb.h"
#include "float3.h"
#include "matrix44.h"
namespace Math
{
	const float float3::epsilon = 0.00001F;

	//------------------------------------------------------------------------
	float4 float3::calculateFaceNormalWithoutNormalize( const float3& v1, const float3& v2, const float3& v3 )
	{
		float3 normal = calculateBasicFaceNormalWithoutNormalize( v1, v2, v3);
		return float4(normal.x(), normal.y(), normal.z(),-(normal.dotProduct(v1)));
	}
	//------------------------------------------------------------------------
	float3 float3::calculateBasicFaceNormalWithoutNormalize(const float3& v1, const float3& v2, const float3& v3)
	{
		float3 normal = (v2-v1).crossProduct(v3 - v1);
		return normal;
	}
	//-------------------------------------------------------------------------
	void float3::hermite(Math::float3 *pOut,const Math::float3 *pV1,const Math::float3 *pT1,const Math::float3 *pV2,const Math::float3 *pT2,Math::scalar s)
	{
		Math::scalar fLenSD =  (*pV2 - *pV1).length();
		Math::float3 T1;
		Math::float3 T2;
		T1 = Math::float3::normalize(*pT1);
		T2 = Math::float3::normalize(*pT2);

		T1 *= fLenSD;
		T2 *= fLenSD;

		*pOut = ((2*s*s*s - 3*s*s + 1) * (*pV1) + (-2*s*s*s + 3*s*s)*(*pV2) + (s*s*s - 2*s*s +s)*(T1) + (s*s*s - s*s)*(T2));
	}


	float3 float3::transformPoint(const Math::matrix44& m) const
	{
		float3 out;
		m.MultiplyPoint3(*this,out);	
		return out;

//  		float4 p = float4::transform(m,float4(X,Y,Z,1));
//  		return float3(p.x(), p.y(),p.z());
	}
}