#pragma once
#include <memory>
#include "core/types.h"
#include "../config/RenderDeviceConfig.h"
#include "CmdAllocator.h"

#if RENDERDEVICE_D3D12
#include "D3D12Heads.h"

namespace D3D12
{

	class PrimitiveGroupD3D12;

class CommandList
{

public:
	CommandList();

	~CommandList();

	virtual uint ExecuteCommandList() = 0;

	unsigned int Signal();
	bool IsExecutionDone();
	void WaitForExecution();

	bool IsFenceComplete(unsigned int fenceValue);
	void WaitForFenceValue(unsigned int fenceValue);
	void Flush();
	void IncCmmdNum();
	bool IsEmpty() const;
	void ResetCmmdNum();

protected:

	ID3D12Fence*				m_pFence;
	unsigned int				m_FenceValue;
	HANDLE                      m_FenceEvent;
	unsigned int				m_nCmmd;

};



inline void CommandList::IncCmmdNum()
{
	m_nCmmd++;
}

inline void CommandList::ResetCmmdNum()
{
	m_nCmmd = 0;
}

inline bool CommandList::IsEmpty() const
{
	return m_nCmmd == 0;
}


class GraphicCommandList : public CommandList
{
public:

	GraphicCommandList();
	GraphicCommandList(const std::shared_ptr<CmdAllocator>& allocator);
	
	virtual ~GraphicCommandList();

	void ResetState();

	virtual uint ExecuteCommandList();

	ID3D12GraphicsCommandList*	GetCommandList() const;

	void SetPrimitiveGroup(const std::shared_ptr<PrimitiveGroupD3D12>& pPrimitiveGroup);
	void DrawInstanced( uint VertexCountPerInstance, uint InstanceCount, uint StartVertexLocation, uint StartInstanceLocation);
protected:
private:

	ID3D12GraphicsCommandList*		m_pCommandList;
	std::shared_ptr<CmdAllocator>   m_pCmdAllocator;
};

inline ID3D12GraphicsCommandList* GraphicCommandList::GetCommandList() const
{
	return m_pCommandList;
}

}

#endif