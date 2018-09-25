

#include "stdneb.h"
#include "VertexBuffer.h"

namespace RenderBase
{
	
//------------------------------------------------------------------------------
/**
*/
VertexBuffer::VertexBuffer() :
numVertices(0),
vertexDataSize(0),
usage(BufferData::Static)
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/
VertexBuffer::~VertexBuffer()
{
	if (0 != vertexLayout)
	{
		vertexLayout->Discard();
		vertexLayout = 0;
	}
}

void VertexBuffer::OnDeviceLost()
{

}

void VertexBuffer::OnDeviceReset()
{

}

}