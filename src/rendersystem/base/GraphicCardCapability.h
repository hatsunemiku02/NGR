#pragma once

#ifndef GRAPHICCARDCAPABILITY_H_
#define GRAPHICCARDCAPABILITY_H_
#ifdef __OSX__
#include "../rendersystem/config/RenderDeviceConfig.h"
#include "../rendersystem/base/RenderDeviceTypes.h"
#else
#include "config/RenderDeviceConfig.h"
#include "base/RenderDeviceTypes.h"
#endif


namespace RenderBase
{

struct GraphicCardCapability
{
	GraphicCardCapability()
	{
	}
	static 	int  s_mMaxUniformVectors;
	std::string mDriverName;
	std::string mApiVersion;

	SizeT mMaxTextureWidth;
	SizeT mMaxTextureHeight;
	bool  mSRGBWrite;
	bool  mSRGBRead;
	
#if RENDERDEVICE_OPENGLES20 || RENDERDEVICE_OPENGLES30
	int  mHighFloatRange[2];
	int  mHighFloatPrecision;
	bool mFragmentHightPrecision;
	bool mIndexUint;
	bool mDepthFormat;
	bool mSeparateAplhaBlend;
	bool mStencilTwoSize;
	bool mS3TC;
#endif
	bool mMSAA;

#if RENDERDEVICE_D3D9
	void DetectGraphicCardCapsD3D9();
#endif
#if RENDERDEVICE_OPENGLES20
	void DetectGraphicCardCapsGLES20();
#elif RENDERDEVICE_OPENGLES30
	void DetectGraphicCardCapsGLES30();
#endif

};
}
#endif




