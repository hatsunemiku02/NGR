#include "stdneb.h"
#include "D3D12Window.h"
#if RENDERDEVICE_D3D12
#include "RenderDeviceD3D12.h"
namespace D3D12
{
	D3D12Window::D3D12Window(WindHandle winHandle)
		: RenderBase::RenderWindow( winHandle)
		, m_BeginRender(false)
	{
		//do nothing
	}


	D3D12Window::~D3D12Window()
	{	
		//do nothing
	}

	void D3D12Window::BeginRender()
	{
		//m_RenderDevice->BeginFrame();
		m_BeginRender = true;
	}

	void D3D12Window::EndRender()
	{
		if (m_BeginRender)
		{
			RenderDeviceD3D12::Instance()->SetSwapChain(m_TargetHandle);
			RenderDeviceD3D12::Instance()->Present(m_WinHandle);
			m_BeginRender = false;
		}
	}
}

#endif