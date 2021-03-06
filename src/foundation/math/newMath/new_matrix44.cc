
#include "stdneb.h"
#include "math/matrix44.h"
#include "math/plane.h"
#include "math/quaternion.h"

namespace Math
{

//------------------------------------------------------------------------------
/**
*/
void
matrix44::decompose(float4& outScale, quaternion& outRotation, float4& outTranslation) const
{
	/*
    BOOL result = XMMatrixDecompose(&outScale.vec, 
                        &outRotation.vec, 
                        &outTranslation.vec, 
                        this->mx);
    n_assert(result);
	outScale.set_w(0.0f);
	outTranslation.set_w(0.0f);
	*/
	Ogre::Vector3 outS;
	Ogre::Quaternion outR;
	Ogre::Vector3 outT;
	this->mx.decomposition(outT,outS,outR);
	outScale.set(outS.x,outS.y,outS.z,0.0f);
	outTranslation.set(outT.x,outT.y,outT.z,0.f);
	outRotation.set(outR.x,outR.y,outR.z,outR.w);
}

void matrix44::MultiplyPoint3(const float3& inV, float3& output) const
{
	// = m_Data[0] * v.x + m_Data[4] * v.y + m_Data[ 8] * v.z + m_Data[12];

	output.X = mx.m[0][0] * inV.X + mx.m[0][1] * inV.Y + mx.m[0][2] * inV.Z + mx.m[0][3] ;
	output.Y = mx.m[1][0] * inV.X + mx.m[1][1] * inV.Y + mx.m[1][2] * inV.Z + mx.m[1][3] ; 
	output.Z = mx.m[2][0] * inV.X + mx.m[2][1] * inV.Y + mx.m[2][2] * inV.Z + mx.m[2][3] ; 
}

//------------------------------------------------------------------------------
/**
*/
matrix44
matrix44::transformation(float4 const &scaling, const quaternion& rotation, float4 const &translation)
{
	Ogre::Matrix4 mat;
	mat.makeTransform(	Ogre::Vector3(translation.x(),translation.y(),translation.z()),
						Ogre::Vector3(scaling.x(),scaling.y(),scaling.z()),
						Ogre::Quaternion(rotation.vec) );
	return matrix44(mat);
	/*
    return XMMatrixTransformation(scalingCenter.vec,
                                  scalingRotation.vec,
                                  scaling.vec,
                                  rotationCenter.vec,
                                  rotation.vec,
                                  translation.vec);
	*/
}

//------------------------------------------------------------------------------
/**
*/
bool 
matrix44::ispointinside(const matrix44& m, const float4& p)
{
    float4 p1 = matrix44::transform(m, p);
    // vectorized compare operation
    return !(float4::less4_any(float4(p1.x(), p1.w(), p1.y(), p1.w()), 
             float4(-p1.w(), p1.x(), -p1.w(), p1.y()))
            ||
            float4::less4_any(float4(p1.z(), p1.w(), 0, 0), 
            float4(-p1.w(), p1.z(), 0, 0)));
}

//------------------------------------------------------------------------------
/**
*/
plane
matrix44::transform(const matrix44& m, const plane &p)
{
	return m.mx * p.vec;
}

matrix44
matrix44::rotationquaternion(const quaternion& q)
{
	return Ogre::Matrix4(q.vec);
}

quaternion 
matrix44::rotationmatrix(const matrix44& m)
{
	return m.mx.extractQuaternion();
}

} // namespace Math
