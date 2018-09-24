#pragma once

#include "rendersystem/base/Texture.h"
#include "D3D12Heads.h"
#include "DescriptorHeap.h"

#if RENDERDEVICE_D3D12

namespace D3D12
{
	using namespace RenderBase;
	class RenderTargetD3D12;
	class TextureD3D12 : public Texture
{

public:

	TextureD3D12();

	virtual ~TextureD3D12();

	struct TexResView
	{
		CPUHandle cpusrv;
		CD3DX12_GPU_DESCRIPTOR_HANDLE	gpusrv;
		ID3D12Resource*	pResource;

		void Unload()
		{
			if (pResource != NULL)
			{
				pResource->Release();
				pResource = NULL;
			}
		}
	};

	/// unload the resource, or cancel the pending load
	void Unload();
	/// map a texture mip level for CPU access
	bool Map(IndexT mipLevel, MapType mapType, MapInfo& outMapInfo);
	/// unmap texture after CPU access
	void Unmap(IndexT mipLevel);
	/// map a cube map face for CPU access
	bool MapCubeFace(CubeFace face, IndexT mipLevel, MapType mapType, MapInfo& outMapInfo);
	/// unmap cube map face after CPU access
	void UnmapCubeFace(CubeFace face, IndexT mipLevel);

	
	/// load Buffers;
	bool LoadBuffers();
	bool LoadFileFromBuffers();
	bool _LoadNormBuffer(ubyte* srcData, SizeT size );
	void _LoadNormErrorBuffer();
	bool _LoadCubeBuffer(ubyte* srcData, SizeT size);
	void _LoadCubeErrorBuffer();

	void UpdateTexture(UpdateFunction texUpdateFunc, void* tag);
	void UpdateTexture(std::shared_ptr<Texture> srcTex);
	void Setup(const std::shared_ptr<Texture>& srcTex);

	void SetupFromD3D12Texture(ID3D12Resource* srcTex, const CD3DX12_GPU_DESCRIPTOR_HANDLE&	gpusrv);

	const CD3DX12_GPU_DESCRIPTOR_HANDLE& GetGpuHandle() const;

	void SetRendertargetRes(bool bRes);
	bool IsRendertargetRes() const;

	ID3D12Resource*		GetResource() const;
	void	SetFromRt(RenderTargetD3D12* rt);
	RenderTargetD3D12* GetFromRt() const;

protected:

	TexResView			m_pD3d12Texture;
	ID3D12Resource*		m_pUploadResource;
	bool				m_bRendertargetRes;
	RenderTargetD3D12*	m_pFromRt;
};

inline void TextureD3D12::SetupFromD3D12Texture(ID3D12Resource* srcTex, const CD3DX12_GPU_DESCRIPTOR_HANDLE& gpusrv)
{
	m_pD3d12Texture.pResource = srcTex;
	srcTex->AddRef();
	m_pD3d12Texture.gpusrv = gpusrv;
}

inline const CD3DX12_GPU_DESCRIPTOR_HANDLE& TextureD3D12::GetGpuHandle() const
{
	return m_pD3d12Texture.gpusrv;
}

inline void TextureD3D12::SetRendertargetRes(bool bRes)
{
	m_bRendertargetRes = bRes;
}

inline bool TextureD3D12::IsRendertargetRes() const
{
	return m_bRendertargetRes;
}

inline ID3D12Resource* TextureD3D12::GetResource() const
{
	return m_pD3d12Texture.pResource;
}

inline void TextureD3D12::SetFromRt(RenderTargetD3D12* rt)
{
	m_pFromRt = rt;
}

inline RenderTargetD3D12* TextureD3D12::GetFromRt() const
{
	return m_pFromRt;
}

}

#endif