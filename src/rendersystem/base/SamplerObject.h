#pragma once

#include "RenderCommandType.h"
#include "RenderDeviceTypes.h"

namespace RenderBase
{
	class SamplerObject : public RenderCommandType
	{

	public:

		SamplerObject();

		virtual ~SamplerObject();

		virtual void CreateSampler(const SamplerState& ss) = 0;

	protected:
	private:
	};
}