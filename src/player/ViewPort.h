#pragma once
#include "d3d12/RenderDeviceD3D12.h"
#include "d3d12/DescriptorHeap.h"
#include "base/PixelFormat.h"
class ViewPortD3D12
{
public:
	ViewPortD3D12();
	virtual ~ViewPortD3D12();

	void Init(UINT width, UINT height);

	CD3DX12_VIEWPORT& GetViewPort()
	{
		return m_ViewPort;
	}
	CD3DX12_RECT& GetRect()
	{
		return m_ScissorRect;
	}

	uint GetWidth()
	{
		return m_ViewPort.Width;
	}

	uint GetHeight()
	{
		return m_ViewPort.Height;
	}

protected:

	CD3DX12_VIEWPORT	m_ViewPort;
	CD3DX12_RECT		m_ScissorRect;
	IDXGISwapChain3*	m_pViewPortSwapChain;


private:
};
