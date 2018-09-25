 

#include "stdneb.h"
#include "core/types.h"




//------------------------------------------------------------------------------
/**
    This function is called by n_assert() when the assertion fails.
*/
void 
n_barf(const char* exp, const char* file, int line)
{
  
}

//------------------------------------------------------------------------------
/**
    This function is called by n_assert2() when the assertion fails.
*/
void
n_barf2(const char* exp, const char* msg, const char* file, int line)
{
  
}

//------------------------------------------------------------------------------
/**
    This function is called when a serious situation is encountered which
    requires abortion of the application.
*/

//#if (_DEBUG || DEBUG || __WIN32__ ) && ( __WIN32__ ||__OSX__)
#ifndef __ANDROID__
void __cdecl
log_error(const char* msg, ...)
{
	
}

//------------------------------------------------------------------------------
/**
    This function is called when a warning should be issued which doesn't
    require abortion of the application.
*/
void __cdecl
log_warning(const char* msg, ...)
{
  
}        

//------------------------------------------------------------------------------
/**
    This function is called when a message should be displayed to the
    user which requires a confirmation (usually displayed as a MessageBox).
*/
void __cdecl
log_confirm(const char* msg, ...)
{
   
}

//------------------------------------------------------------------------------
/**
    Nebula's printf replacement. Will redirect the text to the console
    and/or logfile.

     - 27-Nov-98   floh    created
*/
void __cdecl
log_printf(const char *msg, ...)
{
	
}

//------------------------------------------------------------------------------
/**
    Put the message to the debug window.

    - 26-Mar-05    kims    created
*/
void __cdecl
log_dbgout(const char *msg, ...)
{
 
}
#if defined( DEBUG ) || defined( _DEBUG )
void __cdecl
log_debuglog(const char *msg, ...)
{

}
#endif
#endif
//------------------------------------------------------------------------------
#ifndef __ANDROID__
void __cdecl
script_printf(const char* msg, ...)
{

}
//------------------------------------------------------------------------------
void __cdecl
script_dbgout(const char* msg, ...)
{
	
}
//------------------------------------------------------------------------------
void __cdecl
script_warning(const char* msg, ...)
{
	
}
#endif
//------------------------------------------------------------------------------
/**
    Put process to sleep.

     - 21-Dec-98   floh    created
*/
void 
n_sleep(double sec)
{
   
}

//------------------------------------------------------------------------

void n_setCustomAssert(  )
{
	
}
//------------------------------------------------------------------------
#if (__NEBULA_NO_ASSERT__ == 0)
void n_custom_assert( const char* exp, const char* file, int line )
{
	
}

void n_custom_assert2( const char* exp, const char* msg, const char* file, int line )
{
	
}

#endif
//------------------------------------------------------------------------

