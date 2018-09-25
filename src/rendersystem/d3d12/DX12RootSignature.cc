#include "stdneb.h"
#include "DX12RootSignature.h"
#include "RenderDeviceD3D12.h"
#include "D3D12Types.h"

#if RENDERDEVICE_D3D12

namespace D3D12
{

DX12RootSignature::DX12RootSignature()
: m_pRoot(NULL)

{

}

DX12RootSignature::~DX12RootSignature()
{
	m_pRoot->Release();
	m_pRoot = NULL;
}

void DX12RootSignature::Init(const RenderBase::SignatureInfo& info)
{
	SizeT nSlot = info.cbvInfo.size();

	if (info.srvInfo.size() > 0)
	{
		nSlot++;
	}

	std::vector<CD3DX12_ROOT_PARAMETER> rootParamList;
	rootParamList.resize(nSlot, CD3DX12_ROOT_PARAMETER());

	IndexT cvbParmOffset = 0;

	if (info.srvInfo.size() > 0)
	{
		CD3DX12_DESCRIPTOR_RANGE srvRange;
		srvRange.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, info.srvInfo.size(), 0, 0, 0);

		rootParamList[0].InitAsDescriptorTable(1, &srvRange, D3D12_SHADER_VISIBILITY_PIXEL);
		cvbParmOffset = 1;
	}

	for (IndexT i = 0; i < info.cbvInfo.size(); ++i)
	{
		IndexT iSlot = i + cvbParmOffset;
		rootParamList[iSlot].InitAsConstantBufferView(info.cbvInfo[i]);
		m_SlotMap.insert(  std::make_pair(info.cbvInfo[i], iSlot) );
	}

	std::vector<CD3DX12_STATIC_SAMPLER_DESC> samplerList;
	for (IndexT i = 0; i < info.samplerInfo.size(); ++i)
	{
		const RenderBase::DeviceSamplerState& ds = info.samplerInfo[i];
		CD3DX12_STATIC_SAMPLER_DESC samplerDesc(ds.m_bindpoint,
												D3D12Types::AsD3D12TextureFilter(ds.m_Filter),
												D3D12Types::AsD3D12TextureAddress(ds.m_addressU),
												D3D12Types::AsD3D12TextureAddress(ds.m_addressV) );
			
		samplerList.push_back(samplerDesc);
	}

	// A root signature is an array of root parameters.
	const D3D12_ROOT_PARAMETER* rootparam = nullptr;
	const D3D12_STATIC_SAMPLER_DESC* staticsampler = nullptr;
	if (rootParamList.size()!=0)
	{
		rootparam = &(*rootParamList.cbegin());
	}
	if (samplerList.size() != 0)
	{
		staticsampler = &(*samplerList.cbegin());

	}
	CD3DX12_ROOT_SIGNATURE_DESC rootSigDesc(rootParamList.size(), rootparam,
											(UINT)samplerList.size(), staticsampler,
										    D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	// create a root signature with a single slot which points to a descriptor range consisting of a single constant buffer
	ID3DBlob* serializedRootSig = NULL;
	ID3DBlob* errorBlob = NULL;
	HRESULT hr = D3D12SerializeRootSignature(&rootSigDesc, D3D_ROOT_SIGNATURE_VERSION_1,
											 &serializedRootSig, &errorBlob);

	if (FAILED(hr))
	{
		assert(false);
	}

	ID3D12Device* pDevice = RenderDeviceD3D12::Instance()->GetDevice();

	hr = pDevice->CreateRootSignature(
		0,
		serializedRootSig->GetBufferPointer(),
		serializedRootSig->GetBufferSize(),
		IID_PPV_ARGS(&m_pRoot));

	if (FAILED(hr))
	{
		assert(false);
	}

	serializedRootSig->Release();

	if (errorBlob != NULL)
	{
		char* errorChar = (char*)errorBlob->GetBufferPointer();
		errorBlob->Release();
	}
	
}

}


#endif