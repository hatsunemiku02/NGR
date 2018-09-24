#pragma once

#include "RenderCommandType.h"
#include "RenderDeviceTypes.h"

namespace RenderBase
{
	class RasterizerObject : public RenderCommandType
	{
	public:

		RasterizerObject();

		virtual ~RasterizerObject();

		virtual void CreateRasterizer(const DeviceRasterizerState& rs) = 0;

	protected:
	private:
	};
}