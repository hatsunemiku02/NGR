

#pragma once
//------------------------------------------------------------------------------
/**
    @file core/debug.h

    Nebula debug macros.
  
    n_assert()  - the vanilla assert() Macro
    n_verify()  - like assert() except that the statement in parens is simply evaluated, unchecked, if __NEBULA_NO_ASSERT is set
    n_assert2() - an assert() plus a message from the programmer
*/
#include "core/config.h"


#if __ANDROID__
#include <android/log.h>
#endif
#if __OSX__ || __ANDROID__
#include <assert.h>
#endif

#ifndef __ANDROID__
	void log_printf(const char *, ...) __attribute__((format(printf,1,2)));
	void log_error(const char*, ...) __attribute__((format(printf,1,2)));
	void log_dbgout(const char*, ...) __attribute__((format(printf,1,2)));
	void log_warning(const char*, ...) __attribute__((format(printf,1,2)));
	void log_confirm(const char*, ...) __attribute__((format(printf,1,2)));
	void log_debuglog(const char*, ...) __attribute__((format(printf,1,2)));

	void script_printf(const char*, ...) __attribute__((format(printf,1,2)));
	void script_dbgout(const char*, ...) __attribute__((format(printf,1,2)));
	void script_warning(const char*, ...) __attribute__((format(printf,1,2)));
#endif

	inline void log_nothing(const char*, ...) {}

#if defined( DEBUG ) || defined( _DEBUG )
	#if __ANDROID__
		#define n_printf(...) __android_log_print(ANDROID_LOG_VERBOSE, "Genesis3D", __VA_ARGS__)
		#define n_error(...)  __android_log_print(ANDROID_LOG_ERROR, "Genesis3D", __VA_ARGS__)
		#define n_dbgout(...) __android_log_print(ANDROID_LOG_DEBUG, "Genesis3D", __VA_ARGS__)
		#define n_warning(...)  __android_log_print(ANDROID_LOG_WARN, "Genesis3D", __VA_ARGS__)
		#define n_confirm(...)  __android_log_print(ANDROID_LOG_INFO, "Genesis3D", __VA_ARGS__)
		#define n_debuglog(...)  __android_log_print(ANDROID_LOG_INFO, "Genesis3D", __VA_ARGS__)
		#define sys_warning(...)  __android_log_print(ANDROID_LOG_WARN, "Genesis3D", __VA_ARGS__)
	#else
		#define  n_printf(...) log_printf(__VA_ARGS__)
		#define  n_error(...) log_error(__VA_ARGS__)
		#define  n_dbgout(...) log_dbgout(__VA_ARGS__)
		#define  n_warning(...) log_warning(__VA_ARGS__) 
		#define  n_confirm( ...) log_confirm(__VA_ARGS__)
		#if defined( DEBUG ) || defined( _DEBUG )
			#define	 n_debuglog(...) log_debuglog(__VA_ARGS__)
		#else
			#define n_debuglog(...)
		#endif // _DEBUG
		#define sys_warning(...)  log_dbgout(__VA_ARGS__)
	#endif
#else
	#define  n_printf(...) log_nothing(__VA_ARGS__)
	#define  n_error(...) log_nothing(__VA_ARGS__)
	#define  n_dbgout(...) log_nothing(__VA_ARGS__)
	#define  n_warning(...) log_nothing(__VA_ARGS__)
	#define  n_confirm( ...) log_nothing(__VA_ARGS__)
	#define	 n_debuglog(...) log_nothing(__VA_ARGS__)
#if __ANDROID__
	#define sys_warning(...)  __android_log_print(ANDROID_LOG_WARN, "Genesis3D", __VA_ARGS__)
#else
	#define sys_warning(...) log_warning(__VA_ARGS__) 
#endif
	
#endif

#if __ANDROID__
	#define script_printf(...) __android_log_print(ANDROID_LOG_VERBOSE, "Genesis3D", __VA_ARGS__)
	#define script_error(...)  __android_log_print(ANDROID_LOG_ERROR, "Genesis3D", __VA_ARGS__)
	#define script_dbgout(...) __android_log_print(ANDROID_LOG_DEBUG, "Genesis3D", __VA_ARGS__)
	#define script_warning(...)  __android_log_print(ANDROID_LOG_WARN, "Genesis3D", __VA_ARGS__)
	#define script_confirm(...)  __android_log_print(ANDROID_LOG_INFO, "Genesis3D", __VA_ARGS__)
#else
	//#define script_printf(...) log_printf(__VA_ARGS__)
	#define script_error(...) log_error(__VA_ARGS__)
	//#define script_dbgout(...) log_dbgout(__VA_ARGS__)
	//#define script_warning(...) log_warning(__VA_ARGS__) 
	#define script_confirm(...) log_confirm(__VA_ARGS__)
#endif

typedef void (*__pCustomAssertCallback) (const char* exp_type, const char* msg, const char* file, int line);
void n_setCustomAssert();
void n_custom_assert(const char* exp, const char* file, int line);
void n_custom_assert2(const char* exp, const char* msg, const char* file, int line);


void n_sleep(double);
void n_barf(const char *, const char *, int);
void n_barf2(const char*, const char*, const char*, int);



// backward compatibility
#define n_message n_confirm

// Static compile-time assertion
namespace StaticAssert
{
	template< bool > struct FAILED;
	template<> struct FAILED< true > { };
}
// use template in compile check. so can use in Release mode
#define n_static_assert( exp ) (StaticAssert::FAILED< (exp) != 0 >())  




#if (__NEBULA_NO_ASSERT__ == 1)
#	define n_assert(exp) if(!(exp)){}
#	define n_verify(exp) (exp)
#	define n_verify2(exp,imsg) (exp)
#	define n_assert2(exp, msg) if(!(exp)){}
#	if __WIN32__ || __WIN64__
#		define n_dxtrace(hr, msg)
#	endif
	//#define n_static_assert(exp)
#elif (__NEBULA_NO_ASSERT__ == 0)

#	if __OSX__ || __ANDROID__
#		define n_assert(exp) assert(exp);
#		define n_assert2(exp, msg) assert(exp);
#	endif
#	if __WIN32__ || __WIN64__
#		define n_assert(exp) { if (!(exp)) n_custom_assert(#exp,__FILE__,__LINE__); }
#		define n_assert2(exp, msg) { if (!(exp)) n_custom_assert2(#exp,msg,__FILE__,__LINE__); }
#	endif
	//#define n_static_assert(exp) { int _x[ 2*((exp) != 0)-1 ]; (void)(_x[0]=0);}	
#	if __WIN32__ || __WIN64__
	// dx9 specific: check HRESULT and display DX9 specific message box
#		define n_dxtrace(hr, msg) { if (FAILED(hr)) DXTrace(__FILE__,__LINE__,hr,msg,true); }
#	endif
#endif

//------------------------------------------------------------------------------

