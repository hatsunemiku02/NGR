#ifndef MULTIPLERENDERTARGET_H_
#define MULTIPLERENDERTARGET_H_

#include "RenderCommandType.h"
#include "RenderDeviceTypes.h"

namespace RenderBase
{
	class RenderTarget;

	const IndexT MaxNumRenderTargets = 4;

class MultipleRenderTarget : public RenderCommandType
{
public:

	MultipleRenderTarget();

	virtual ~MultipleRenderTarget();

	/// set rendertarget
	void AddRenderTarget(const std::shared_ptr<RenderTarget>& rt);
	/// get rendertarget at index
	const std::shared_ptr<RenderTarget>& GetRenderTarget(IndexT i) const;
	/// get number of rendertargets used
	SizeT GetNumRendertargets() const;

protected:

	std::shared_ptr<RenderTarget>      m_RenderTargets[MaxNumRenderTargets];
	SizeT                  m_NumRenderTargets;
};

inline const std::shared_ptr<RenderTarget>& MultipleRenderTarget::GetRenderTarget(IndexT i) const
{
	assert( i < m_NumRenderTargets );
	return m_RenderTargets[i];
}

inline SizeT MultipleRenderTarget::GetNumRendertargets() const
{
	return m_NumRenderTargets;
}

}




#endif