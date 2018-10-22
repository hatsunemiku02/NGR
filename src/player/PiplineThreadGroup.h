#pragma once
#include "d3d12/CmdAllocator.h"

class PiplineThreadGroup
{
public:
	PiplineThreadGroup();
	virtual ~PiplineThreadGroup();
	void Init();
	const std::shared_ptr<CmdAllocator>& GetCMDAllocator() const;
protected:
	std::shared_ptr<CmdAllocator> m_pCmdAlloctor;

};


inline const std::shared_ptr<CmdAllocator>& PiplineThreadGroup::GetCMDAllocator() const
{
	return m_pCmdAlloctor;
}
