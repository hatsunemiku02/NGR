#include "RenderTarget.h"
#include "d3d12/DescriptorHeap.h"


RenderTarget::RenderTarget()
{

}

RenderTarget::~RenderTarget()
{

}

void RenderTarget::Init(IDXGISwapChain1* pSwapChain, RenderBase::PixelFormat::Code format,uint index)
{
	D3D12::DescriptorHeap* pRTVHeap = D3D12::RenderDeviceD3D12::Instance()->GetRtvHeap();

	m_pRenderTargetHandle = &pRTVHeap->GetCPUHandle();

	pSwapChain->GetBuffer(index, IID_PPV_ARGS(&m_pRenderTarget));

	D3D12::RenderDeviceD3D12::Instance()->GetDevice()->
	CreateRenderTargetView(m_pRenderTarget, nullptr, m_pRenderTargetHandle->handle);

	m_RTFormat = format;
}

void RenderTarget::Init(const std::shared_ptr<D3D12::TextureD3D12>& pViewPort)
{
	assert(0);
}

void RenderTarget::GotoWriteState(const std::shared_ptr<D3D12::GraphicCommandList>& cmdList)
{
	cmdList->GetCommandList()
		->ResourceBarrier(1,
			&CD3DX12_RESOURCE_BARRIER::Transition(m_pRenderTarget,
				D3D12_RESOURCE_STATE_PRESENT,
				D3D12_RESOURCE_STATE_RENDER_TARGET));
}

void RenderTarget::GotoPresentState(const std::shared_ptr<D3D12::GraphicCommandList>& cmdList)
{
	cmdList->GetCommandList()->ResourceBarrier
	(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_pRenderTarget, D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));

}

