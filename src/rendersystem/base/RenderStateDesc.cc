

#include "stdneb.h"
#include "RenderStateDesc.h"

namespace RenderBase
{

	RenderStateDesc::RenderStateDesc()
	{

	}

	RenderStateDesc::~RenderStateDesc()
	{

	}

	void RenderStateDesc::Setup()
	{
		SetRenderCommandType(SetRenderState);
		SetUpdateFlag(eAllRenderState);
	}
}