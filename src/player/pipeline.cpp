#include "pipeline.h"
#include "d3d12/DescriptorHeap.h"
#include "d3d12/VertexBufferD3D12.h"
#include "d3d12/D3D12Types.h"
#include <wrl.h>
using Microsoft::WRL::ComPtr;

Pipeline::Pipeline()
{
	m_CommandList = std::make_unique<D3D12::GraphicCommandList> ();
}

Pipeline::~Pipeline()
{

}

void Pipeline::SetViewPort(const std::shared_ptr<ViewPortD3D12>& pViewPort)
{
	m_pViewPort = pViewPort;
}


void Pipeline::SetRenderTarget(const std::shared_ptr<RenderTarget>& pRenderTarget)
{
	if (pRenderTarget==nullptr)
	{

	}
	else
	{
		m_pRenderTarget = pRenderTarget;
	}
}

void Pipeline::SetRenderToScreen(RenderBase::PixelFormat::Code format, HWND hwnd)
{
	
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
	swapChainDesc.BufferCount = 2;
	swapChainDesc.Width = m_pViewPort->GetWidth();
	swapChainDesc.Height = m_pViewPort->GetHeight();
	swapChainDesc.Format = D3D12::D3D12Types::AsD3D12PixelFormat(format);
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapChainDesc.SampleDesc.Count = 1;

	IDXGIFactory4* factory = D3D12::RenderDeviceD3D12::Instance()->GetDXGI();
	ComPtr<IDXGISwapChain1> swapChain;
	HRESULT hr = factory->CreateSwapChainForHwnd(
		D3D12::RenderDeviceD3D12::Instance()->GetCmdQueue(),		// Swap chain needs the queue so that it can force a flush on it.
		hwnd,
		&swapChainDesc,
		nullptr,
		nullptr,
		&swapChain
	);
	ComPtr<IDXGISwapChain3> swapChain3;
	hr = swapChain.As(&swapChain3);
	m_pDefaultSwapChain = swapChain3.Get();
	// This sample does not support fullscreen transitions.
	hr = factory->MakeWindowAssociation(hwnd, DXGI_MWA_NO_ALT_ENTER);
	D3D12::DescriptorHeap* pRTVHeap = D3D12::RenderDeviceD3D12::Instance()->GetRtvHeap();
	for (IndexT i = 0; i < 2; ++i)
	{
		m_pDefaultRenderTargets.push_back(std::make_shared<RenderTarget>());
		m_pDefaultRenderTargets[i]->Init(m_pDefaultSwapChain, format,i);
	}
}

void Pipeline::Reset()
{
	m_CommandList->WaitForExecution();
	m_CommandList->ResetState();

	m_CommandList->GetCommandList()->RSSetViewports(1, &m_pViewPort->GetViewPort());
	m_CommandList->GetCommandList()->RSSetScissorRects(1, &m_pViewPort->GetRect());


	std::shared_ptr<RenderTarget> currentRT;
	if (m_pRenderTarget==nullptr)
	{
		currentRT = m_pDefaultRenderTargets[m_pDefaultSwapChain->GetCurrentBackBufferIndex()];
	}
	else {
		currentRT = m_pRenderTarget;
	}

	m_CommandList->GetCommandList()
		->ResourceBarrier(1,
			&CD3DX12_RESOURCE_BARRIER::Transition(currentRT->GetRenderTarget(),
				D3D12_RESOURCE_STATE_PRESENT,
				D3D12_RESOURCE_STATE_RENDER_TARGET));

	D3D12::CPUHandle* cpuhandle = currentRT->GetRenderTargetHandke();
	m_CommandList->GetCommandList()->OMSetRenderTargets(1, &cpuhandle->handle, FALSE, nullptr);

	const float clearColor[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	m_CommandList->GetCommandList()->ClearRenderTargetView(cpuhandle->handle, clearColor, 0, nullptr);

}

void Pipeline::SetMaterial(const std::shared_ptr<RenderObj>& obj)
{
	m_CommandList->GetCommandList()->SetGraphicsRootSignature(obj->m_pRootSig->GetRootSignature());
	m_CommandList->GetCommandList()->SetPipelineState(obj->m_pPipeStateObj->GetPso());

	for (int i = 0; i < obj->GetConstantBuffers().size(); i++)
	{
		m_CommandList->GetCommandList()->SetGraphicsRootConstantBufferView(i, obj->GetConstantBuffers()[i].GetBuffer()->GetGPUVirtualAddress());
	}

	for (int n = 0; n < obj->m_pMaterial->GetConstantBuffers().size(); n++)
	{
		m_CommandList->GetCommandList()->SetGraphicsRootConstantBufferView(n + obj->GetConstantBuffers().size(), obj->m_pMaterial->GetConstantBuffers()[n].GetBuffer()->GetGPUVirtualAddress());
	}

	D3D12::DescriptorHeap* pSrvHeap = D3D12::RenderDeviceD3D12::Instance()->GetCsuHeap();
	for (int i = 0; i < obj->m_pMaterial->GetTextures().size(); i++)
	{
		m_CommandList->GetCommandList()->SetGraphicsRootDescriptorTable(i, pSrvHeap->GetGpuHandleFromCpu(*obj->m_pMaterial->GetTextures()[i]->GetCpuHandle()));
	}

}

void Pipeline::SetVertexBuffer(const std::shared_ptr<RenderObj>& obj)
{
	m_CommandList->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_CommandList->GetCommandList()->IASetVertexBuffers(0, 1, &obj->m_pPrimitiveGroup->GetVertexBuffer()->GetView());
}

void Pipeline::RenderOneItem(const std::shared_ptr<RenderObj>& obj)
{
	
	SetMaterial(obj);
	SetVertexBuffer(obj);

	m_CommandList->GetCommandList()->DrawInstanced(3, 1, 0, 0);
}

void Pipeline::Render()
{
	for (auto renderobj : m_RenderList)
	{
		RenderOneItem(renderobj);
	}

	std::shared_ptr<RenderTarget> currentRT;
	if (m_pRenderTarget == nullptr)
	{
		currentRT = m_pDefaultRenderTargets[m_pDefaultSwapChain->GetCurrentBackBufferIndex()];
	}
	else {
		currentRT = m_pRenderTarget;
	}

	// Indicate that the back buffer will now be used to present.
	m_CommandList->GetCommandList()->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(currentRT->GetRenderTarget(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));

	// Execute the command list.
	m_CommandList->ExecuteCommandList();
	// Present the frame.
	if(m_pDefaultSwapChain!=nullptr)
	{
		HRESULT HR = m_pDefaultSwapChain->Present(1, 0);
	}
	
}

Pipeline2MatInfo Pipeline::GenerateMatExternalInfo()
{
	Pipeline2MatInfo ret;
	if (m_pRenderTarget!=nullptr)
	{
		ret.m_RTPixelFormat = m_pRenderTarget->GetRTFormat();
	}
	else
	{
		ret.m_RTPixelFormat = m_pDefaultRenderTargets[0]->GetRTFormat();
	}
	return ret;
}
