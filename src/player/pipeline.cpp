#include "pipeline.h"
#include "d3d12/DescriptorHeap.h"
#include "d3d12/VertexBufferD3D12.h"

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


void Pipeline::Reset()
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

	const float clearColor[] = { 0.0f, 0.2f, 0.4f, 1.0f };
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
	// Indicate that the back buffer will now be used to present.
	m_CommandList->GetCommandList()->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_pViewPort->GetCurrentRT(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));

	// Execute the command list.
	m_CommandList->ExecuteCommandList();
	// Present the frame.
	HRESULT HR = m_pViewPort->GetSwapChain()->Present(1, 0);
}
