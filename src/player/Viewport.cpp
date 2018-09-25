#include "ViewPort.h"
#include "d3d12/D3D12Types.h"
#include <wrl.h>
using Microsoft::WRL::ComPtr;

ViewPortD3D12::ViewPortD3D12()
{

}

ViewPortD3D12::~ViewPortD3D12()
{

}

void ViewPortD3D12::Init(RenderBase::PixelFormat::Code format,UINT width, UINT height, HWND hwnd)
{
	m_PixelFormat = format;
	m_ViewPort = CD3DX12_VIEWPORT(0.0f, 0.0f, static_cast<float>(width), static_cast<float>(height));
	m_ScissorRect = CD3DX12_RECT(0, 0, static_cast<LONG>(width), static_cast<LONG>(height));

	DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
	swapChainDesc.BufferCount = 2;
	swapChainDesc.Width = width;
	swapChainDesc.Height = height;
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;// D3D12::D3D12Types::AsD3D12PixelFormat(m_PixelFormat);
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapChainDesc.SampleDesc.Count = 1;

	IDXGIFactory4* factory = D3D12::RenderDeviceD3D12::Instance()->GetDXGI();
	ComPtr<IDXGISwapChain1> swapChain;
	HRESULT hr = factory->CreateSwapChainForHwnd(
		D3D12::RenderDeviceD3D12::Instance()->GetCmdQueue(),		// Swap chain needs the queue so that it can force a flush on it.
		hwnd,
		&swapChainDesc,
		nullptr,
		nullptr,
		&swapChain
	);
	ComPtr<IDXGISwapChain3> swapChain3;
	hr = swapChain.As(&swapChain3);
	m_pViewPortSwapChain = swapChain3.Get();
	// This sample does not support fullscreen transitions.
	hr = factory->MakeWindowAssociation(hwnd, DXGI_MWA_NO_ALT_ENTER);
	D3D12::DescriptorHeap* pRTVHeap = D3D12::RenderDeviceD3D12::Instance()->GetRtvHeap();
	for (IndexT i = 0; i < 2; ++i)
	{
		m_pRenderTargetHandle[i] = &pRTVHeap->GetCPUHandle();
		hr = m_pViewPortSwapChain->GetBuffer(i, IID_PPV_ARGS(&m_pRenderTarget[i]));
		if (FAILED(hr))
		{
			assert(false);
		}

		D3D12::RenderDeviceD3D12::Instance()->GetDevice()->CreateRenderTargetView(m_pRenderTarget[i], NULL, m_pRenderTargetHandle[i]->handle);
	}

}
