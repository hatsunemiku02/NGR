#ifndef RENDERDEVICECONFIG_H
#define RENDERDEVICECONFIG_H
#include "core/config.h"

#if __WIN64__
#define RENDERDEVICE_D3D12 1
#endif

// #ifdef WIN32
// ////#include <dxerr9.h>
// #	ifndef DXTRACE_ERR
// #		define DXTRACE_ERR(x,y)
// #	endif
// #	ifndef DXGetErrorString9
// #		define DXGetErrorString9(x) ""
// #	endif // DXGetErrorString9
// #	ifndef DXGetErrorDescription9
// #		define DXGetErrorDescription9(x) ""
// #	endif
// 
// #	define RENDERDEVICE_NULL 0
// #	define RENDERDEVICE_D3D9 1
// #	define RENDERDEVICE_D3D11 0
// #	define RENDERDEVICE_D3D12 0
// #	define RENDERDEVICE_OPENGLES20 0
// #	define RENDERDEVICE_OPENGLES30 0
// 
// #elif __ANDROID__ || __OSX__
// #	define RENDERDEVICE_OPENGLES20 0
// #	define RENDERDEVICE_OPENGLES30 1
// #	define RENDERDEVICE_NULL 0
// #endif

#endif// RENDERDEVICECONFIG_H