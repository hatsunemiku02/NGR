#pragma once
#pragma once
#include <memory>
#include "RenderDeviceD3D12.h"
#include "core/types.h"
#include "../config/RenderDeviceConfig.h"
#include "D3D12Heads.h"

class CmdAllocator
{
public:
	CmdAllocator();
	virtual ~CmdAllocator();

	void Init(D3D12::RenderDeviceD3D12* pRenderDevice);

	ID3D12CommandAllocator* GetCmdAllocator();
protected:
	ID3D12CommandAllocator * m_pCommandAllocator;
private:

};


inline ID3D12CommandAllocator* CmdAllocator::GetCmdAllocator()
{
	return m_pCommandAllocator;
}
