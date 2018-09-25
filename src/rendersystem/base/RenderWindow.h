
#ifndef __RENDERWINDOW_H__
#define __RENDERWINDOW_H__
#ifdef __OSX__
#include "core/types.h"
#else
#include "foundation/core/types.h"
#endif
#include "rendersystem/base/RenderDeviceTypes.h"
namespace RenderBase
{
class RenderWindow 
{

public:
	RenderWindow( WindHandle winHandle );
	virtual ~RenderWindow();

	virtual void		BeginRender() = 0;

	virtual void		EndRender() = 0;

	inline WindHandle			GetHandle() const { return m_WinHandle; }

	inline void SetTargetHandle(const RenderTargetHandle& handle) { m_TargetHandle = handle; }

protected:
	WindHandle	m_WinHandle;
	RenderTargetHandle m_TargetHandle;
};
}
#endif //__RENDERWINDOW_H__
