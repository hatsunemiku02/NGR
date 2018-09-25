
#ifndef __ray_H__
#define __ray_H__

#include "math/float3.h"
#include "math/matrix44.h"

namespace Math
{
	class Ray
	{
	public:
		Ray();
		
		Ray(const float3& origin, const float3& normalizeDir);
		
		const float3& Start() const;
		
		void Start(const float3& startPoint);

		const float3& Direction() const;
		void DirectionNormalize( const float3& normalizeDir );

		float3 PointAt(scalar t) const;

		float SqrDistance(const float3 &q) const;

		Ray Tramsform(const matrix44& m) const;

	protected:
		float3 mOrigin;
		float3 mDir;
	};
	//------------------------------------------------------------------------
	inline
	Ray::Ray()
	{

	}
	//------------------------------------------------------------------------
	inline
	Ray::Ray( const float3& origin, const float3& normalizeDir )
	{
		assert( float3::isNormalize(normalizeDir) );
		mOrigin = origin;
		mDir = normalizeDir;
	}
	//------------------------------------------------------------------------
	inline
	const float3& 
	Ray::Start() const
	{
		return mOrigin;
	}
	//------------------------------------------------------------------------
	inline
	void 
	Ray::Start(const float3& startPoint)
	{
		mOrigin = startPoint;
	}
	//------------------------------------------------------------------------
	inline 
	const float3&
	Ray::Direction() const
	{
		return mDir;
	}
	//------------------------------------------------------------------------
	inline 
	void 
	Ray::DirectionNormalize( const float3& normalizeDir )
	{
		assert( float3::isNormalize(normalizeDir) );
		mDir = normalizeDir;
	}
	//------------------------------------------------------------------------
	inline
	float3 
	Ray::PointAt(scalar t) const
	{
		return mOrigin + t * mDir;
	}
	//------------------------------------------------------------------------
	// <Geometric Tools for computer Graphics> P265
	inline
	float 
	Ray::SqrDistance (const float3 & q) const
	{
		float3 vectorSubtract = q - mOrigin; 

		float t1 = mDir.dotProduct(vectorSubtract); 
		float t2 = mDir.dotProduct(mDir); 

		assert( t2 != 0.0f );
		float t = t1 / t2; 

		float3 qPrime = PointAt (t);
		float3 pLength = q - qPrime;
		return pLength.lengthsq();
	}
	//------------------------------------------------------------------------
	inline
	Ray Ray::Tramsform(const matrix44& m) const
	{
		float3 origin = mOrigin.transformPoint(m);
		float3 normalizeDir = mDir.transformVector(m);
		normalizeDir.normalise();
		return Ray(origin,normalizeDir);
	}

}







#endif // __ray_H__
