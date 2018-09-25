

#include "stdneb.h"
#include "MultipleRenderTarget.h"
#include "RenderTarget.h"

namespace RenderBase
{

MultipleRenderTarget::MultipleRenderTarget()
: m_NumRenderTargets(0)
{

}

MultipleRenderTarget::~MultipleRenderTarget()
{

}

void MultipleRenderTarget::AddRenderTarget(const std::shared_ptr<RenderTarget> &rt)
{
	assert(rt);
	assert(m_NumRenderTargets < MaxNumRenderTargets);

	m_RenderTargets[m_NumRenderTargets] = rt;
	m_NumRenderTargets++;
}

}