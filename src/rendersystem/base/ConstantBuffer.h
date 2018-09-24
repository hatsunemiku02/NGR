#pragma once

#include "RenderCommandType.h"
#include "BufferData.h"

namespace RenderBase
{
	class ConstantBuffer : public RenderCommandType
{
public:

	ConstantBuffer();

	virtual ~ConstantBuffer();

	virtual void CreateBuffer(SizeT nSize, const RenderBase::BufferData& desc) = 0;

	virtual void UpdateBuffer(const DataStream& data) = 0;

	virtual void Discard() = 0;

protected:
	SizeT					m_nBufferSize;
};
}