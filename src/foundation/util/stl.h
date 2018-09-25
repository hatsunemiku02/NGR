

#ifndef __stl_H__
#define __stl_H__
#include <list>
#include <set>
#include <map>
#include <vector>
#include <deque>
#include <utility>
#include <cctype>
#include <limits>
#include <algorithm>

namespace Util
{

	template <typename T> 
	struct STL_deque 
	{ 
		typedef typename std::deque<T> type;    
	}; 

	template <typename T> 
	struct STL_vector 
	{ 
		typedef typename std::vector<T> type;    
	}; 

	template <typename T> 
	struct STL_list 
	{ 
		typedef typename std::list<T> type;    
	}; 

	template <typename T, typename P = std::less<T> > 
	struct STL_set 
	{ 
		typedef typename std::set<T, P> type;   
		typedef std::pair<typename type::iterator, bool> InsertResult;
	}; 

	template <typename K, typename V, typename P = std::less<K> > 
	struct STL_map 
	{ 
		typedef typename std::map<K, V, P> type; 
	}; 

	template <typename K, typename V, typename P = std::less<K> > 
	struct STL_multimap 
	{ 
		typedef typename std::multimap<K, V, P> type; 
	};

	template <typename T1, typename T2>
	struct STL_pair
	{
	public:
		typedef std::pair<T1,T2> type;
	};

	template <typename T1>
	class STL_numeric_limits
	{
	public:	
		inline static T1 infinity()
		{
			return std::numeric_limits<T1>::infinity();
		}
		//inline static T1 max()
		//{
		//	return std::numeric_limits<T1>::max();
		//}
		//inline static T1 min()
		//{
		//	return std::numeric_limits<T1>::min();
		//}
	};

	class STL
	{
	public:
		template<typename _RanIt,typename _Pr> 
		inline static void sort(_RanIt _First, _RanIt _Last, _Pr _Pred)
		{
			std::sort(_First, _Last, _Pred);
		}
		template<typename _RanIt> 
		inline static void sort(_RanIt _First, _RanIt _Last)
		{
			std::sort(_First, _Last);
		}
		inline static char tolower(char ch)
		{
#if __WIN32__
			return ::tolower(ch);
#endif
			
			if (ch >= 'A' && ch <= 'Z')
			{
				ch += 0x20;
		}

			return ch;
		}
		inline static char toupper(char ch)
		{
#if __WIN32__
			return ::toupper(ch);
#endif
		
			if (ch >= 'a' && ch <= 'z')
			{
				ch -= 0x20;
			}

			return ch;
		}

		template<class _InIt,class _Diff> 
		inline static void advance(_InIt& _Where, _Diff _Off)
		{	
			std::advance(_Where, _Off);
		}

	};


}








#endif // __stl_H__
