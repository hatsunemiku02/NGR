#include "RenderObj.h"
RenderObj::RenderObj()
{

}

RenderObj::~RenderObj()
{

}

void RenderObj::Init(const std::shared_ptr<D3D12::PrimitiveGroupD3D12>& pg)
{
	m_pPrimitiveGroup = pg;
}

