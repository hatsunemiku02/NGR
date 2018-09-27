#include "stdneb.h"
#include "IndexBufferD3D12.h"
#include "RenderDeviceD3D12.h"
#include "D3D12Types.h"


#if RENDERDEVICE_D3D12

namespace D3D12
{


IndexBufferD3D12::IndexBufferD3D12()
: m_pIndexBuffer(nullptr),
  m_pUploadBuffer(nullptr),
  m_pMappedData(nullptr)
{

}

IndexBufferD3D12::~IndexBufferD3D12()
{
	Discard();
}

void IndexBufferD3D12::CreateIndexBuffer(const std::shared_ptr<GraphicCommandList>& pCmdList, const std::shared_ptr<RenderBase::IndexBufferData>& ibd, std::shared_ptr<IndexBufferD3D12>& out)
{
	out = std::make_shared<IndexBufferD3D12>();// IndexBufferD3D12::Create();
	DWORD d3dIndexBufferSize = IndexBufferData::SizeOf(ibd->indexType) * ibd->indexCount;

	out->_SetIndexDataSize(d3dIndexBufferSize);
	out->_SetNumIndices(ibd->indexCount);
	out->_SetIndexType(ibd->indexType);
	out->_SetUsage(ibd->usage);

	ID3D12Device* pDevice = RenderDeviceD3D12::Instance()->GetDevice();
	ID3D12GraphicsCommandList* pCommandList = pCmdList->GetCommandList();

	ID3D12Resource* pIndexBuffer = nullptr;
	ID3D12Resource* pUploadBuffer = nullptr;

	void* dstPtr = ibd->stream;

	if (ibd->usage == RenderBase::BufferData::Static)
	{
		// Create the actual default buffer resource.
		HRESULT hr = pDevice->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer(d3dIndexBufferSize),
			D3D12_RESOURCE_STATE_COMMON,
			nullptr,
			IID_PPV_ARGS(&pIndexBuffer));

		if (FAILED(hr))
		{
			assert(false);
		}

		// In order to copy CPU memory data into our default buffer, we need to create
		// an intermediate upload heap. 
		hr = pDevice->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer(d3dIndexBufferSize),
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
		subResourceData.RowPitch = d3dIndexBufferSize;
		subResourceData.SlicePitch = subResourceData.RowPitch;

		// Schedule to copy the data to the default buffer resource.  At a high level, the helper function UpdateSubresources
		// will copy the CPU memory into the intermediate upload heap.  Then, using ID3D12CommandList::CopySubresourceRegion,
		// the intermediate upload heap data will be copied to mBuffer.
		pCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(pIndexBuffer, 
			D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_COPY_DEST));
		UpdateSubresources<1>(pCommandList, pIndexBuffer, pUploadBuffer, 0, 0, 1, &subResourceData);
		pCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(pIndexBuffer,
			D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_GENERIC_READ));

		out->SetD3D12IndexBuffer(pIndexBuffer);
		out->SetUploadBuffer(pUploadBuffer);
	}
	else if (ibd->usage == RenderBase::BufferData::Dynamic)
	{
		HRESULT hr = pDevice->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer(d3dIndexBufferSize),
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&pIndexBuffer));

		if (FAILED(hr))
		{
			assert(false);
		}

		out->SetD3D12IndexBuffer(pIndexBuffer);
	}	

	D3D12_INDEX_BUFFER_VIEW ibv;
	ibv.BufferLocation = pIndexBuffer->GetGPUVirtualAddress();
	ibv.Format  = ibd->indexType == RenderBase::IndexBufferData::Int16 ? DXGI_FORMAT_R16_UINT : DXGI_FORMAT_R32_UINT;
	ibv.SizeInBytes	   = d3dIndexBufferSize;
	out->SetView(ibv);
}

void IndexBufferD3D12::Discard()
{
	if (m_pMappedData != NULL)
	{
		m_pIndexBuffer->Unmap(0, NULL);
	}

	m_pIndexBuffer->Release();
	m_pIndexBuffer = NULL;
}

void IndexBufferD3D12::UpdateData(const RenderBase::DataStream& stream)
{
	assert(stream.data);
	assert(indexDataSize >= stream.sizeInByte);

	if (m_pMappedData == NULL)
	{
		m_pIndexBuffer->Map(0, NULL, reinterpret_cast<void**>(&m_pMappedData));
	}

	memcpy(stream.data, m_pMappedData, stream.sizeInByte);
}

}



#endif