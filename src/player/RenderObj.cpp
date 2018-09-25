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

	RenderBase::DeviceBlendState blendState;
	psoData.renderBlendState = blendState;
	RenderBase::DeviceRasterizerState rasterState;
	psoData.renderRasterizerState = rasterState;
	RenderBase::DeviceDepthAndStencilState dsState;
	psoData.renderDepthStencilState = dsState;
	psoData.rtvFormat = pViewPort->GetPixelFormat();

	RenderBase::SignatureInfo sigInfo = m_pMaterial->GetRootSigInfo();
	m_pRootSig = std::make_shared<D3D12::DX12RootSignature>();
	m_pRootSig->Init(sigInfo);

	m_pPipeStateObj = std::make_shared<D3D12::DX12Pso>();
	m_pPipeStateObj->Init(psoData, m_pRootSig);

}

