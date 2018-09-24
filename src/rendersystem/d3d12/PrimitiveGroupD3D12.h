#pragma once

#include "../config/RenderDeviceConfig.h"

#if RENDERDEVICE_D3D12

#include "VertexBufferD3D12.h"
#include "IndexBufferD3D12.h"
#include "base/PrimitiveGroup.h"
#include "RenderDeviceD3D12.h"

namespace D3D12
{
	class PrimitiveGroupD3D12 : public PrimitiveGroup
{
public:

	// constructor
	PrimitiveGroupD3D12();
	// destructor
	virtual ~PrimitiveGroupD3D12();

	/// discard
	virtual void Discard();

	void DiscardVertexBuffer();

	void DiscardIndexBuffer();

	void LoadBuffers(const std::shared_ptr<GraphicCommandList>& pCmdList,const std::shared_ptr<RenderBase::VertexBufferData>& vbd, const std::shared_ptr<RenderBase::IndexBufferData>& ibd);

	/// get VertexBuffer
	std::shared_ptr<VertexBufferD3D12> GetVertexBuffer() const;
	/// get IndexBuffer
	std::shared_ptr<IndexBufferD3D12> GetIndexBuffer() const;
};

	inline std::shared_ptr<VertexBufferD3D12> PrimitiveGroupD3D12::GetVertexBuffer() const
	{
		return std::static_pointer_cast<VertexBufferD3D12>(this->m_vertexBuffer);
	}

	inline std::shared_ptr<IndexBufferD3D12> PrimitiveGroupD3D12::GetIndexBuffer() const
	{
		return std::static_pointer_cast<IndexBufferD3D12>(this->m_indexBuffer);
	}

	inline void PrimitiveGroupD3D12::DiscardVertexBuffer()
	{
		m_vertexBuffer = NULL;
	}

	inline void PrimitiveGroupD3D12::DiscardIndexBuffer()
	{
		m_indexBuffer = NULL;
	}

}



#endif