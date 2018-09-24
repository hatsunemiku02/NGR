#pragma once
#include "RenderCommandType.h"
#include "RenderDeviceTypes.h"

namespace RenderBase
{
	class BlendObject : public RenderCommandType
	{

	public:

		BlendObject();

		virtual ~BlendObject();

		virtual void CreateBlendObject(const BlendState& bs) = 0;
	};
}