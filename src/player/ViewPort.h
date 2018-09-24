#pragma once
#include "d3d12/RenderDeviceD3D12.h"
#include "d3d12/DescriptorHeap.h"
class ViewPortD3D12
{
public:
	ViewPortD3D12();
	virtual ~ViewPortD3D12();

	void Init(UINT width, UINT height, HWND hwnd);

	CD3DX12_VIEWPORT& GetViewPort()
	{
		return m_ViewPort;
	}
	CD3DX12_RECT& GetRect()
	{
		return m_ScissorRect;
	}
	IDXGISwapChain1* GetSwapChain()
	{
		return m_pViewPortSwapChain;
	}
	int GetCurrentFrameIndex()
	{
		return m_pViewPortSwapChain->GetCurrentBackBufferIndex();
	}

	ID3D12Resource* GetCurrentRT()
	{
		return m_pRenderTarget[m_pViewPortSwapChain->GetCurrentBackBufferIndex()];
	}

	D3D12::CPUHandle* GetCurrentRTHandle()
	{
		return m_pRenderTargetHandle[m_pViewPortSwapChain->GetCurrentBackBufferIndex()];
	}
protected:

	CD3DX12_VIEWPORT	m_ViewPort;
	CD3DX12_RECT		m_ScissorRect;
	IDXGISwapChain3*	m_pViewPortSwapChain;
	ID3D12Resource *   m_pRenderTarget[2];
	D3D12::CPUHandle*   m_pRenderTargetHandle[2];
private:
};
