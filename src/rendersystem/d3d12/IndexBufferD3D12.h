#pragma once

#include "D3D12Heads.h"
#include "base/IndexBuffer.h"

#if RENDERDEVICE_D3D12


namespace D3D12
{
	class IndexBufferD3D12 : public RenderBase::IndexBuffer
{
public:

	/// constructor
	IndexBufferD3D12();
	/// destructor
	virtual ~IndexBufferD3D12();

	/// clear d3d resource
	void Discard();

	void UpdateData(const RenderBase::DataStream& stream);

	static void CreateIndexBuffer(const std::shared_ptr<RenderBase::IndexBufferData>& ibd, std::shared_ptr<IndexBufferD3D12>& out);

	void SetD3D12IndexBuffer(ID3D12Resource* ptr);
	void SetUploadBuffer(ID3D12Resource* ptr);
	void SetView(const D3D12_INDEX_BUFFER_VIEW& view);
	const D3D12_INDEX_BUFFER_VIEW& GetView() const;

private:

	ID3D12Resource*				m_pIndexBuffer;
	ID3D12Resource*				m_pUploadBuffer;
	D3D12_INDEX_BUFFER_VIEW		m_IndexBufferView;
	byte*						m_pMappedData;

};

inline void IndexBufferD3D12::SetD3D12IndexBuffer(ID3D12Resource* ptr)
{
	m_pIndexBuffer = ptr;
}

inline void IndexBufferD3D12::SetUploadBuffer(ID3D12Resource* ptr)
{
	m_pUploadBuffer = ptr;
}

inline void IndexBufferD3D12::SetView(const D3D12_INDEX_BUFFER_VIEW& view)
{
	m_IndexBufferView = view;
}

inline const D3D12_INDEX_BUFFER_VIEW& IndexBufferD3D12::GetView() const
{
	return m_IndexBufferView;
}

}

#endif
