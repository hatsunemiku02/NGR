

#pragma once
#include "stdneb.h"
#ifdef __OSX__
#include "../rendersystem/base/RenderResource.h"
#else
#include "base/RenderResource.h"
#endif
//------------------------------------------------------------------------------
namespace RenderBase
{

RenderResource::RenderResource():
	usage(UsageDynamic),
	access(AccessReadWrite)
{

}

RenderResource::~RenderResource()
{

}

void RenderResource::OnDeviceLost()
{
	///empty.
}

void RenderResource::OnDeviceReset()
{
	///empty.
}
} // namespace Base
//------------------------------------------------------------------------------
