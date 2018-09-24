#pragma once

#include "config/RenderDeviceConfig.h"
#include "../base/RenderWindow.h"

namespace D3D12
{
	class RenderDeviceD3D12;
	class D3D12Window : public RenderBase::RenderWindow
	{
	public:
		D3D12Window(WindHandle winHandle);
		virtual ~D3D12Window();
		virtual void		BeginRender();
		virtual void		EndRender();

	private:
		bool m_BeginRender;
	};
}