#pragma once
#include "base/RenderDeviceTypes.h"
#include "d3d12/DX12ConstantBuffer.h"
#include <vector>
class Material
{
public:
	Material();
	~Material();
	
	void SetShaderCode(void* vs,uint vl,void* ps,uint pl);

	void SetConstantBuffers(const std::vector<uint>& sizeList);
	void UpdateConstantBuffer(int index,const RenderBase::DataStream& data);

	RenderBase::PsoData GetPsoData();
	RenderBase::SignatureInfo GetRootSigInfo();
	std::vector<D3D12::DX12ConstantBuffer>& GetConstantBuffers();
protected:
	RenderBase::PsoData m_PsoData;
	RenderBase::SignatureInfo m_SigInfo;
	std::vector<D3D12::DX12ConstantBuffer> m_ConstantBuffers;
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
