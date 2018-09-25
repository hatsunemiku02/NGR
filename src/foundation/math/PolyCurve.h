

#ifndef __poly_curve_H__
#define __poly_curve_H__
#include <vector>
#include "curve.h"

namespace Math
{
	template<typename T>
	class PolyCurve:public Curve<T>
	{
	public:
		struct FrameLever
		{
			FrameLever(){}
			FrameLever(const T& leftkey,  const T& rightkey)
			{
				_left = leftkey;
				_right = rightkey;
			}
			T _left;
			T _right;
		};
		typedef std::vector<FrameLever> LeverContainer;


		struct CurveType
		{
			CurveType()
			{
				_leftLinner = false;
				_rightLinner = false;
			}
			CurveType(bool leftLinner,bool rightLinner)
			{
				_leftLinner = leftLinner;
				_rightLinner = rightLinner;
			}
			bool _leftLinner;
			bool _rightLinner;
		};
		typedef std::vector<CurveType> TypeContainer;

	public:
		PolyCurve(){}
		~PolyCurve(){}

		T EvaluatePolyCurveFloat(float t);

		//void AddKeyFrameAndLever(const Util::Array<T>& contaner);

		IndexT AddFrameLever(const T& leftkey,  const T& rightkey) {
			mFrameLevers.push_back(FrameLever(leftkey, rightkey));
			return mFrameLevers.size() -1;
		}

		void AddLeftLever(IndexT nIdx, const T& key) {
			if ( nIdx >= mFrameLevers.size())
			{
				mFrameLevers.push_back(FrameLever());
				mFrameLevers.back()._left = key;
			}
			else
			{
				mFrameLevers[nIdx]._left = key;
			}
		}
		const T& GetLeftLever(IndexT nIdx){
			assert( nIdx >= 0 && nIdx < mFrameLevers.size() );
			return mFrameLevers[nIdx]._left;
		}

		void AddRightLever(IndexT nIdx, const T& key) {
			if ( nIdx >= mFrameLevers.size())
			{
				mFrameLevers.push_back(FrameLever());
				mFrameLevers.back()._right = key;
			}
			else
			{
				mFrameLevers[nIdx]._right = key;
			}
		}
		const T& GetRightLever(IndexT nIdx){
			assert( nIdx >= 0 && nIdx < mFrameLevers.size() );
			return mFrameLevers[nIdx]._right;
		}
		const FrameLever& GetFrameLever(IndexT index) const {
			assert( index >= 0 && index < mFrameLevers.size() );
			return mFrameLevers[index];
		}

		FrameLever& GetFrameLever(IndexT index) {
			assert( index >= 0 && index < mFrameLevers.size() );
			return mFrameLevers[index];
		}

		SizeT GetFrameLeversCount() const
		{
			return  mFrameLevers.size();
		}

		void ClearFrameLevers()
		{
			mFrameLevers.clear();
		}

		void RemoveFrameLevers(IndexT index)
		{
			if ( index >= 0 && index < mFrameLevers.size() ) {
				auto deleteitr = mFrameLevers.begin() + index;
				mFrameLevers.erase(deleteitr);
			}	
		}

		IndexT AddCurveType(bool leftkey,bool rightkey) {
			mCurveTypes.push_back(CurveType(leftkey, rightkey));
			return mCurveTypes.size() -1;
		}

		void AddLeftCurveType(IndexT nIdx, bool key) {
			if ( nIdx >= mCurveTypes.size())
			{
				mCurveTypes.push_back(CurveType());
				mCurveTypes.back()._leftLinner = key;
			}
			else
			{
				mCurveTypes[nIdx]._leftLinner = key;
			}
		}

		bool GetLeftCurveType(IndexT nIdx) const
		{
			assert( nIdx >= 0 && nIdx < mCurveTypes.size() );
			return mCurveTypes[nIdx]._leftLinner;
		}

		void AddRightCurveType(IndexT nIdx,bool key) {
			if ( nIdx >= mCurveTypes.size())
			{
				mCurveTypes.push_back(CurveType());
				mCurveTypes.back()._rightLinner = key;
			}
			else
			{
				mCurveTypes[nIdx]._rightLinner = key;
			}
		}
		bool GetRightCurveType(IndexT nIdx)const
		{
			assert( nIdx >= 0 && nIdx < mCurveTypes.size() );
			return mCurveTypes[nIdx]._rightLinner;
		}

		SizeT GetCurveTypesCount() const
		{
			return  mCurveTypes.size();
		}

		void ClearCurveTypes()
		{
			mCurveTypes.clear();
		}

		void AddPolyKeyFrame(const T& key ,bool leftLinner = false ,bool rightLinner = false) {
			//IndexT InX = this->AddKeyFrame(key);
			//mCurveTypes.Insert(InX,CurveType(leftLinner,rightLinner));
			assert(0, "not implemented yet");
		}
	protected:
		LeverContainer mFrameLevers;
		TypeContainer mCurveTypes;
	};


	typedef PolyCurve<Math::FloatKeyFrame>         FloatPolyCurve;

	//--------------------------------------------------------------------------------
	template<typename T>
	T PolyCurve<T>::EvaluatePolyCurveFloat(float t)
	{
		int keyCount = this->mKeyFrames.size();
		T resultFrame;

		if(t < this->mKeyFrames[0].GetTime()  || keyCount < 2)
		{
			 float result = this->mKeyFrames[0].GetValue();
			 resultFrame.SetValue(result);
		}
		else if (t > this->mKeyFrames[keyCount-1].GetTime() )
		{
			float result = this->mKeyFrames[keyCount-1].GetValue();
			resultFrame.SetValue(result );
		}		
		else
		{

			//--------------------------------------------------------------------------------
			T first;
			T second;
			float percentage;
			int nIdx = this->CalculateKeyFrame(t, first, second, percentage);
			if ( nIdx >= this->mFrameLevers.size() || (mCurveTypes[nIdx]._rightLinner && mCurveTypes[nIdx+1]._leftLinner))
			{
				// if isnt exit,linner
				return Curve<T>::EvaluateFloat(t);
			}

			T& firstRight = this->mFrameLevers[nIdx]._left;
			T& secondLeft = this->mFrameLevers[nIdx]._right;

			if ( secondLeft.GetTime() < TINY )
				return first;
			if ( firstRight.GetTime() < TINY )
				return second;


			//--------------------------------------------------------------------------------
			float tSquared, tCubed; 
			float result; 

			float timeLength = second.GetTime() - first.GetTime();
			float curPercentage = ( t -  first.GetTime() ) / timeLength;

			tSquared = curPercentage * curPercentage; 
			tCubed = tSquared * curPercentage; 

			float t_ = 1.0f - curPercentage;
			float t_Squared = t_ * t_;
			float t_Cubed = t_ * t_ * t_;

			result = t_Cubed * first.GetValue() + 3 * curPercentage * t_Squared * firstRight.GetValue()
				     + 3 * tSquared * t_ * secondLeft.GetValue() + tCubed * second.GetValue();
			/***/
			resultFrame.SetValue(result);
		}

		return resultFrame;
	}
	//--------------------------------------------------------------------------------
	//template<typename T>
	//void PolyCurve<T>::AddKeyFrameAndLever(const Util::Array<T>& contaner)
	//{

	//}
}




#endif // __poly_curve_H__
