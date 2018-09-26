#include "Material.h"

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

void Material::SetConstantBuffers(const std::vector<uint>& sizeList)
{
	m_ConstantBuffers.resize(sizeList.size());
	for (int i=0;i< sizeList.size();i++)
	{
		m_ConstantBuffers[i].CreateBuffer(sizeList[i]);
	}
}

void Material::UpdateConstantBuffer(int index, const RenderBase::DataStream& data)
{
	if (index>= m_ConstantBuffers.size())
	{
		assert(0);
	}
	m_ConstantBuffers[index].UpdateBuffer(data);
}

