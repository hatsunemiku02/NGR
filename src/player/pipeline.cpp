#include "pipeline.h"
#include "d3d12/DescriptorHeap.h"

pipeline::pipeline()
{
	m_CommandList = std::make_unique<D3D12::GraphicCommandList> ();
}

pipeline::~pipeline()
{

}

void pipeline::SetViewPort(const std::shared_ptr<ViewPortD3D12>& pViewPort)
{
	m_pViewPort = pViewPort;
}


void pipeline::Reset()
{
	m_CommandList->WaitForExecution();
	m_CommandList->ResetState();

	m_CommandList->GetCommandList()->RSSetViewports(1, &m_pViewPort->GetViewPort());
	m_CommandList->GetCommandList()->RSSetScissorRects(1, &m_pViewPort->GetRect());

	m_CommandList->GetCommandList()
		->ResourceBarrier(1,
			&CD3DX12_RESOURCE_BARRIER::Transition(m_pViewPort->GetCurrentRT(),
			D3D12_RESOURCE_STATE_PRESENT, 
			D3D12_RESOURCE_STATE_RENDER_TARGET));

	D3D12::DescriptorHeap* pRTVHeap = D3D12::RenderDeviceD3D12::Instance()->GetRtvHeap();

	D3D12::CPUHandle* cpuhandle =  m_pViewPort->GetCurrentRTHandle();

	m_CommandList->GetCommandList()->OMSetRenderTargets(1, &cpuhandle->handle, FALSE, nullptr);

	const float clearColor[] = { 1.0f, 0.2f, 0.4f, 1.0f };
	m_CommandList->GetCommandList()->ClearRenderTargetView(cpuhandle->handle, clearColor, 0, nullptr);

}

void pipeline::Render()
{
	// Indicate that the back buffer will now be used to present.
	m_CommandList->GetCommandList()->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_pViewPort->GetCurrentRT(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));

	

	// Execute the command list.
	m_CommandList->ExecuteCommandList();
	// Present the frame.
	HRESULT HR = m_pViewPort->GetSwapChain()->Present(1, 0);
}
