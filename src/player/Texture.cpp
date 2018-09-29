#include "Texture.h"
#include "d3d12/D3D12Types.h"

Texture::Texture()
{

}

Texture::~Texture()
{

}

void Texture::Init(bool allowRT,uint width, uint height, RenderBase::PixelFormat::Code colorFormat)
{
	m_TextureFormat = colorFormat;

	D3D12_RESOURCE_DESC textureDesc = {};
	textureDesc.MipLevels = 1;
	textureDesc.Format = D3D12::D3D12Types::AsD3D12PixelFormat(m_TextureFormat); 
	textureDesc.Width = width;
	textureDesc.Height = height;
	textureDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
	textureDesc.DepthOrArraySize = 1;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;

	D3D12_CLEAR_VALUE* pClearVal = nullptr;

	D3D12_RESOURCE_STATES resrouceState = D3D12_RESOURCE_STATE_COPY_DEST;
	if(allowRT)
	{
		resrouceState = D3D12_RESOURCE_STATE_PRESENT;
		textureDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;

		D3D12_CLEAR_VALUE optClear;
		optClear.Format = textureDesc.Format;
		optClear.Color[0] = 0.5f;
		optClear.Color[1] = 0.5f;
		optClear.Color[2] = 0.5f;
		optClear.Color[3] = 1.0f;
		pClearVal = &optClear;
	}
	ID3D12Device* pDevice = D3D12::RenderDeviceD3D12::Instance()->GetDevice();

	HRESULT hr = pDevice->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&textureDesc,
		resrouceState,
		pClearVal,
		IID_PPV_ARGS(&m_pTextureRes));

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.Format = D3D12::D3D12Types::AsD3D12PixelFormat(m_TextureFormat);
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = 1;
	srvDesc.Texture2D.ResourceMinLODClamp = 0.0f;
	srvDesc.Texture2D.PlaneSlice = 0;

	D3D12::DescriptorHeap* pSrvHeap = D3D12::RenderDeviceD3D12::Instance()->GetCsuHeap();
	D3D12::CPUHandle* m_pSrvHandle = &pSrvHeap->GetCPUHandle();

	pDevice->CreateShaderResourceView(m_pTextureRes, &srvDesc, m_pSrvHandle->handle);
	
// 	if (this->HasDepthStencilBuffer())
// 	{
// 		if (sharedDepthStencilBufferTarget)
// 		{
// 			const std::shared_ptr<RenderTargetD3D12>& pSharedDepthStencilBufferTarget = std::static_pointer_cast<RenderTargetD3D12>(this->sharedDepthStencilBufferTarget);
// 			pInfo->pDepthResource = pSharedDepthStencilBufferTarget->m_pD3D12Rt->pDepthResource;
// 			pInfo->pDepthResource->AddRef();
// 			pInfo->cpuDsv = pSharedDepthStencilBufferTarget->m_pD3D12Rt->cpuDsv;
// 		}
// 		else
// 		{
// 			_CreateDepthStencilView(*pInfo);
// 		}
// 	}
}
