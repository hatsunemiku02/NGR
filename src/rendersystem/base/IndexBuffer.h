#pragma once

#ifndef INDEXBUFFER_H_
#define INDEXBUFFER_H_
#include <string>
#include "RenderDeviceTypes.h"
#include "core/types.h"
#ifdef __OSX__
#include "../rendersystem/config/RenderDeviceConfig.h"
#else
#include "config/RenderDeviceConfig.h"
#endif
#include "BufferData.h"

namespace RenderBase
{

class IndexBuffer 
{
public:

	virtual ~IndexBuffer();

	IndexBufferData::IndexType GetIndexType() const;
	SizeT GetIndexDataSize() const;
	SizeT GetNumIndices() const;
	BufferData::Usage GetUsage() const;

	void _SetNumIndices(SizeT num);
	void _SetIndexDataSize(SizeT size);
	void _SetIndexType(IndexBufferData::IndexType type);
	void _SetUsage(BufferData::Usage usage);

	/// on device lost
	virtual void OnDeviceLost();
	/// on device reset
	virtual void OnDeviceReset();

protected:	
	IndexBuffer();
	SizeT numIndices;
	SizeT indexDataSize;
	IndexBufferData::IndexType indexType;
	BufferData::Usage usage;

};

//------------------------------------------------------------------------------
/**
*/
inline void
IndexBuffer::_SetIndexType(IndexBufferData::IndexType type)
{
	this->indexType = type;
}

//------------------------------------------------------------------------------
/**
*/
inline IndexBufferData::IndexType
IndexBuffer::GetIndexType() const
{
	return this->indexType;
}
//------------------------------------------------------------------------------
/**
*/
inline BufferData::Usage
IndexBuffer::GetUsage() const
{
	return this->usage;
}
//------------------------------------------------------------------------------
/**
*/
inline void
IndexBuffer::_SetNumIndices(SizeT num)
{
	assert(num > 0);
	this->numIndices = num;
}

//------------------------------------------------------------------------------
/**
*/
inline SizeT
IndexBuffer::GetNumIndices() const
{
	return this->numIndices;
}
//------------------------------------------------------------------------------
/**
*/
inline void
IndexBuffer::_SetIndexDataSize(SizeT size)
{
	this->indexDataSize = size;
}

inline SizeT
IndexBuffer::GetIndexDataSize() const
{
	return this->indexDataSize;
}
inline void 
IndexBuffer::_SetUsage(BufferData::Usage usage)
{
	this->usage = usage;
}
}
#endif