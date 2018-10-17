#pragma once
#include "d3d12/CmdAllocator.h"

class PiplineThreadGroup
{
public:
	PiplineThreadGroup();
	virtual ~PiplineThreadGroup();


protected:
	CmdAllocator m_CmdAlloctor;

};