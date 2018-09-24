#pragma once
#include "base/Pso.h"
#include "D3D12Heads.h"

#if RENDERDEVICE_D3D12



namespace D3D12
{
	class RootSignature;
	class DX12Pso : public RenderBase::Pso
{

public:

	DX12Pso();

	virtual ~DX12Pso();

	virtual void Init(const RenderBase::PsoData& info, const std::shared_ptr<RenderBase::RootSignature>& pSignature);

	ID3D12PipelineState* GetPso() const;

protected:

	std::vector<D3D12_INPUT_ELEMENT_DESC>	_CreateInput(const RenderBase::PsoData& info);
	void									_DecodeShader(const std::string& srcCode, std::string& shaderCode);
	CD3DX12_RASTERIZER_DESC					_CreateRasterzerDesc(const RenderBase::PsoData& info);
	D3D12_RENDER_TARGET_BLEND_DESC			_CreateBlendDesc(const RenderBase::PsoData& info);
	D3D12_DEPTH_STENCIL_DESC				_CreateDepthStencilDesc(const RenderBase::PsoData& info);

private:

	ID3D12PipelineState*		m_PipelineState;
};

inline ID3D12PipelineState* DX12Pso::GetPso() const
{
	return m_PipelineState;
}

}

#endif