#pragma once

#include "D3D12Heads.h"
#include "core/types.h"
#include "base/RenderDeviceTypes.h"
#include "base/RenderResource.h"
#include "base/VertexComponent.h"
#include "base/BufferData.h"

#if RENDERDEVICE_D3D12



namespace D3D12
{
	using namespace RenderBase;

	class D3D12Types
{
public:
	static PixelFormat::Code			AsGenesisPixelFormat(DXGI_FORMAT f);
	static DXGI_FORMAT					AsD3D12PixelFormat(PixelFormat::Code p);
	static DXGI_FORMAT					AsD3D12VertexDeclarationType(VertexComponent::Format f);
	static UINT							AsD3D12MultiSampleType(AntiAliasQuality::Code c);
	static LPCSTR						AsD3D12SemanticName(VertexComponent::SemanticName n);
	static D3D12_TEXTURE_ADDRESS_MODE	AsD3D12TextureAddress(TextureAddressMode mode);
	static D3D12_FILTER					AsD3D12TextureFilter(TextureFilter filter);
	static D3D12_PRIMITIVE_TOPOLOGY				AsD3D12PrimitiveType(PrimitiveTopology::Code t);
	static D3D12_BLEND							AsD3D12BlendFactor(BlendFactor factor);
	static D3D12_BLEND_OP						AsD3D12BlendOperation(BlendOperation blendop);
	static D3D12_COMPARISON_FUNC				AsD3D12CompareFunction(CompareFunction func);
	static D3D12_CULL_MODE						AsD3D12CullMode(CullMode mode);
	static D3D12_FILL_MODE						AsD3D12FillMode(FillMode mode);
};

}

#endif