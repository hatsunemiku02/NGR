

#include "stdneb.h"
#include "core/types.h"
#include "VertexLayout.h"
#include <vector>
namespace RenderBase
{
	//------------------------------------------------------------------------------
	/**
	*/
	VertexLayout::VertexLayout() :
	vertexByteSize(0),
	componentstoNum(0)
	{
		// empty
	}

	//------------------------------------------------------------------------------
	/**
	*/
	VertexLayout::~VertexLayout()
	{
		if (this->IsValid())
		{
			Discard();
		}
		assert(!this->IsValid());
	}

	void VertexLayout::CopyFrom(const VertexLayout* vl)
	{
		components = vl->components;
		vertexByteSize = vl->vertexByteSize;
		componentstoNum = vl->componentstoNum;
	}

	//------------------------------------------------------------------------------
	/**
	*/
	std::string
		VertexLayout::BuildSignature(const std::vector<VertexComponent>& comps)
	{
		assert(comps.size() > 0);
		std::string sig;
		IndexT i;
		for (i = 0; i < comps.size(); i++)
		{
			sig.append(comps[i].GetSignature());
		}
		return sig;
	}

	//------------------------------------------------------------------------------
	/**
	*/
	void
		VertexLayout::Setup(const std::vector<VertexComponent>& comps)
	{
		assert(comps.size() > 0);
		assert(!this->IsValid());
		assert(0 == this->vertexByteSize);
		this->components = comps;
		this->vertexByteSize = VertexComponent::BuildComponentsOffsetAndSize(this->components);

		for (IndexT i = 0; i < comps.size(); ++i)
		{
			this->componentstoNum += static_cast<IndexT>(comps[i].GetSemanticName());
		}
	}

	//------------------------------------------------------------------------------
	/**
	*/
	void
		VertexLayout::Discard()
	{
		if(this->IsValid())
		{
			this->components.clear();
			this->vertexByteSize = 0;
		}
	}

	//------------------------------------------------------------------------------
	/**
	*/
	IndexT
		VertexLayout::FindComponent(VertexComponent::SemanticName semName, IndexT semIndex) const
	{
		IndexT i;
		for (i = 0; i < this->components.size(); i++)
		{
			const VertexComponent& cur = this->components[i];
			if ((cur.GetSemanticName() == semName) && (cur.GetSemanticIndex() == semIndex))
			{
				return i;
			}
		}
		// fallthrough: not found
		return InvalidIndex;
	}
	//------------------------------------------------------------------------------
	/**
	*/
	bool
		VertexLayout::HasSameVertexComponent(const VertexLayout* lhs, const VertexLayout* rhs)
	{
		return lhs->componentstoNum == rhs->componentstoNum;
	}

}