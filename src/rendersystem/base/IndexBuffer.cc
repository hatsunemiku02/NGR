

#include "stdneb.h"
#include "IndexBuffer.h"


namespace RenderBase
{
	
IndexBuffer::IndexBuffer() :
    indexType(IndexBufferData::Unknown),
    numIndices(0),
    indexDataSize(0)
{
	
    // empty
}

IndexBuffer::~IndexBuffer()
{
}

void IndexBuffer::OnDeviceLost()
{

}

void IndexBuffer::OnDeviceReset()
{

}

}