#pragma once
#include <memory>
#include "d3d12/RenderDeviceD3D12.h"
#include "d3d12/CommandQueue.h"
#include "ViewPort.h"
#include "Texture.h"

class RenderTarget
{
public:
	RenderTarget();
	~RenderTarget();
	void Init(IDXGISwapChain1* pSwapChain, RenderBase::PixelFormat::Code format, uint index);
	void Init(const std::shared_ptr<Texture>& pTexture);

	ID3D12Resource* GetRenderTarget()
	{
		return m_pRenderTarget;
	}
	D3D12::CPUHandle* GetRenderTargetHandke()
	{
		return m_pRenderTargetHandle;
	}
	RenderBase::PixelFormat::Code GetRTFormat()
	{
		return m_RTFormat;
	}
protected:
	ID3D12Resource * m_pRenderTarget;
	D3D12::CPUHandle* m_pRenderTargetHandle;

	RenderBase::PixelFormat::Code m_RTFormat;
private:
};