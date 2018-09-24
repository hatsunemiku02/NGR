#pragma once

#include "RenderCommandType.h"
#include "RenderDeviceTypes.h"

namespace RenderBase
{
	class DepthStencilObject : public RenderCommandType
	{
	public:

		DepthStencilObject();

		virtual ~DepthStencilObject();

		virtual void CreateDepthStencil(const DeviceDepthAndStencilState& ds) = 0;

	protected:
	private:
	};
}