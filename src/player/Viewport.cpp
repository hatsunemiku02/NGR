#include "ViewPort.h"
#include "d3d12/D3D12Types.h"
#include <wrl.h>
using Microsoft::WRL::ComPtr;

ViewPortD3D12::ViewPortD3D12()
{

}

ViewPortD3D12::~ViewPortD3D12()
{

}

void ViewPortD3D12::Init(UINT width, UINT height)
{
	m_ViewPort = CD3DX12_VIEWPORT(0.0f, 0.0f, static_cast<float>(width), static_cast<float>(height));
	m_ScissorRect = CD3DX12_RECT(0, 0, static_cast<LONG>(width), static_cast<LONG>(height));
}
