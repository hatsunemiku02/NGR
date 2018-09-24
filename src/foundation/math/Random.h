#ifndef RANDOM_H
#define RANDOM_H

#include "rand.h"
#include "float2.h"
#include "float3.h"
#include "scalar.h"
#include "newMath/new_quaternion.h"
//#include "Runtime/Math/FloatConversion.h"
namespace Math
{
	inline float RangedRandom (Rand& r, float min, float max)
	{
		float t = r.GetFloat ();
		t = min * t + (1.0F - t) * max;
		return t;
	}

	inline float Random01 (Rand& r)
	{
		return r.GetFloat ();
	}

	inline int RangedRandom (Rand& r, int min, int max)
	{
		int dif;
		if (min < max)
		{
			dif = max - min;
			int t = r.Get () % dif;
			t += min;
			return t;
		}
		else if (min > max)
		{
			dif = min - max;
			int t = r.Get () % dif;
			t = min - t;
			return t;
		}
		else
		{
			return min;
		}
	}

	inline float3 RandomUnitVector (Rand& rand)
	{
		float z = RangedRandom (rand, -1.0f, 1.0f);
		float a = RangedRandom (rand, 0.0f, 2.0f * N_PI);

		float r = sqrt (1.0f - z*z);

		float x = r * cos (a);
		float y = r * sin (a);

		return float3 (x, y, z);
	}

	inline float2 RandomUnitVector2 (Rand& rand)
	{
		float a = RangedRandom (rand, 0.0f, 2.0f * N_PI);

		float x = cos (a);
		float y = sin (a);

		return float2 (x, y);
	}


	inline quaternion RandomQuaternion (Rand& rand)
	{
		quaternion q;
		q.x() = RangedRandom (rand, -1.0f, 1.0f);
		q.y() = RangedRandom (rand, -1.0f, 1.0f);
		q.z() = RangedRandom (rand, -1.0f, 1.0f);
		q.w() = RangedRandom (rand, -1.0f, 1.0f);
		q =  quaternion::normalize(q) ;//  NormalizeSafe (q);
		if (q.dot (quaternion::identity ()) < 0.0f)
			return -q;
		else
			return q;
	}

	inline quaternion RandomQuaternionUniformDistribution (Rand& rand)
	{
		const float two_pi =  2.0f * N_PI;

		// Employs Hopf fibration to uniformly distribute quaternions
		float u1 = RangedRandom( rand, 0.0f, 1.0f );
		float theta = RangedRandom( rand, 0.0f, two_pi );
		float rho = RangedRandom( rand, 0.0f, two_pi );

		float i = sqrt( 1.0f - u1 );
		float j = sqrt( u1 );

		// We do not need to normalize the generated quaternion, because the probability density corresponds to the Haar measure.
		// This means that a random rotation is obtained by picking a point at random on S^3, and forming the unit quaternion.
		quaternion q( i * sin(theta), i * cos(theta), j * sin(rho), j * cos(rho) );

		if (q.dot (quaternion::identity ()) < 0.0f)
			return -q;
		else
			return q;
	}


	inline float3 RandomPointInsideCube (Rand& r, const float3& extents)
	{
		return float3 (	RangedRandom (r, -extents.x(), extents.x()),
			RangedRandom (r, -extents.y(), extents.y()),
			RangedRandom (r, -extents.z(), extents.z()));
	}

	inline float3 RandomPointBetweenCubes (Rand& r, const float3& min, const float3& max)
	{
		float3 v;
		int i;
		for (i=0;i<3;i++)
		{
			float x = r.GetFloat () * 2.0F - 1.0F;
			if (x > 0.0f)
				v[i] = min[i] + x * (max[i] - min[i]);
			else
				v[i] = -min[i] + x * (max[i] - min[i]);
		}
		return v;
	}

	inline float3 RandomPointInsideUnitSphere (Rand& r)
	{
		float3 v = RandomUnitVector (r);
		v *= pow (Random01 (r), 1.0F / 3.0F);
		return v;
	}

	inline float3 RandomPointInsideEllipsoid (Rand& r, const float3& extents)
	{
		return RandomPointInsideUnitSphere (r)*extents;
	}

	inline float3 RandomPointBetweenSphere (Rand& r, float minRadius, float maxRadius)
	{
		float3 v = RandomUnitVector (r);
		// As the volume of the sphere increases (x^3) over an interval we have to increase range as well with x^(1/3)
		float range = pow (RangedRandom (r, 0.0F, 1.0F), 1.0F / 3.0F);
		return v * (minRadius + (maxRadius - minRadius) * range);
	}

	inline float2 RandomPointInsideUnitCircle (Rand& r)
	{
		float2 v =	RandomUnitVector2 (r);
		// As the volume of the sphere increases (x^3) over an interval we have to increase range as well with x^(1/3)
		v *= pow (RangedRandom (r, 0.0F, 1.0F), 1.0F / 2.0F);
		return v;
	}

	inline float3 RandomPointBetweenEllipsoid (Rand& r, const float3& maxExtents, float minRange)
	{
		float3 v = RandomUnitVector (r)* maxExtents;
		// As the volume of the sphere increases (x^3) over an interval we have to increase range as well with x^(1/3)
		float range = pow (RangedRandom (r, minRange, 1.0F), 1.0F / 3.0F);
		return v * range;
	}

	/// Builds a random Barycentric coordinate which can be used to generate random points on a triangle:
	/// Vector3f point = v0 * barycentric.x + v1 * barycentric.y + v2 * barycentric.z;
	inline float3 RandomBarycentricCoord (Rand& rand)
	{
		// Was told that this leads to bad distribution because of the 1.0F - s
		//	float s = gRand.GetFloat ();
		//	float t = RangedRandom (gRand, 0.0F, 1.0F - s);
		//	float r = (1.0F - s - t);
		//	Vector3f positionOnMesh = r * vertices[face.v1] + s * vertices[face.v2] + t * vertices[face.v3];
		//	return positionOnMesh;
		float u = rand.GetFloat ();
		float v = rand.GetFloat ();
		if (u + v > 1.0F)
		{
			u = 1.0F - u;
			v = 1.0F - v;
		}
		float w = 1.0F - u - v;
		return float3 (u, v, w);
	}
}



#endif
