#include "Material.h"

const int MAT_RIG_START_IDX = 1;

Material::Material()
{
	// init default blend state
	RenderBase::DeviceBlendState blendState;
	m_PsoData.renderBlendState = blendState;
	RenderBase::DeviceRasterizerState rasterState;
	m_PsoData.renderRasterizerState = rasterState;
	RenderBase::DeviceDepthAndStencilState dsState;
	m_PsoData.renderDepthStencilState = dsState;
	
}

Material::~Material()
{

}

void Material::SetShaderCode(void* vs, uint vl, void* ps, uint pl)
{
	m_PsoData.vsByteCode = vs;
	m_PsoData.vsLength = vl;
	m_PsoData.psByteCode = ps;
	m_PsoData.psLength = pl;
}

void Material::InitMat(const std::vector<uint>& sizeList, uint textureCount,uint samplerCount)
{
	m_ConstantBuffers.resize(sizeList.size());
	for (int i=0;i< sizeList.size();i++)
	{
		m_ConstantBuffers[i].CreateBuffer(sizeList[i]);
	}
	m_SigInfo.cbvInfo.resize(sizeList.size());
	for (int i = 0; i < sizeList.size(); i++)
	{
		m_SigInfo.cbvInfo[i] = i+ MAT_RIG_START_IDX;
	}
	m_Textures.resize(textureCount);
	m_SigInfo.srvInfo.resize(textureCount);
	for (int i = 0; i < m_Textures.size(); i++)
	{
		m_SigInfo.srvInfo[i] = i;
	}

	m_SigInfo.samplerInfo.resize(samplerCount);
	
}

void Material::UpdateConstantBuffer(int index, const RenderBase::DataStream& data)
{
	if (index>= m_ConstantBuffers.size())
	{
		assert(0);
	}
	m_ConstantBuffers[index].UpdateBuffer(data);
}

void Material::SetTexture(uint index, const std::shared_ptr<Texture>& pTexture)
{
	m_Textures[index] = pTexture;
}

