
#include "stdneb.h"
#include "VertexComponent.h"

namespace RenderBase
{
	size_t VertexComponent::BuildComponentsOffsetAndSize(std::vector<VertexComponent>& in_out)
	{
		IndexT i;
		int vertexByteSize = 0;
		for (i = 0; i < in_out.size(); i++)
		{
			// update the components byte offset while we're at it
			in_out[i].SetByteOffset(vertexByteSize);
			vertexByteSize += in_out[i].GetByteSize();
		}
		return vertexByteSize;
	}
}