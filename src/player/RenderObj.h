#pragma once
#include <memory>
#include "d3d12/PrimitiveGroupD3D12.h"

class RenderObj
{
public:
	RenderObj();
	~RenderObj();

	void Init(const std::shared_ptr<D3D12::PrimitiveGroupD3D12>& pg);
protected:
	std::shared_ptr<D3D12::PrimitiveGroupD3D12> m_pPrimitiveGroup;

private:
};
