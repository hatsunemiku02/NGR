#include "RenderTarget.h"
#include "d3d12/DescriptorHeap.h"


RenderTarget::RenderTarget()
{

}

RenderTarget::~RenderTarget()
{

}

void RenderTarget::Init(const std::shared_ptr<ViewPortD3D12>& pViewPort)
{
	D3D12::DescriptorHeap* pRTVHeap = D3D12::RenderDeviceD3D12::Instance()->GetRtvHeap();

	D3D12::CPUHandle& cpuhandle = pRTVHeap->GetCPUHandle();

	pViewPort->GetSwapChain()->GetBuffer(1, IID_PPV_ARGS(&m_pRenderTarget));

	D3D12::RenderDeviceD3D12::Instance()->GetDevice()->
	CreateRenderTargetView(m_pRenderTarget, nullptr, cpuhandle.handle);

}

