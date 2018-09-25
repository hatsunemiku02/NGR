#include "stdneb.h"
#include "DX12Pso.h"
#include "DX12RootSignature.h"
#include "D3D12Types.h"
#include "RenderDeviceD3D12.h"
#include "d3d12/VertexLayoutD3D12.h"
#include <string>

#if RENDERDEVICE_D3D12



namespace D3D12
{

DX12Pso::DX12Pso()
: m_PipelineState(NULL)
{

}

DX12Pso::~DX12Pso()
{
	m_PipelineState->Release();
	m_PipelineState = NULL;
}

void DX12Pso::Init(const RenderBase::PsoData& info, const std::shared_ptr<RenderBase::RootSignature>& pSignature)
{
	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
	
	std::shared_ptr<D3D12::VertexLayoutD3D12> dx12layout = std::static_pointer_cast<D3D12::VertexLayoutD3D12>(info.pVertexLayout);
	std::vector<D3D12_INPUT_ELEMENT_DESC>& inputList = dx12layout->GetHandle();

	psoDesc.InputLayout.NumElements = inputList.size();
	psoDesc.InputLayout.pInputElementDescs = &(*inputList.cbegin());

	const std::shared_ptr<DX12RootSignature>& pRoot = std::static_pointer_cast<DX12RootSignature>(pSignature);
	psoDesc.pRootSignature = pRoot->GetRootSignature();

// 	std::string vs, ps;
// 	_DecodeShader(info.vsCode, vs);
// 	_DecodeShader(info.psCode, ps);
// 
// 	psoDesc.VS.pShaderBytecode = vs.c_str();
// 	psoDesc.VS.BytecodeLength  = vs.length();
// 	psoDesc.PS.pShaderBytecode = ps.c_str();
// 	psoDesc.PS.BytecodeLength  = ps.length();

	psoDesc.VS.pShaderBytecode = info.vsByteCode;
	psoDesc.VS.BytecodeLength = info.vsLength;
	psoDesc.PS.pShaderBytecode = info.psByteCode;
	psoDesc.PS.BytecodeLength = info.psLength;



	if (info.renderBlendState.m_alphaBlendEnable[0])
	{
		psoDesc.BlendState.RenderTarget[0] = _CreateBlendDesc(info);
	}
	else
	{
		psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	}

	//psoDesc.RasterizerState = _CreateRasterzerDesc(info);
	psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	psoDesc.DepthStencilState = _CreateDepthStencilDesc(info);
	psoDesc.SampleMask = UINT_MAX;
	psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	psoDesc.NumRenderTargets = 1;
	psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;// D3D12Types::AsD3D12PixelFormat(info.rtvFormat);
	psoDesc.SampleDesc.Count = 1;// RenderDeviceD3D12::Instance()->GetAAType();
	//psoDesc.SampleDesc.Quality = RenderDeviceD3D12::Instance()->GetAAQuality();
	//psoDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	//psoDesc.NodeMask = 0;
	psoDesc.IBStripCutValue = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED;
	//psoDesc.InputLayout = {(,)}

	ID3D12Device* pDevice = RenderDeviceD3D12::Instance()->GetDevice();
	HRESULT hr = pDevice->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&m_PipelineState));
	if (FAILED(hr))
	{
		assert(false);
	}
}

std::vector<D3D12_INPUT_ELEMENT_DESC> DX12Pso::_CreateInput(const RenderBase::PsoData& info)
{
	std::vector<D3D12_INPUT_ELEMENT_DESC> inputDescCollection;
	
	for (IndexT i = 0; i < info.vertexLayout.size(); ++i)
	{
		const RenderBase::VertexLayoutDesc& vd = info.vertexLayout[i];

		RenderBase::VertexComponent::Format format;
		if (vd.name == "POSITION" || vd.name == "NORMAL")
		{
			format = RenderBase::VertexComponent::Float3;
		}
		else if (vd.name == "TEXCOORD")
		{
			format = RenderBase::VertexComponent::Float2;
		}
		else if (vd.name == "TANGENT")
		{
			format = RenderBase::VertexComponent::Float4;
		}
		else if (vd.name == "BLENDINDICES")
		{
			format = RenderBase::VertexComponent::Short4;
		}
		else if (vd.name == "BLENDWEIGHT")
		{
			format = RenderBase::VertexComponent::Float4;
		}
		else if (vd.name == "COLOR")
		{
			format = RenderBase::VertexComponent::ColorBGRA;
		}
		else
		{
			assert(false);
		}

		D3D12_INPUT_ELEMENT_DESC desc;

		desc.SemanticName         = vd.name.c_str();
		desc.SemanticIndex        = (UINT)vd.si;
		desc.Format               = D3D12Types::AsD3D12VertexDeclarationType(format);
		desc.InputSlot			  = 0;
		desc.AlignedByteOffset	  = vd.offset;
		desc.InputSlotClass       = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
		desc.InstanceDataStepRate = 0;

		inputDescCollection.push_back(desc);
	}

	return inputDescCollection;
}

CD3DX12_RASTERIZER_DESC DX12Pso::_CreateRasterzerDesc(const RenderBase::PsoData& info)
{
	CD3DX12_RASTERIZER_DESC desc;
	desc.CullMode = D3D12Types::AsD3D12CullMode(info.renderRasterizerState.m_cullMode);
	desc.FillMode = D3D12Types::AsD3D12FillMode(info.renderRasterizerState.m_fillMode);
	desc.DepthBias = 0;
	desc.DepthBiasClamp = 0.0f;
	desc.DepthClipEnable = true;
	desc.FrontCounterClockwise = true;
	desc.MultisampleEnable = true;
	desc.SlopeScaledDepthBias = 0.0f;
	desc.ForcedSampleCount = 0;
	desc.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;
	desc.AntialiasedLineEnable = TRUE;
	return desc;
}

D3D12_RENDER_TARGET_BLEND_DESC DX12Pso::_CreateBlendDesc(const RenderBase::PsoData& info)
{
	D3D12_RENDER_TARGET_BLEND_DESC desc;
	desc.BlendEnable = true;
	desc.LogicOpEnable = false;
	desc.SrcBlend = D3D12Types::AsD3D12BlendFactor(info.renderBlendState.m_srcBlend[0]);
	desc.DestBlend = D3D12Types::AsD3D12BlendFactor(info.renderBlendState.m_destBlend[0]);
	desc.BlendOp = D3D12Types::AsD3D12BlendOperation(info.renderBlendState.m_blendOP[0]);
	desc.SrcBlendAlpha = D3D12_BLEND_ONE;
	desc.DestBlendAlpha = D3D12_BLEND_ZERO;
	desc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	desc.LogicOp = D3D12_LOGIC_OP_NOOP;
	desc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

	return desc;
}

D3D12_DEPTH_STENCIL_DESC DX12Pso::_CreateDepthStencilDesc(const RenderBase::PsoData& info)
{
	D3D12_DEPTH_STENCIL_DESC desc;
	desc.DepthEnable = info.renderDepthStencilState.m_depthEnable;
	desc.DepthWriteMask = info.renderDepthStencilState.m_depthWriteMask ? D3D12_DEPTH_WRITE_MASK_ALL : D3D12_DEPTH_WRITE_MASK_ZERO;
	desc.DepthFunc = D3D12Types::AsD3D12CompareFunction(info.renderDepthStencilState.m_zFunc);
	desc.StencilEnable = false;
	desc.StencilReadMask	= D3D12_DEFAULT_STENCIL_READ_MASK;
	desc.StencilWriteMask	= D3D12_DEFAULT_STENCIL_WRITE_MASK;

	desc.FrontFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
	desc.FrontFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
	desc.FrontFace.StencilPassOp = D3D12_STENCIL_OP_REPLACE;
	desc.FrontFace.StencilFunc = D3D12_COMPARISON_FUNC_ALWAYS;
	desc.BackFace = desc.FrontFace;

	return desc;
}

void DX12Pso::_DecodeShader(const std::string& srcCode, std::string& shaderCode)
{
	const char* pbuf = srcCode.c_str();
	SizeT len   = srcCode.length();
	std::string str;
	str.reserve(len/2);
	for (IndexT i = 0; i < len;)
	{
		char c1 = pbuf[i] - 'a';
		char c2 = pbuf[i+1] - 'a';
		char src = 16 * c1 + c2;
		str.push_back(src);
		i += 2;
	}
	shaderCode= std::move(str);
}

}

#endif