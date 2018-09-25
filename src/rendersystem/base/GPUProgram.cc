

#include "stdneb.h"
#include "GPUProgram.h"

namespace RenderBase
{

	GPUProgram::GPUProgram()
	{
		SetRenderCommandType(RenderCommandType::SetGPUProgram);
	}

	GPUProgram::~GPUProgram()
	{
		//empty
	}
}