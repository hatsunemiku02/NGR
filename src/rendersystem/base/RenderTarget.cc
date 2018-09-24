/****************************************************************************
Copyright (c) 2007, Radon Labs GmbH
Copyright (c) 2011-2013,WebJet Business Division,CYOU
 
http://

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/

//------------------------------------------------------------------------------
//  rendertargetbase.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"

#include "RenderTarget.h"

namespace RenderBase
{


	//------------------------------------------------------------------------------
	/**
	*/
	RenderTarget::RenderTarget() :
		width(0),
		height(0),
		resolveTextureDimensionsValid(false),
		resolveRectValid(false),
		resolveTextureWidth(0),
		resolveTextureHeight(0),
		resolveRect(0, 0, 0, 0),
		mrtIndex(0),
		clearFlags(0),
		memorySize(0),
		clearColor(0.0f, 0.0f, 1.0f, 0.0f),
		clearDepth(1.0f),
		clearStencil(0),
		antiAliasQuality(AntiAliasQuality::None),
		colorBufferFormat(PixelFormat::X8R8G8B8),
		mipMapsEnabled(false),
		isValid(false),    
		inBeginPass(false),
		inBeginBatch(false),
		hasDepthStencilBuffer(false),
		isDefaultRenderTarget(false),
		resolveCpuAccess(false),
		screenSizeRatio(0.0),
		attahcedToMRT(false),
		dummyRT(false),
		bColorBuffer(true)
	{
		SetRenderCommandType(RenderCommandType::SetRenderTarget);
	}

	//------------------------------------------------------------------------------
	/**
	*/
	RenderTarget::~RenderTarget()
	{
		assert(!this->isValid);
	}

	//------------------------------------------------------------------------------
	/**
	*/
	void
		RenderTarget::setup()
	{
		assert(!this->isValid);
		this->isValid = true;
		SetRenderCommandType(RenderCommandType::SetRenderTarget);
// 		PROFILER_ADDDEVICESTATEVAL(videoRam, memorySize);
// 		PROFILER_ADDDEVICESTATEVAL(fboRam, memorySize);
// 		PROFILER_ADDDEVICESTATEVAL(renderTargetNum, 1);
	}

	//------------------------------------------------------------------------------
	/**
	*/
	void
		RenderTarget::Discard()
	{
// 		PROFILER_ADDDEVICESTATEVAL(videoRam, -memorySize);
// 		PROFILER_ADDDEVICESTATEVAL(fboRam, -memorySize);
// 		PROFILER_ADDDEVICESTATEVAL(renderTargetNum, -1);
		memorySize = 0;

		assert(this->isValid);
		assert(!this->inBeginPass);
		assert(!this->inBeginBatch);
		this->sharedDepthStencilBufferTarget = 0;
		if (this->resolveTexture)
		{
			this->resolveTexture = 0;
		}
		if (this->resolveCPUTexture)
		{
			resolveCPUTexture = 0;
		}
		if (this->resolveDepthTexture)
		{
			resolveDepthTexture = 0;
		}
		this->isValid = false;
	}

	//------------------------------------------------------------------------------
	/**
	*/
	void
		RenderTarget::AddDepthStencilBuffer()
	{
		assert(!this->hasDepthStencilBuffer);
		this->hasDepthStencilBuffer = true;
		this->sharedDepthStencilBufferTarget = 0;
	}

	//------------------------------------------------------------------------------
	/**
	*/
	void
		RenderTarget::AddSharedDepthStencilBuffer(const std::shared_ptr<RenderTarget>& rt)
	{
		assert(!this->hasDepthStencilBuffer);
		this->hasDepthStencilBuffer = true;
		this->sharedDepthStencilBufferTarget = rt;
	}

	//------------------------------------------------------------------------------
	/**
	*/
	void
		RenderTarget::BeginPass()
	{
		assert(this->isValid);
		assert(!this->inBeginPass);
		assert(!this->inBeginBatch);
		this->inBeginPass = true;
	}

	//------------------------------------------------------------------------------
	/**
	*/
	void
		RenderTarget::BeginBatch()
	{
		assert(this->inBeginPass);
		assert(!this->inBeginBatch);
		this->inBeginBatch = true;
	}

	//------------------------------------------------------------------------------
	/**
	*/
	void
		RenderTarget::EndBatch()
	{
		assert(this->inBeginBatch);
		this->inBeginBatch = false; 
	}

	//------------------------------------------------------------------------------
	/**
	*/
	void
		RenderTarget::EndPass()
	{
		assert(this->isValid);
		assert(this->inBeginPass);
		assert(!this->inBeginBatch);
		this->inBeginPass = false;
	}

	//------------------------------------------------------------------------------
	/**
	*/
	void
		RenderTarget::GenerateMipLevels()
	{
		assert(this->mipMapsEnabled);
	}

	//------------------------------------------------------------------------------
	/**
	*/
	void
		RenderTarget::ResolveDepthBuffer()
	{
		printf("RenderTargetBase::ResolveDepthBuffer not implemented!");
		assert(0);
	}

	//------------------------------------------------------------------------------
	/**
	*/
	int 
		RenderTarget::GetMemorySize()
	{
		return memorySize;
	}

	//------------------------------------------------------------------------------
	/**
	*/
	void 
		RenderTarget::CopyFrom(const Math::float4& srcRect, const std::shared_ptr<RenderTarget>srcRT, const Math::float4& desRect)
	{
		printf("this function need to be implemented in child class!");
		assert(0);
	}

	//------------------------------------------------------------------------------
	/**
	*/
	void
		RenderTarget::OnDeviceLost()
	{

	}

	//------------------------------------------------------------------------------
	/**
	*/
	void 
		RenderTarget::OnDeviceReset()
	{

	}

	//------------------------------------------------------------------------------
	/**
	*/
	void 
		RenderTarget::Resolve()
	{

	}
} // namespace Base