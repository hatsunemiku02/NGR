#pragma once

#include "RenderCommandType.h"
#include "RenderDeviceTypes.h"

namespace RenderBase
{
	class RootSignature;
	class Pso : public RenderCommandType
{
public:

	Pso();

	virtual ~Pso();

	virtual void Init(const PsoData& info, const std::shared_ptr<RootSignature>& pSignature) = 0;

};


}