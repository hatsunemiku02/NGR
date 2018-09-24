#include "stdneb.h"
#include "RenderDeviceD3D12.h"
#include "VertexBufferD3D12.h"
#include "D3D12Types.h"
#include "CommandQueue.h"

#if RENDERDEVICE_D3D12

namespace D3D12
{

VertexBufferD3D12::VertexBufferD3D12()
: m_pVertexBuffer(nullptr),
  m_pUploadBuffer(nullptr),
  m_pMappedData(nullptr)
{

}

VertexBufferD3D12::~VertexBufferD3D12()
{
	Discard();
}

void VertexBufferD3D12::CreateVertexBuffer(const std::shared_ptr<GraphicCommandList>& pCmdList, const std::shared_ptr<RenderBase::VertexBufferData>& vbd, std::shared_ptr<VertexBufferD3D12>& out)
{
	out = std::make_shared<VertexBufferD3D12>();
	std::shared_ptr<VertexLayoutD3D12> layout = std::make_shared<VertexLayoutD3D12>();
	layout->Setup(vbd->vertex.vertexComponents);
	layout->GenerateDeclarationD3D12();

	SizeT vertexBufferSize = vbd->vertexCount * layout->GetVertexByteSize();

	out->_SetVertexDataSize(vertexBufferSize);
	out->_SetNumVertices(vbd->vertexCount);
	out->_SetUsage(vbd->usage);
	out->_SetD3d12VertexLayout(layout);
	
	uchar* dstPtr = new uchar[vertexBufferSize];
	DirectCopyVertexDataToGraphicBuffer(*vbd, dstPtr, layout->GetVertexByteSize());

	ID3D12Device* pDevice = RenderDeviceD3D12::Instance()->GetDevice();
	ID3D12GraphicsCommandList* pCommandList = pCmdList->GetCommandList();
//	RenderDeviceD3D12::Instance()->GetGraphicQueue()->IncCmmdNum();

	ID3D12Resource* pVertexBuffer = nullptr;
	ID3D12Resource* pUploadBuffer = nullptr;

	if (vbd->usage == RenderBase::BufferData::Static)
	{
		// Create the actual default buffer resource.
		HRESULT hr = pDevice->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer(vertexBufferSize),
			D3D12_RESOURCE_STATE_COMMON,
			nullptr,
			IID_PPV_ARGS(&pVertexBuffer));

		if (FAILED(hr))
		{
			assert(false);
		}

		// In order to copy CPU memory data into our default buffer, we need to create
		// an intermediate upload heap. 
		hr = pDevice->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer(vertexBufferSize),
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&pUploadBuffer));

		if (FAILED(hr))
		{
			assert(false);
		}

		// Describe the data we want to copy into the default buffer.
		D3D12_SUBRESOURCE_DATA subResourceData = {};
		subResourceData.pData = dstPtr;
		subResourceData.RowPitch = vertexBufferSize;
		subResourceData.SlicePitch = subResourceData.RowPitch;

		// Schedule to copy the data to the default buffer resource.  At a high level, the helper function UpdateSubresources
		// will copy the CPU memory into the intermediate upload heap.  Then, using ID3D12CommandList::CopySubresourceRegion,
		// the intermediate upload heap data will be copied to mBuffer.
		pCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(pVertexBuffer, 
			D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_COPY_DEST));
		UpdateSubresources<1>(pCommandList, pVertexBuffer, pUploadBuffer, 0, 0, 1, &subResourceData);
		pCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(pVertexBuffer,
			D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_GENERIC_READ));

		out->_SetD3D12VertexBuffer(pVertexBuffer);
		out->_SetUploadBuffer(pUploadBuffer);
	}
	else if (vbd->usage == RenderBase::BufferData::Dynamic)
	{
		HRESULT hr = pDevice->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer(vertexBufferSize),
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&pVertexBuffer));

		if (FAILED(hr))
		{
			assert(false);
		}

		RenderBase::DataStream ds;
		ds.data = dstPtr;
		ds.sizeInByte = vertexBufferSize;

		out->_SetD3D12VertexBuffer(pVertexBuffer);
		out->UpdateData(ds);
	}

	delete[] dstPtr;
	

	D3D12_VERTEX_BUFFER_VIEW vbv;
	vbv.BufferLocation = pVertexBuffer->GetGPUVirtualAddress();
	vbv.StrideInBytes  = layout->GetVertexByteSize();
	vbv.SizeInBytes	   = vertexBufferSize;
	out->_SetView(vbv);
}

void VertexBufferD3D12::UpdateData(const RenderBase::DataStream& stream)
{
	assert(stream.data);
	assert(vertexDataSize >= stream.sizeInByte);

	if (m_pMappedData == nullptr)
	{
		m_pVertexBuffer->Map(0, nullptr, reinterpret_cast<void**>(&m_pMappedData));
	}

	memcpy(stream.data, m_pMappedData, stream.sizeInByte);
}

void VertexBufferD3D12::Discard()
{
	if (m_pMappedData != nullptr)
	{
		m_pVertexBuffer->Unmap(0, nullptr);
	}

	m_pVertexBuffer->Release();
	m_pVertexBuffer = nullptr;
}

}


#endif