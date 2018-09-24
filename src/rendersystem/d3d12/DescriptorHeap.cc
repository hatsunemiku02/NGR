#include "stdneb.h"
#include "../config/RenderDeviceConfig.h"
#if RENDERDEVICE_D3D12
#include "DescriptorHeap.h"
#include "RenderDeviceD3D12.h"

namespace D3D12
{

DescriptorHeap::DescriptorHeap()
: m_pDescriptorHeap(NULL),
  m_nDescriptorSize(0)
{

}

DescriptorHeap::~DescriptorHeap()
{
	if (m_pDescriptorHeap != NULL)
	{
		m_pDescriptorHeap->Release();
		m_pDescriptorHeap = NULL;
	}
}

void DescriptorHeap::Init(uint nCount)
{
	m_CPUHandleList.resize(nCount, CPUHandle());
}

CPUHandle& DescriptorHeap::GetCPUHandle()
{
	for (IndexT i = 0; i < m_CPUHandleList.size(); ++i)
	{
		CPUHandle& info = m_CPUHandleList[i];
		if (info.bValid)
		{
			info.bValid = false;
			info.handle.InitOffsetted(m_pDescriptorHeap->GetCPUDescriptorHandleForHeapStart(),
									  i, m_nDescriptorSize);
			info.ind = i;
			return info;
		}
	}
	assert(false);
}

CD3DX12_GPU_DESCRIPTOR_HANDLE DescriptorHeap::GetGpuHandleFromCpu(const CPUHandle& handle)
{
	return CD3DX12_GPU_DESCRIPTOR_HANDLE(m_pDescriptorHeap->GetGPUDescriptorHandleForHeapStart(), handle.ind, m_nDescriptorSize);
}

//------------------------------------------------------------------------------

RTVHeap::RTVHeap()
{

}

RTVHeap::~RTVHeap()
{

}

void RTVHeap::Init(uint nCount)
{
	DescriptorHeap::Init(nCount);

	D3D12_DESCRIPTOR_HEAP_DESC desc;
	desc.NumDescriptors = nCount;
	desc.Type			= D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	desc.Flags			= D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	desc.NodeMask		= 0;

	ID3D12Device* pDevice = RenderDeviceD3D12::Instance()->GetDevice();
	m_nDescriptorSize = pDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	HRESULT hr = pDevice->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&m_pDescriptorHeap));
	if (FAILED(hr))
	{
		assert(false);
	}
}
//------------------------------------------------------------------------------

DSVHeap::DSVHeap()
{

}

DSVHeap::~DSVHeap()
{

}

void DSVHeap::Init(uint nCount)
{
	DescriptorHeap::Init(nCount);

	D3D12_DESCRIPTOR_HEAP_DESC desc;
	desc.NumDescriptors = nCount;
	desc.Type			= D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	desc.Flags			= D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	desc.NodeMask		= 0;

	ID3D12Device* pDevice = RenderDeviceD3D12::Instance()->GetDevice();
	m_nDescriptorSize = pDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
	HRESULT hr = pDevice->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&m_pDescriptorHeap));
	if (FAILED(hr))
	{
		assert(false);
	}

}
//------------------------------------------------------------------------------

CSUHeap::CSUHeap()
{

}

CSUHeap::~CSUHeap()
{

}

void CSUHeap::Init(uint nCount)
{
	DescriptorHeap::Init(nCount);

	D3D12_DESCRIPTOR_HEAP_DESC desc;
	desc.NumDescriptors = nCount;
	desc.Type			= D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	desc.Flags			= D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	desc.NodeMask		= 0;

	ID3D12Device* pDevice = RenderDeviceD3D12::Instance()->GetDevice();
	m_nDescriptorSize = pDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	HRESULT hr = pDevice->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&m_pDescriptorHeap));
	if (FAILED(hr))
	{
		assert(false);
	}
}

}

#endif


