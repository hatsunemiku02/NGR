/****************************************************************************
Copyright (c) 2007, Radon Labs GmbH
Copyright (c) 2011-2013,WebJet Business Division,CYOU
 
http://

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/

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