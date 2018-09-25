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

void RenderObj::GenerateInternal(const std::shared_ptr<ViewPortD3D12>& pViewPort)
{
	RenderBase::PsoData psoData = m_pMaterial->GetPsoData();

	psoData.pVertexLayout =	m_pPrimitiveGroup->GetVertexBuffer()->GetVertexLayout();

// 	RenderBase::VertexLayoutDesc vertLayoutDesc;
// 	vertLayoutDesc.name = "POSITION";
// 	vertLayoutDesc.offset = 0;
// 	vertLayoutDesc.si = 0;
//	psoData.vertexLayout.push_back(vertLayoutDesc);
	RenderBase::DeviceBlendState blendState;
	psoData.renderBlendState = blendState;
	RenderBase::DeviceRasterizerState rasterState;
	psoData.renderRasterizerState = rasterState;
	RenderBase::DeviceDepthAndStencilState dsState;
	psoData.renderDepthStencilState = dsState;
	psoData.rtvFormat = RenderBase::PixelFormat::A8R8G8B8;

	


	RenderBase::SignatureInfo sigInfo;
}

