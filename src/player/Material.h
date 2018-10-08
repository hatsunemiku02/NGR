#pragma once
#include "base/RenderDeviceTypes.h"
#include "d3d12/DX12ConstantBuffer.h"
#include "Texture.h"
#include <vector>
class Material
{
public:
	Material();
	~Material();
	
	void SetShaderCode(void* vs,uint vl,void* ps,uint pl);

	void InitMat(const std::vector<uint>& sizeList, uint textureCount, uint samplerCount);
	void UpdateConstantBuffer(int index,const RenderBase::DataStream& data);

	void SetTexture(uint index, const std::shared_ptr<Texture>& pTexture);
	std::vector<std::shared_ptr<Texture>>& GetTextures();

	RenderBase::PsoData GetPsoData();
	RenderBase::SignatureInfo GetRootSigInfo();
	std::vector<D3D12::DX12ConstantBuffer>& GetConstantBuffers();
protected:
	RenderBase::PsoData m_PsoData;
	RenderBase::SignatureInfo m_SigInfo;
	std::vector<D3D12::DX12ConstantBuffer> m_ConstantBuffers;
	std::vector<std::shared_ptr<Texture>> m_Textures;
};


inline RenderBase::PsoData Material::GetPsoData()
{
	return m_PsoData;
}

inline RenderBase::SignatureInfo Material::GetRootSigInfo()
{
	return m_SigInfo;
}

inline std::vector<D3D12::DX12ConstantBuffer>& Material::GetConstantBuffers()
{
	return m_ConstantBuffers;
}

inline std::vector<std::shared_ptr<Texture>>& Material::GetTextures()
{
	return m_Textures;
}
