#include "stdneb.h"
#include "CommandQueue.h"
#include "RenderDeviceD3D12.h"
#if RENDERDEVICE_D3D12

namespace D3D12
{
CommandList::CommandList()

	  :m_pFence(NULL),
	  m_FenceValue(0),
	  m_nCmmd(0)
{

}

CommandList::~CommandList()
{

	if (m_pFence != NULL)
	{
		m_pFence->Release();
		m_pFence = NULL;
	}
}

uint CommandList::Signal()
{
	uint fenceValue = ++m_FenceValue;
	RenderDeviceD3D12::Instance()->GetCmdQueue()->Signal(m_pFence, fenceValue);
	return fenceValue;
}

bool CommandList::IsExecutionDone()
{
	return m_pFence->GetCompletedValue() >= m_FenceValue;
}

void CommandList::WaitForExecution()
{
	if (!IsExecutionDone())
	{
		m_pFence->SetEventOnCompletion(m_FenceValue, m_FenceEvent);
		::WaitForSingleObject(m_FenceEvent, 0xffffffffUL);
	}
}

bool CommandList::IsFenceComplete(uint fenceValue)
{
	return m_pFence->GetCompletedValue() >= fenceValue;
}

void CommandList::WaitForFenceValue(uint fenceValue)
{
	if (!IsFenceComplete(fenceValue))
	{
		m_pFence->SetEventOnCompletion(fenceValue, m_FenceEvent);
		::WaitForSingleObject(m_FenceEvent, 0xffffffffUL);
	}
}

void CommandList::Flush()
{
	WaitForFenceValue(Signal());
	ResetCmmdNum();
}

//------------------------------------------------------------------------------

GraphicCommandList::GraphicCommandList()
{
	ID3D12Device* pDevice = RenderDeviceD3D12::Instance()->GetDevice();

	HRESULT hr = pDevice->CreateFence(m_FenceValue, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_pFence));
	if (FAILED(hr))
	{
		assert(false);
	}

	m_FenceEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);

	hr = pDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, RenderDeviceD3D12::Instance()->GetCmdAllocator(), NULL, IID_PPV_ARGS(&m_pCommandList));
	if (FAILED(hr))
	{
		assert(false);
	}
	m_pCommandList->Close();
}

GraphicCommandList::~GraphicCommandList()
{

}

void GraphicCommandList::ResetState()
{
	//

	m_pCommandList->Reset(RenderDeviceD3D12::Instance()->GetCmdAllocator(), NULL);
}

uint GraphicCommandList::ExecuteCommandList()
{
	ID3D12CommandList* const ppCommandLists[] = {
		m_pCommandList
	};
	m_pCommandList->Close();
	RenderDeviceD3D12::Instance()->GetCmdQueue()->ExecuteCommandLists(1, ppCommandLists);
	uint fenceValue = Signal();

	return fenceValue;
}

}


#endif