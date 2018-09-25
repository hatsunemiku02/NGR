
#include "stdneb.h"
#include "math/float4.h"
#include "math/matrix44.h"


namespace Math
{

	//--------------------------------------------------
	quaternion::quaternion(const float3& lhs, const float3& rhs)
	{
		float3 vecFrom = float3::normalize(lhs);
		float3 vecTo = float3::normalize(rhs);

		Math::float3 rotateAxis = vecFrom.crossProduct(vecTo);
		rotateAxis.normalise();

		float angle = Math::n_acos(vecFrom.dotProduct(vecTo));
					
		vec = Ogre::Quaternion(Ogre::Radian(angle), Ogre::Vector3(rotateAxis.x(), rotateAxis.y(), rotateAxis.z()));
	}

//------------------------------------------------------------------------

vector quaternion::operator* (const vector& v) const
{
	// copy from ogre

	// nVidia SDK implementation
	vector uv, uuv;
	vector qvec (x(), y(), z() );
	uv = qvec.crossProduct(v);
	uuv = qvec.crossProduct(uv);
	uv *= (2.0f * w());
	uuv *= 2.0f;

	return v + uv + uuv;
}
}

