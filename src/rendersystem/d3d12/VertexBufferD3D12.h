#pragma once
#include "CommandQueue.h"
#include "base/VertexBuffer.h"
#include "VertexLayoutD3D12.h"

#if RENDERDEVICE_D3D12



namespace D3D12
{
class VertexBufferD3D12 : public RenderBase::VertexBuffer
{

public:

	/// constructor
	VertexBufferD3D12();
	/// destructor
	virtual ~VertexBufferD3D12();

	void Discard();

	void UpdateData(const RenderBase::DataStream& stream);

	static void CreateVertexBuffer(const std::shared_ptr<GraphicCommandList>& pCmdList,const std::shared_ptr<RenderBase::VertexBufferData>& vbd, std::shared_ptr<VertexBufferD3D12>& out);

	void ReleaseUploadBuffer();

	const D3D12_VERTEX_BUFFER_VIEW& GetView() const;

private:

	void _SetD3D12VertexBuffer(ID3D12Resource* ptr);
	void _SetUploadBuffer(ID3D12Resource* ptr);
	void _SetD3d12VertexLayout(std::shared_ptr<VertexLayoutD3D12>& vertexLayout);
	void _SetView(const D3D12_VERTEX_BUFFER_VIEW& view);

	ID3D12Resource*				m_pVertexBuffer;
	ID3D12Resource*				m_pUploadBuffer;
	D3D12_VERTEX_BUFFER_VIEW	m_VertexBufferView;
	byte*						m_pMappedData;
};

inline void VertexBufferD3D12::_SetD3D12VertexBuffer(ID3D12Resource* ptr)
{
	m_pVertexBuffer = ptr;
}

inline void VertexBufferD3D12::_SetUploadBuffer(ID3D12Resource* ptr)
{
	m_pUploadBuffer = ptr;
}

inline void VertexBufferD3D12::_SetD3d12VertexLayout(std::shared_ptr<VertexLayoutD3D12>& vertexLayout)
{
	this->vertexLayout = vertexLayout;
}

inline void VertexBufferD3D12::_SetView(const D3D12_VERTEX_BUFFER_VIEW& view)
{
	m_VertexBufferView = view;
}

inline void VertexBufferD3D12::ReleaseUploadBuffer()
{
	if (m_pUploadBuffer != NULL)
	{
		m_pUploadBuffer->Release();
	}
}

inline const D3D12_VERTEX_BUFFER_VIEW& VertexBufferD3D12::GetView() const
{
	return m_VertexBufferView;
}

}

#endif