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
#ifndef VERTEXLAYOUT_H
#define VERTEXLAYOUT_H

#include "VertexComponent.h"
#include <assert.h>
#include <string>
#include <assert.h>
#include <map>
#include <memory>

namespace RenderBase
{
class VertexLayout 
{
public:
	/// constructor
	VertexLayout();
	/// destructor
	virtual ~VertexLayout();

	virtual void CopyFrom(const VertexLayout* vl);

	/// setup the vertex layout
	void Setup(const std::vector<VertexComponent>& c);
	/// return true if valid has been setup
	bool IsValid() const;

	/// get number of components
	SizeT GetNumComponents() const;
	/// get vertex component at index
	const VertexComponent& GetComponentAt(IndexT i) const;
	/// return true if vertex component exists
	bool HasComponent(VertexComponent::SemanticName semName, IndexT semIndex) const;
	/// get index of vertex component by semantics
	IndexT FindComponent(VertexComponent::SemanticName semName, IndexT semIndex) const;
	/// get the vertex stride in number of bytes
	SizeT GetVertexByteSize() const;
	/// get vertex components
	const std::vector<VertexComponent>& GetVertexComponents() const;   

		/// discard the vertex layout object
	void Discard();

	/// get sharing signature for a set of vertex components
	static std::string BuildSignature(const std::vector<VertexComponent>& c);

	static bool HasSameVertexComponent(const VertexLayout* lhs, const VertexLayout* rhs);
protected:
	friend class VertexBuffer;


	std::vector<VertexComponent> components;
	SizeT vertexByteSize;
	SizeT componentstoNum;
};

//------------------------------------------------------------------------------
/**
*/
inline bool
VertexLayout::IsValid() const
{
	return this->components.size() > 0;
}

//------------------------------------------------------------------------------
/**
*/
inline SizeT
VertexLayout::GetNumComponents() const
{
	return this->components.size();
}

//------------------------------------------------------------------------------
/**
*/
inline const VertexComponent&
VertexLayout::GetComponentAt(IndexT i) const
{
	return this->components[i];
}

//------------------------------------------------------------------------------
/**
*/
inline bool
VertexLayout::HasComponent(VertexComponent::SemanticName semName, IndexT semIndex) const
{
	return (InvalidIndex != this->FindComponent(semName, semIndex));
}

//------------------------------------------------------------------------------
/**
*/
inline SizeT
VertexLayout::GetVertexByteSize() const
{
	return this->vertexByteSize;
}

//------------------------------------------------------------------------------
/**
*/
inline
const std::vector<VertexComponent>& 
VertexLayout::GetVertexComponents() const
{
	return this->components;
}

static std::map<std::string, std::shared_ptr<VertexLayout> > s_vertexLayouts;
/// create shared vertex layout object
static std::shared_ptr<VertexLayout> CreateSharedVertexLayout(const std::vector<VertexComponent>& vertexComponents)
{
	assert(vertexComponents.size() > 0);

	// get sharing signature from vertex components
	std::string signature = VertexLayout::BuildSignature(vertexComponents);
	if (s_vertexLayouts.find(signature)!= s_vertexLayouts.end())
	{
		// return existing instance
		return s_vertexLayouts[signature];
	}
	else
	{
		// create new instance
		std::shared_ptr<VertexLayout> newVertexLayout =  std::make_shared<VertexLayout>();
		newVertexLayout->Setup(vertexComponents);
		s_vertexLayouts.insert(  std::make_pair(signature, newVertexLayout) );
		return newVertexLayout;
	}
}
}

#endif//VERTEXLAYOUT_H