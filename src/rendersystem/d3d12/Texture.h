#pragma once
#include "d3d12/RenderDeviceD3D12.h"
#include "d3d12/DescriptorHeap.h"
#include "ResState.h"
class Texture
{

public:
	Texture();
	~Texture();

	void Init(bool allowRT, uint width, uint height, RenderBase::PixelFormat::Code colorFormat);
	ID3D12Resource* GetTextureRes()
	{
		return m_pTextureRes;
	}
	RenderBase::PixelFormat::Code GetTextureFormat()
	{
		return m_textureFormat;
	}
	D3D12::CPUHandle* GetCpuHandle()
	{
		return m_pSrvHandle;
	}
	ResState GetResState()
	{
		return m_textureState;
	}
	void RecordState(ResState newState)
	{
		m_textureState = newState;
	}
private:
	ID3D12Resource* m_pTextureRes;
	D3D12::CPUHandle* m_pSrvHandle;
	ResState m_textureState;
	RenderBase::PixelFormat::Code m_textureFormat;
};

