#pragma once
#include <memory>
#include "d3d12/RenderDeviceD3D12.h"
#include "d3d12/CommandQueue.h"
#include "ViewPort.h"

class RenderTarget
{
public:
	RenderTarget();
	~RenderTarget();
	void Init(IDXGISwapChain1* pSwapChain, RenderBase::PixelFormat::Code format, uint index);
	void Init(const std::shared_ptr<D3D12::TextureD3D12>& pViewPort);
	void GotoWriteState(const std::shared_ptr<D3D12::GraphicCommandList>& cmdList);
	void GotoPresentState(const std::shared_ptr<D3D12::GraphicCommandList>& cmdList);


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