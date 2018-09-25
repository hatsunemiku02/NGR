

#ifndef POINT2D_SIZE_H
#define POINT2D_SIZE_H

namespace Math
{
	template<typename T>
	struct TPoint2D
	{
		T left;
		T top;

		TPoint2D() :
		left(0),
			top(0)
		{
		}

		TPoint2D(T const& _left, T const& _top) :
		left(_left),
			top(_top)
		{
		}

		TPoint2D(TPoint2D const& _obj) :
		left(_obj.left),
			top(_obj.top)
		{
		}

		TPoint2D& operator -= (TPoint2D const& _obj)
		{
			left -= _obj.left;
			top -= _obj.top;
			return *this;
		}

		TPoint2D& operator += (TPoint2D const& _obj)
		{
			left += _obj.left;
			top += _obj.top;
			return *this;
		}

		TPoint2D operator - (TPoint2D const& _obj) const
		{
			return TPoint2D(left - _obj.left, top - _obj.top);
		}

		TPoint2D operator + (TPoint2D const& _obj) const
		{
			return TPoint2D(left + _obj.left, top + _obj.top);
		}

		TPoint2D& operator = (TPoint2D const& _obj)
		{
			left = _obj.left;
			top = _obj.top;
			return *this;
		}

		template<typename U>
		TPoint2D& operator = (TPoint2D<U> const& _obj)
		{
			left = _obj.left;
			top = _obj.top;
			return *this;
		}

		bool operator == (TPoint2D const& _obj) const
		{
			return ((left == _obj.left) && (top == _obj.top));
		}

		bool operator != (TPoint2D const& _obj) const
		{
			return ! ((left == _obj.left) && (top == _obj.top));
		}

		void clear()
		{
			left = top = 0;
		}

		void set(T const& _left, T const& _top)
		{
			left = _left;
			top = _top;
		}

		void swap(TPoint2D& _value)
		{
			TPoint2D tmp = _value;
			_value = *this;
			*this = tmp;
		}

		bool empty() const
		{
			return ((left == 0) && (top == 0));
		}
	};

	typedef TPoint2D<float> point2df;
	typedef TPoint2D<int> point2di;
}


#endif