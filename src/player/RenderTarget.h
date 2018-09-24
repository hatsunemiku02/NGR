#pragma once
#include <memory>
#include "d3d12/RenderDeviceD3D12.h"
#include "ViewPort.h"

class RenderTarget
{
public:
	RenderTarget();
	~RenderTarget();
	void Init(const std::shared_ptr<ViewPortD3D12>& pViewPort);
	ID3D12Resource* GetRenderTarget()
	{
		return m_pRenderTarget;
	}
protected:
	ID3D12Resource * m_pRenderTarget;
private:
};