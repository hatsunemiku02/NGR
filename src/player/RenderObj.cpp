#include "RenderObj.h"

const uint POSTION_BUFFER_IDX = 0;
const uint POSTION_BUFFER_SIZE = 16;

RenderObj::RenderObj()
{

}

RenderObj::~RenderObj()
{

}

void RenderObj::Init(const std::shared_ptr<D3D12::PrimitiveGroupD3D12>& pg)
{
	m_pPrimitiveGroup = pg;

	m_ConstantBuffers.emplace_back();
	m_ConstantBuffers[POSTION_BUFFER_IDX].CreateBuffer(POSTION_BUFFER_SIZE);
}

void RenderObj::UpdatePosBuffer(const RenderBase::DataStream& posBuffer)
{
	m_ConstantBuffers[POSTION_BUFFER_IDX].UpdateBuffer(posBuffer);
}

void RenderObj::GenerateInternal(const Pipeline2MatInfo& extrenalInfo)
{
	RenderBase::PsoData psoData = m_pMaterial->GetPsoData();

	psoData.pVertexLayout =	m_pPrimitiveGroup->GetVertexBuffer()->GetVertexLayout();

	psoData.rtvFormat = extrenalInfo.m_RTPixelFormat;

	RenderBase::SignatureInfo sigInfo = m_pMaterial->GetRootSigInfo();
	//insert position
	sigInfo.cbvInfo.insert(sigInfo.cbvInfo.begin(), POSTION_BUFFER_IDX);

	m_pRootSig = std::make_shared<D3D12::DX12RootSignature>();
	m_pRootSig->Init(sigInfo);

	m_pPipeStateObj = std::make_shared<D3D12::DX12Pso>();
	m_pPipeStateObj->Init(psoData, m_pRootSig);

}

