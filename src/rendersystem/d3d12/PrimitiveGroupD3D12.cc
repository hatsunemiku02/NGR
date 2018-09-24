#include "stdneb.h"
#include "D3D12Types.h"
#include "PrimitiveGroupD3D12.h"

#if RENDERDEVICE_D3D12

namespace D3D12
{

	using namespace RenderBase;

PrimitiveGroupD3D12::PrimitiveGroupD3D12()
{

}

PrimitiveGroupD3D12::~PrimitiveGroupD3D12()
{
	m_vertexBuffer = 0;
	m_indexBuffer  = 0;
}

void PrimitiveGroupD3D12::Discard()
{
	m_vertexBuffer = 0;
	m_indexBuffer  = 0;
}

void PrimitiveGroupD3D12::LoadBuffers(const std::shared_ptr<GraphicCommandList>& pCmdList, const std::shared_ptr<RenderBase::VertexBufferData>& vbd, const std::shared_ptr<RenderBase::IndexBufferData>& ibd)
{
	assert(PrimitiveTopology::InvalidPrimitiveTopology !=  vbd->topology);

	SetBoundingBox(Math::bbox::Zero);
	if (vbd)
	{
		SetPrimitiveTopology(vbd->topology);
		assert(nullptr == m_vertexBuffer);
		SetBaseVertex(0);
		SetNumVertices(vbd->vertexCount);

		std::shared_ptr<VertexBufferD3D12> out;
		VertexBufferD3D12::CreateVertexBuffer(pCmdList,vbd, out);
		m_vertexBuffer = out;
	}

	//copy index data
// 	if (ibd)
// 	{
// 		assert(nullptr == m_indexBuffer);
// 		SetBaseIndex(0);
// 		SetNumIndices(ibd->indexCount);
// 
// 		std::shared_ptr<IndexBufferD3D12> out;
// 		IndexBufferD3D12::CreateIndexBuffer(ibd, out);
// 		m_indexBuffer = out;
// 	}
}

}


#endif