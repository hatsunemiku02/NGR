

#ifndef __MinMax_Curve_H__
#define __MinMax_Curve_H__
#include <vector>
#include <assert.h>
#include "PolyCurve.h"

namespace Math
{
	class MinMaxCurve
	{
	public:
		enum	CurveState { Scalar = 0,RandomScalar = 1,Curve = 2, TwoCurves = 3,};
		MinMaxCurve(CurveState curveState = Scalar);
		~MinMaxCurve(){}

		float Calculate(float time,float randomValue = 1.0f);
		void SetTwoCurve(const FloatPolyCurve& curve1, const FloatPolyCurve& curve2);
		void GetTwoCurve(FloatPolyCurve& curve1, FloatPolyCurve& curve2)const;
		void SetOneCurve(const FloatPolyCurve& curve1);
		void GetOneCurve(FloatPolyCurve& curve1)const;
		void SetMaxY(float Value);
		void SetScalar(float Value);
		float GetMaxY(void) const;
		float GetScalar(void) const;
		void SetRandomScalar(float2 Value);
		float2 GetRandomScalar() const;
		void SetCurveState(CurveState state);
		CurveState GetCurveState(void) const;
		void CopyFrom(const MinMaxCurve& source);
		void GetArrayFromCurve(std::vector<float2>& _list, std::vector<bool>& _curveTypes) const;
		void SetCurveFromArray(const std::vector<float2>& _list, const std::vector<bool>& _curveTypes);
		void GetArrayFromCurve(std::vector<float2>& _list1, std::vector<bool>& _curveTypes1,
			std::vector<float2>& _list2, std::vector<bool>& _curveTypes2) const;
		void SetCurveFromArray(const std::vector<float2>& _list1, const std::vector<bool>& _curveTypes1,
			const std::vector<float2>& _list2, const std::vector<bool>& _curveTypes2);
		void Clear();
	protected:
		FloatPolyCurve	mMinCurve;
		FloatPolyCurve	mMaxCurve;

		float					mMinScalar;
		float                   mMaxScalar;
		float					mMaxY;
		float					mMinY;
		CurveState        mCurveState;

		void _getCurve(const FloatPolyCurve& curve, std::vector<float2>& _list, std::vector<bool>& _curveTypes)const;
		void _setCurve(FloatPolyCurve& curve, const std::vector<float2>& _list, const std::vector<bool>& _curveTypes);
	};
	inline void MinMaxCurve::GetTwoCurve(FloatPolyCurve& curve1, FloatPolyCurve& curve2) const
	{
		curve1 = mMinCurve;
		curve2 = mMaxCurve;
	}
	inline void MinMaxCurve::SetTwoCurve(const FloatPolyCurve& curve1, const FloatPolyCurve& curve2)
	{
		mMinCurve = curve1;
		mMaxCurve = curve2;
	}
	inline void MinMaxCurve::SetOneCurve(const FloatPolyCurve& curve1)
	{
		mMinCurve = curve1;
	}
	inline void MinMaxCurve::GetOneCurve(FloatPolyCurve& curve1) const
	{
		curve1 = mMinCurve;
	}
	inline void MinMaxCurve::SetScalar(float Value)
	{
		mMinScalar = Value;
	}
	inline float MinMaxCurve::GetMaxY(void) const
	{
		return mMaxY;
	}
	inline float MinMaxCurve::GetScalar(void) const
	{
		return mMinScalar;
	}
	inline void MinMaxCurve::SetRandomScalar(float2 Value)
	{
		mMinScalar = Value.x();
		mMaxScalar = Value.y();
	}
	inline float2 MinMaxCurve::GetRandomScalar() const
	{
		return float2(mMinScalar,mMaxScalar);
	}
	inline MinMaxCurve::CurveState MinMaxCurve::GetCurveState(void) const
	{
		return mCurveState;
	}
}








#endif //__MinMax_curve_H__