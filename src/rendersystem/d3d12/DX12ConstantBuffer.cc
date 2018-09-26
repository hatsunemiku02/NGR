#include "stdneb.h"
#include "DX12ConstantBuffer.h"
#include "RenderDeviceD3D12.h"


#if RENDERDEVICE_D3D12
namespace D3D12
{

SizeT CalcConstantBufferByteSize(SizeT nSize)
{
	return (nSize + 255) & ~255;
}

DX12ConstantBuffer::DX12ConstantBuffer()
: m_pCBuffer(NULL),
  m_pMappedData(NULL)
{

}

DX12ConstantBuffer::~DX12ConstantBuffer()
{
	Discard();
}

void DX12ConstantBuffer::CreateBuffer(SizeT nSize)
{
	ID3D12Device* pDevice	 = RenderDeviceD3D12::Instance()->GetDevice();
	DescriptorHeap* pCbvHeap = RenderDeviceD3D12::Instance()->GetCsuHeap();

	SizeT packedSize = CalcConstantBufferByteSize(nSize);

	HRESULT hr = pDevice->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(packedSize),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&m_pCBuffer));

	if (FAILED(hr))
	{
		assert(false);
	}

	hr = m_pCBuffer->Map(0, NULL, reinterpret_cast<void**>(&m_pMappedData));
	if (FAILED(hr))
	{
		assert(false);
	}

	m_CpuCbv = pCbvHeap->GetCPUHandle();

	D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc;
	cbvDesc.BufferLocation = m_pCBuffer->GetGPUVirtualAddress();
	cbvDesc.SizeInBytes = packedSize;
	pDevice->CreateConstantBufferView(&cbvDesc, m_CpuCbv.handle);
}

void DX12ConstantBuffer::UpdateBuffer(const RenderBase::DataStream& data)
{
	memcpy((void*)m_pMappedData,data.data , data.sizeInByte);
}

void DX12ConstantBuffer::Discard()
{
	m_pCBuffer->Unmap(0, NULL);
	m_pCBuffer->Release();
	m_pCBuffer = NULL;
	m_pMappedData = NULL;
}

}

#endif