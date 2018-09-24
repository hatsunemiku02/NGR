#pragma once

#include "core/types.h"
#include "../config/RenderDeviceConfig.h"


#if RENDERDEVICE_D3D12
#include "D3D12Heads.h"

namespace D3D12
{
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

	virtual ~GraphicCommandList();

	void ResetState();

	virtual uint ExecuteCommandList();

	ID3D12GraphicsCommandList*	GetCommandList() const;

protected:
private:

	ID3D12GraphicsCommandList*		m_pCommandList;
};

inline ID3D12GraphicsCommandList* GraphicCommandList::GetCommandList() const
{
	return m_pCommandList;
}

}

#endif