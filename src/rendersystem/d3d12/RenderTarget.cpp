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

void RenderTarget::Init(const std::shared_ptr<Texture>& pTexture)
{
	D3D12::DescriptorHeap* pRTVHeap = D3D12::RenderDeviceD3D12::Instance()->GetRtvHeap();

	m_pRenderTargetHandle = &pRTVHeap->GetCPUHandle();

	m_pRenderTarget = pTexture->GetTextureRes();

	D3D12::RenderDeviceD3D12::Instance()->GetDevice()->
		CreateRenderTargetView(m_pRenderTarget, nullptr, m_pRenderTargetHandle->handle);

	m_RTFormat = pTexture->GetTextureFormat();

	m_pBindColorTex = pTexture;
}


