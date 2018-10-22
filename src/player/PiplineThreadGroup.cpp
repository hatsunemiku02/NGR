#include "PiplineThreadGroup.h"

PiplineThreadGroup::PiplineThreadGroup()
{

}

PiplineThreadGroup::~PiplineThreadGroup()
{

}

void PiplineThreadGroup::Init()
{
	m_pCmdAlloctor = std::make_shared<CmdAllocator>();
	m_pCmdAlloctor->Init();
}
