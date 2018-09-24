#include "stdneb.h"
#include "VertexLayoutD3D12.h"
#include "D3D12Types.h"

#if RENDERDEVICE_D3D12

namespace D3D12
{
	using namespace RenderBase;


VertexLayoutD3D12::VertexLayoutD3D12()
{

}

VertexLayoutD3D12::~VertexLayoutD3D12()
{

}

void VertexLayoutD3D12::GenerateDeclarationD3D12()
{
	assert(GetVertexComponents().size() > 0);

	components = GetVertexComponents();

	IndexT compIndex;
	UINT   offset = 0;
	for (compIndex = 0; compIndex < this->components.size(); compIndex++)
	{
		D3D12_INPUT_ELEMENT_DESC desc;

		const VertexComponent& component = this->components[compIndex];
		UINT streamIndex = (UINT) component.GetStreamIndex();
		assert(streamIndex < RenderBase::MaxNumVertexStreams);

		desc.SemanticName         = D3D12Types::AsD3D12SemanticName(component.GetSemanticName());
		desc.SemanticIndex        = (UINT)component.GetSemanticIndex();
		desc.Format               = D3D12Types::AsD3D12VertexDeclarationType(component.GetFormat());
		desc.InputSlot			  = streamIndex;
		desc.AlignedByteOffset	  = offset;
		desc.InputSlotClass       = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
		desc.InstanceDataStepRate = 0;

		offset += component.GetByteSize();

		m_InputDescCollection.push_back(desc);

	}
}

}



#endif