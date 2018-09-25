
#ifndef __curve_H__
#define __curve_H__
#include <vector>
#include "float2.h"
#include "float3.h"
#include "quaternion.h"


namespace Math
{
	template<typename T>
	class TemplateKeyframe
	{
	public:
		typedef T value_type;
		TemplateKeyframe()
			: mTime(0.0f) {
		}

		TemplateKeyframe(float t, const T& value)
			: mValue(value), mTime(t) {
		}

		inline void SetValue(const T& value) { 
			mValue = value;
		}

		inline const T& GetValue(void) const { 
			return mValue;
		}

		inline void SetTime(float t) {
			mTime = t;
		}

		inline float GetTime(void) const {
			return mTime;
		}

		inline bool operator < ( const TemplateKeyframe<T>& rhs ) {
			return mTime < rhs.mTime;
		}

		inline friend bool operator < ( const TemplateKeyframe<T>& lhs, const TemplateKeyframe<T>& rhs ) {
			return lhs.mTime < rhs.mTime;
		}

		inline bool operator > ( const TemplateKeyframe<T>& rhs ) {
			return mTime > rhs.mTime;
		}

		inline friend bool operator > ( const TemplateKeyframe<T>& lhs, const TemplateKeyframe<T>& rhs ) {
			return lhs.mTime > rhs.mTime;
		}

		inline friend bool operator != ( const TemplateKeyframe<T>& lhs, const TemplateKeyframe<T>& rhs ) {
			return lhs.mTime != rhs.mTime;
		}

	protected:	// for storage, must first T, second time
		T mValue;
		float mTime;
				
	};


	// curve definition
	template<typename T>
	class Curve
	{
	public:
		typedef T KeyFrame;
		typedef typename KeyFrame::value_type KeyFrameData;
		typedef std::vector<KeyFrame> Container;
		Curve() { }
		~Curve() { }

		struct KeyFrameCache 
		{
			int   lowerframe;
			float begintime;
			float endtime;

			KeyFrameCache ()
			{
				begintime  = Math::N_INFINITY;
				endtime    = -Math::N_INFINITY;
				lowerframe = 0;
			}
		};

		// get keyFrame Count
		SizeT GetKeyFrameCount(void) const {
			return mKeyFrames.size();
		}

		const KeyFrame& GetKeyFrame(IndexT index) const {
			assert( index >= 0 && index < mKeyFrames.size() );
			return mKeyFrames[index];
		}

		KeyFrame& GetKeyFrame(IndexT index) {
			assert( index >= 0 && index < mKeyFrames.size() );
			return mKeyFrames[index];
		}

		void SetKeyFrameData( IndexT index, const KeyFrameData& data ) {
			if ( index >= 0 && index < mKeyFrames.size() ) {
				mKeyFrames[index].SetValue( data );
			}
		}
		IndexT AddKeyFrame(const KeyFrame& key ) {
			assert(0, "InsertSorted not implemented");
			return 0;
			//return mKeyFrames.InsertSorted(key);
		}

		void RemoveKeyFrame(IndexT index ) {
			if ( index >= 0 && index < mKeyFrames.size() ) {
				auto deleteitr = mKeyFrames.begin() + index;
				mKeyFrames.erase(deleteitr);
			}	
		}

		const Container& GetContainer(void) const {
			return mKeyFrames;
		}

		Container& GetContainer(void){
			return mKeyFrames;
		}

		void SetContainer( const Container& container, bool bNeedSort = true ) {
			mKeyFrames = container;
			if ( bNeedSort ) {
				mKeyFrames.Sort();
			}
		}

		float GetBeginTime(void) const {
			return mKeyFrames.IsEmpty() ? Math::N_INFINITY : mKeyFrames.Front().GetTime(); 
		}

		float GetEndTime(void) const {
			return mKeyFrames.IsEmpty() ? -Math::N_INFINITY : mKeyFrames.Back().GetTime(); 
		}

		//!!!Slow!!!
		bool Validate(void) const{
			return mKeyFrames.IsEmpty() ? false : mKeyFrames.IsSorted();
		}

		void ClearFrames()
		{
			mKeyFrames.clear();
		}

		void ClearFrameCache()
		{
			mKeyFrameCache.lowerframe = 0;
			mKeyFrameCache.begintime  = Math::N_INFINITY;
			mKeyFrameCache.endtime    = -Math::N_INFINITY;
		}

		bool IsEmpty() const{
			return mKeyFrames.empty() ? true : false;
		}

		int CalculateKeyFrame(float time, T& first, T& second, float& percentage);
		T EvaluateFloat(float time);
		T EvaluateFloat3(float time);
		T EvaluateQuaternion(float time);

		T EvaluateFloatOnPolyBezier(float time);
		

	protected:
		Container mKeyFrames;
		KeyFrameCache     mKeyFrameCache;
	};

	typedef TemplateKeyframe<float>       FloatKeyFrame;
	typedef TemplateKeyframe<float2>      Float2KeyFrame;
	typedef TemplateKeyframe<float3>      Float3fKeyFrame;
	typedef TemplateKeyframe<quaternion>  QuaternionKeyFrame;

	typedef Curve<FloatKeyFrame>         FloatCurve;
	typedef Curve<Float2KeyFrame>        Float2Curve;
	typedef Curve<Float3fKeyFrame>       Float3fCurve;
	typedef Curve<QuaternionKeyFrame>    QuaternionCurve;
	

	//--------------------------------------------------------------------------------
	template<typename T>
	int Curve<T>::CalculateKeyFrame(float time, T& first, T& second, float& percentage)
	{
		int keyCount   = mKeyFrames.size();
		int lowerFrame = 0;
		//Look mKeyFrameCache for optimization
		if (time >= mKeyFrameCache.begintime && time < mKeyFrameCache.endtime)
		{
			lowerFrame = mKeyFrameCache.lowerframe;

			first  = mKeyFrames[lowerFrame];
			second = mKeyFrames[lowerFrame+1];

			percentage = (time - first.GetTime()) /
				( second.GetTime() - first.GetTime());
		}
		else
		{
		
				int nCount = keyCount;
				int temp;

				//Binary search key match the interval
				//Note: Assume key frames are sorted in increase time order
				//nCount is upper bound
				//lowerFrame is lower bound
				while (nCount > 0)
				{
					temp = nCount >> 1;

					//Search towards right
					if (time > mKeyFrames[lowerFrame + temp].GetTime())
					{
						lowerFrame += temp + 1;
						nCount     -= temp + 1;
						
					} 
					//Search towards left
					else
					{
						nCount = temp;
					}
				}

				if(lowerFrame != 0)
				{
					lowerFrame -= 1;
				}
			

			if (lowerFrame == mKeyFrames.size() - 1)
			{
				lowerFrame -= 1;
			}

			if (time>mKeyFrames[lowerFrame+1].GetTime() || time < mKeyFrames[lowerFrame].GetTime() && lowerFrame+1 != keyCount)
			{
				assert(0,"the time is not between two key frames");
			}

			first  = mKeyFrames[lowerFrame];
			second = mKeyFrames[lowerFrame+1];

			percentage = (time - first.GetTime()) /
				( second.GetTime() -  first.GetTime());

			mKeyFrameCache.lowerframe = lowerFrame;
			mKeyFrameCache.begintime  = first.GetTime();
			mKeyFrameCache.endtime    = second.GetTime();
		}
		return lowerFrame;
	}

	template<typename T>
	T Curve<T>::EvaluateFloat(float time)
	{
		T resultFrame;
		int keyCount   = mKeyFrames.size();

		if (time > mKeyFrames[keyCount-1].GetTime() || 
			n_fequal(time, mKeyFrames[keyCount-1].GetTime(), N_TINY) ||
			time < mKeyFrames[0].GetTime() )
		{
			float result = mKeyFrames[keyCount-1].GetValue();
			resultFrame.SetValue(result);
			return resultFrame;
		}

		T first;
		T second;
		float percentage;
		CalculateKeyFrame(time, first, second, percentage );

		float reslut =  Math::n_lerp(first.GetValue(), second.GetValue(), percentage);
		resultFrame.SetValue(reslut);

		return resultFrame;
	}

	template<typename T>
	T Curve<T>::EvaluateFloat3(float time)
	{
		T resultFrame;

		T first;
		T second;
		float percentage;
		CalculateKeyFrame(time, first, second,  percentage );

		float3 reslut;
		float lerpX, lerpY, lerpZ;
		lerpX = Math::n_lerp(first.GetValue().x(), second.GetValue().x(), percentage);
		lerpY = Math::n_lerp(first.GetValue().y(), second.GetValue().y(), percentage);
		lerpZ = Math::n_lerp(first.GetValue().z(), second.GetValue().z(), percentage);
		reslut.set(lerpX, lerpY, lerpZ);
		resultFrame.SetValue(reslut);

		return resultFrame;
	}

	template<typename T>
	T Curve<T>::EvaluateQuaternion(float time)
	{
		T resultFrame;

		T first;
		T second;
		float percentage;
		CalculateKeyFrame(time, first, second,  percentage );

		quaternion reslut = quaternion::slerp(first.GetValue(), second.GetValue(), percentage);
		resultFrame.SetValue(reslut);

		return resultFrame; 
	}



}







#endif // __curve_H__
