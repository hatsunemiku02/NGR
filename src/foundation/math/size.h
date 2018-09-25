

#ifndef MATH_SIZE_H
#define MATH_SIZE_H

namespace Math
{
	template<typename T>
	class tsize
	{
	public:
		T width;
		T height;

		tsize() :
		width(0),
			height(0)
		{
		}

		tsize(T const& _width, T const& _height) :
		width(_width),
			height(_height)
		{
		}

		tsize(tsize const& _obj) :
		width(_obj.width),
			height(_obj.height)
		{
		}

		tsize& operator -= (tsize const& _obj)
		{
			width -= _obj.width;
			height -= _obj.height;
			return *this;
		}

		tsize& operator += (tsize const& _obj)
		{
			width += _obj.width;
			height += _obj.height;
			return *this;
		}

		tsize operator - (tsize const& _obj) const
		{
			return tsize(width - _obj.width, height - _obj.height);
		}

		tsize operator + (tsize const& _obj) const
		{
			return tsize(width + _obj.width, height + _obj.height);
		}

		tsize operator * (tsize const& _obj) const
		{
			return tsize(width * _obj.width, height * _obj.height);
		}

		tsize operator * (T const& _obj) const
		{
			return tsize(width * _obj, height * _obj);
		}

		tsize <float>operator / (const T& value) const
		{
			return tsize(width / value, height / value);
		}

		tsize& operator = (tsize const& _obj)
		{
			width = _obj.width;
			height = _obj.height;
			return *this;
		}

		template<typename U>
		tsize& operator = (tsize<U> const& _obj)
		{
			width = _obj.width;
			height = _obj.height;
			return *this;
		}

		bool operator == (tsize const& _obj) const
		{
			return ((width == _obj.width) && (height == _obj.height));
		}

		bool operator != (tsize const& _obj) const
		{
			return !((width == _obj.width) && (height == _obj.height));
		}

		void clear()
		{
			width = height = 0;
		}

		void set(T const& _width, T const& _height)
		{
			width = _width;
			height = _height;
		}

		void swap(tsize& _value)
		{
			tsize tmp = _value;
			_value = *this;
			*this = tmp;
		}

		bool empty() const
		{
			return ((width == 0) && (height == 0));
		}
	};

	typedef tsize<float> sizef;
	typedef tsize<int> sizei;
}


#endif //MATH_SIZE_H