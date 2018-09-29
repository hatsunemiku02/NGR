#include "stdneb.h"
#include "TextureD3D12.h"
#include "D3D12Types.h"
#include "RenderDeviceD3D12.h"
#include <vector>
#include <algorithm>

#if RENDERDEVICE_D3D12

namespace D3D12
{

void MoveCursorToNextMipMap (PixelFormat::Code format, ubyte*& srcData, SizeT& texWidth, SizeT& texHeight, SizeT texDepth = 1 )
{
	srcData += PixelFormat::GetMemorySize(texWidth, texHeight, texDepth, format);
	texWidth =  std::max<size_t>( texWidth / 2, 1 );
	texHeight = std::max<size_t>( texHeight / 2, 1 );
}

void InitSubResource(std::vector<D3D12_SUBRESOURCE_DATA>& subList, SizeT texMipMap, SizeT texWidth, SizeT texHeight, ubyte* srcData, PixelFormat::Code format)
{
	for (IndexT level = 0; level < texMipMap; ++level)
	{
		SizeT srcRowPitch = PixelFormat::GetMemorySize(texWidth, 1, 1, format);
		D3D12_SUBRESOURCE_DATA sd;
		sd.pData = srcData;
		sd.RowPitch = srcRowPitch;
		sd.SlicePitch = 0;
		subList.push_back(sd);
		MoveCursorToNextMipMap(format, srcData, texWidth, texHeight );
	}

}

TextureD3D12::TextureD3D12()
	: m_pFromRt(NULL)
{

}

TextureD3D12::~TextureD3D12()
{
	Unload();
}

void TextureD3D12::Unload()
{
	m_pD3d12Texture.Unload();
}

void TextureD3D12::Setup(const std::shared_ptr<Texture>& srcTex)
{
	Texture::Setup();
	/// set texture resouce path
	SetTexturePath(srcTex->GetTexturePath());
	/// set texture stream
	//SetStream(srcTex->GetStream());
	/// set texture unit index
	SetUnitIndex(srcTex->GetUnitIndex());
	/// set attri
	SetType( srcTex->GetType() );
	SetWidth( srcTex->GetWidth() );
	SetHeight( srcTex->GetHeight() );
	SetDepth( srcTex->GetDepth() );
	SetNumMipLevels( srcTex->GetNumMipLevels() );
	SetPixelFormat( srcTex->GetPixelFormat() );
	SetSkippedMips( srcTex->GetSkippedMips() );

	SetUsage(srcTex->GetUsage());		//[2012/4/13 zhongdaohuan]
	SetAccess(srcTex->GetAccess());		//[2012/4/13 zhongdaohuan]
}

bool TextureD3D12::LoadFileFromBuffers()
{
// 	std::shared_ptr<IO::Stream> stream = m_stream;
// 
// 	stream->SetAccessMode(IO::Stream::ReadAccess);
// 	if (stream->Open())
// 	{
// 		void* srcData = stream->Map();
// 		UINT srcDataSize = stream->GetSize();
// 
// 		
// 	}

	return true;
}

void TextureD3D12::Init(SizeT width, SizeT height,RenderBase::PixelFormat::Code colorFormat)
{
	D3D12_RESOURCE_DESC texDesc;
	ZeroMemory(&texDesc, sizeof(D3D12_RESOURCE_DESC));
	texDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	texDesc.Alignment = 0;
	texDesc.Width = this->width;
	texDesc.Height = this->height;
	texDesc.DepthOrArraySize = 1;
	texDesc.MipLevels = 1;
	texDesc.Format = D3D12Types::AsD3D12PixelFormat(colorFormat);
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	//texDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;

	ID3D12Device* pDevice = RenderDeviceD3D12::Instance()->GetDevice();
	//ID3D12GraphicsCommandList* pCommandList = pCmdList->GetCommandList();

// 	HRESULT hr = pDevice->CreateCommittedResource(
// 		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
// 		D3D12_HEAP_FLAG_NONE,
// 		&texDesc,
// 		D3D12_RESOURCE_STATE_RENDER_TARGET,
// 		NULL,
// 		IID_PPV_ARGS(&pInfo->pResource));
}



bool TextureD3D12::LoadBuffers(const std::shared_ptr<GraphicCommandList>& pCmdList,ubyte* srcData,uint srcDataSize)
{
	DXGI_FORMAT format = D3D12Types::AsD3D12PixelFormat( GetPixelFormat() );

	if (format == DXGI_FORMAT_UNKNOWN || GetType() == InvalidType)
	{
		_LoadNormErrorBuffer();
		return false;
	}
	bool bCreateOK = false;
	if (srcData!=nullptr)
	{

		if (GetType() == Texture2D)
		{
		

		//CreateOK = _LoadNormBuffer(srcData, srcDataSize);

		}
		else if (GetType() == TextureCube)
		{

		}
		else
		{
			//error("TextureD3D12::LoadBuffers: Invalid tex type");

			bCreateOK = false;
		}

	}

	if (!bCreateOK)
	{
		_LoadNormErrorBuffer();
	}

	return bCreateOK;
	return false;
}

void BlitCopyImage( ubyte* dest, SizeT destRowPitch, SizeT destHeight,  
	ubyte* src, SizeT srcRowPitch, SizeT srcHeight)
{

	for ( IndexT hIndex = 0; hIndex < destHeight && hIndex < srcHeight; ++ hIndex )
	{
		SizeT copyRow = Math::n_min(destRowPitch,  srcRowPitch);
		memcpy(src, dest, copyRow );
		dest += destRowPitch;
		src += srcRowPitch;
	}
}

bool TextureD3D12::_LoadNormBuffer(const std::shared_ptr<GraphicCommandList>& pCmdList, ubyte* srcData, SizeT size )
{
	if( GetDepth() != 1 || GetWidth() <= 0 || GetHeight() <= 0 )
	{
		return false;
	}

	if( GetPixelFormat() == PixelFormat::InvalidPixelFormat )
	{
		return false;
	}

	SizeT texWidth = GetWidth();
	SizeT texHeight = GetHeight();
	SizeT texMipMap = GetNumMipLevels();	//	in d3d , this is count.

	PixelFormat::Code format = GetPixelFormat();

	ID3D12Device* pDevice = RenderDeviceD3D12::Instance()->GetDevice();
	DescriptorHeap* pHeap = RenderDeviceD3D12::Instance()->GetCsuHeap();

	D3D12_RESOURCE_DESC texDesc;
	ZeroMemory(&texDesc, sizeof(D3D12_RESOURCE_DESC));
	texDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	texDesc.Alignment = 0;
	texDesc.Width = texWidth;
	texDesc.Height = texHeight;
	texDesc.DepthOrArraySize = 1;
	texDesc.MipLevels = texMipMap;
	texDesc.Format = D3D12Types::AsD3D12PixelFormat(format);
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	texDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

	HRESULT hr = pDevice->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&texDesc,
		D3D12_RESOURCE_STATE_COMMON,
		NULL,
		IID_PPV_ARGS(&m_pD3d12Texture.pResource)
		);

	if (FAILED(hr))
	{
		assert(false);
	}

	const UINT num2DSubresources = texDesc.DepthOrArraySize * texDesc.MipLevels;
	const UINT64 uploadBufferSize = GetRequiredIntermediateSize(m_pD3d12Texture.pResource, 0, num2DSubresources);

	hr = pDevice->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(uploadBufferSize),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&m_pUploadResource));

	if (FAILED(hr))
	{
		assert(false);
	}

	std::vector<D3D12_SUBRESOURCE_DATA> initData;
	InitSubResource(initData, texMipMap, texWidth, texHeight, srcData, format);

	ID3D12GraphicsCommandList* pCommandList = pCmdList->GetCommandList();
	pCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_pD3d12Texture.pResource,
		D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_COPY_DEST));

	// Use Heap-allocating UpdateSubresources implementation for variable number of subresources (which is the case for textures).
	UpdateSubresources(pCommandList, m_pD3d12Texture.pResource, m_pUploadResource, 0, 0, num2DSubresources, &initData[0]);

	pCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_pD3d12Texture.pResource,
		D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE));

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.ResourceMinLODClamp = 0.0f;
	srvDesc.Format = D3D12Types::AsD3D12PixelFormat(format);
	srvDesc.Texture2D.MipLevels = texMipMap;

	DescriptorHeap* pSrvHeap = RenderDeviceD3D12::Instance()->GetCsuHeap();
	m_pD3d12Texture.cpusrv = pSrvHeap->GetCPUHandle();
	pDevice->CreateShaderResourceView(m_pD3d12Texture.pResource, &srvDesc, m_pD3d12Texture.cpusrv.handle);
	m_pD3d12Texture.gpusrv = pSrvHeap->GetGpuHandleFromCpu(m_pD3d12Texture.cpusrv);

	return true;
}

void TextureD3D12::_LoadNormErrorBuffer()
{

}

void TextureD3D12::UpdateTexture(std::shared_ptr<Texture> srcTex)
{

}

void TextureD3D12::UpdateTexture(UpdateFunction texUpdateFunc, void* tag)
{

}

bool TextureD3D12::Map(IndexT mipLevel, MapType mapType, MapInfo& outMapInfo)
{
	return false;
}

void TextureD3D12::Unmap(IndexT mipLevel)
{

}

bool TextureD3D12::MapCubeFace(CubeFace face, IndexT mipLevel, MapType mapType, MapInfo& outMapInfo)
{
	return false;
}

void TextureD3D12::UnmapCubeFace(CubeFace face, IndexT mipLevel)
{

}

}


#endif