#pragma once

#include "RenderDeviceTypes.h"

namespace RenderBase
{
class RootSignature : public RenderCommandType
{
public:
	RootSignature();

	virtual ~RootSignature();

	virtual void Init(const SignatureInfo& info) = 0;

protected:
private:
};

}