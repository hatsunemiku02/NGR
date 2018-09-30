#pragma once
#include <d3d12.h>

enum class ResState:uint
{
	ShaderRes,
	RenderTarget,
	Present,

};

inline D3D12_RESOURCE_STATES ToDX12State(ResState state)
{
	switch (state)
	{
	case ResState::ShaderRes:
		return D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
		break;
	case ResState::RenderTarget:
		return D3D12_RESOURCE_STATE_RENDER_TARGET;
		break;
	case ResState::Present:
		return D3D12_RESOURCE_STATE_PRESENT;
		break;
	default:
		break;
	}
}