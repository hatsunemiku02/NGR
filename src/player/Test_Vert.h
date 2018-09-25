#pragma once
#include "d3d12/VertexBufferD3D12.h"
#include "math/float3.h"
#include <memory>

struct Vertex
{
	Math::float4 position;
};



class TestVert
{
public:
	void SetupVertData();

	std::shared_ptr<RenderBase::VertexBufferData> m_pData;

	Math::float3* m_pVert;

};

void TestVert::SetupVertData()
{
	m_pData = std::make_shared<RenderBase::VertexBufferData>();
	m_pData->topology = RenderBase::PrimitiveTopology::TriangleList;
	m_pData->vertexCount = 3;
	m_pData->vertex.vertexComponents.emplace_back(RenderBase::VertexComponent::Position, 0, RenderBase::VertexComponent::Float4, 0);
	m_pData->vertex.vertexComponentStreams.emplace_back();
	m_pVert = new Math::float3[3]();
	m_pVert[0].x() = 0.0f;
	m_pVert[0].y() = 0.33f;
	m_pVert[0].z() = 0.0f;
	

	m_pVert[1].x() = 0.25f;
	m_pVert[1].y() = -0.33f;
	m_pVert[1].z() = 0.0f;


	m_pVert[2].x() = -0.25f;
	m_pVert[2].y() =-0.33f;
	m_pVert[2].z() = 0.0f;
	
	m_pData->vertex.vertexComponentStreams[0].data = m_pVert;
	m_pData->vertex.vertexComponentStreams[0].offsetInByte = 0;
	m_pData->vertex.vertexComponentStreams[0].elemSizeInByte = 12;
}

