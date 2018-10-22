#include "CmdAllocator.h"

CmdAllocator::CmdAllocator()
{

}

CmdAllocator::~CmdAllocator()
{

}

void CmdAllocator::Init()
{
	HRESULT	hr = D3D12::RenderDeviceD3D12::Instance()->GetDevice()->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_pCommandAllocator));
	if (FAILED(hr))
	{
		assert(false);
	}
}
