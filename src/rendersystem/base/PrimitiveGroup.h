#pragma once

#ifndef PRIMITIVEGROUP_H_
#define PRIMITIVEGROUP_H_
#include "math/bbox.h"
#include "RenderDeviceTypes.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "RenderCommandType.h"

namespace RenderBase
{

	class PrimitiveGroup : public RenderCommandType
	{
	public:
		/// destructor
		virtual ~PrimitiveGroup();

		virtual void Discard();

		/// set base vertex index
		void SetBaseVertex(IndexT i);
		/// get index of base vertex
		IndexT GetBaseVertex() const;
		/// set number of vertices
		void SetNumVertices(SizeT n);
		///get number of vertices
		SizeT GetNumVertices() const;
		/// set base index index
		void SetBaseIndex(IndexT i);
		/// get base index index
		IndexT GetBaseIndex() const;
		/// set number of indices
		void SetNumIndices(SizeT n);
		/// get number of indices
		SizeT GetNumIndices() const;
		/// set the primitive topology
		void SetPrimitiveTopology(PrimitiveTopology::Code topology);
		/// get the primitive topology
		PrimitiveTopology::Code GetPrimitiveTopology() const;
		/// set the primitive group's local bounding box
		void SetBoundingBox(const Math::bbox& b);
		/// get the primitive group's local bounding box
		const Math::bbox& GetBoundingBox() const;
		/// get computed number of primitives
		SizeT GetNumPrimitives() const;
		/// compute number of primitives from a given vertices;
		SizeT NumberOfPrimitives(PrimitiveTopology::Code topology, SizeT numVertices) const;
		/// compute number of vertices from a given primitive;
		SizeT NumberOfVertices(PrimitiveTopology::Code topology, SizeT numPrimitives) const;

		/// get VertexBuffer
		const std::shared_ptr<VertexBuffer>& GetVertexBuffer() const;
		/// get IndexBuffer
		const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const;

		/// get count of InstVBuffer
		SizeT NumOfInstVBuffers() const;
		/// get a vb through index 
		const std::shared_ptr<VertexBuffer>& GetInstanceVBuffer(IndexT index) const;
		//  get vbuffers array
		const std::vector< std::shared_ptr<VertexBuffer> >& GetInstVBufferArray() const; 
		/// set instcount
		void SetInstanceCount(SizeT);
		/// get instcount
		SizeT GetInstanceCount() const;

		/// on device lost
		virtual void OnDeviceLost();

		/// on device reset
		virtual void OnDeviceReset();

	protected:
		/// constructor
		PrimitiveGroup();

		IndexT baseVertex;
		SizeT numVertices;
		IndexT baseIndex;
		SizeT numIndices;
		PrimitiveTopology::Code primitiveTopology;
		Math::bbox boundingBox;
		std::shared_ptr<VertexBuffer> m_vertexBuffer;
		std::shared_ptr<IndexBuffer> m_indexBuffer;

		std::vector< std::shared_ptr<VertexBuffer> > m_InstanceVBuffers;
		SizeT m_InstanceCount;

	};

	//------------------------------------------------------------------------------
	/**
	*/
	inline void
		PrimitiveGroup::SetBaseVertex(IndexT i)
	{
		this->baseVertex = i;
	}

	//------------------------------------------------------------------------------
	/**
	*/
	inline IndexT
		PrimitiveGroup::GetBaseVertex() const
	{
		return this->baseVertex;
	}

	//------------------------------------------------------------------------------
	/**
	*/
	inline void
		PrimitiveGroup::SetNumVertices(SizeT n)
	{
		this->numVertices = n;
	}

	//------------------------------------------------------------------------------
	/**
	*/
	inline SizeT
		PrimitiveGroup::GetNumVertices() const
	{
		return this->numVertices;
	}

	//------------------------------------------------------------------------------
	/**
	*/
	inline void
		PrimitiveGroup::SetBaseIndex(IndexT i)
	{
		this->baseIndex = i;
	}

	//------------------------------------------------------------------------------
	/**
	*/
	inline IndexT
		PrimitiveGroup::GetBaseIndex() const
	{
		return this->baseIndex;
	}

	//------------------------------------------------------------------------------
	/**
	*/
	inline void
		PrimitiveGroup::SetNumIndices(SizeT n)
	{
		this->numIndices = n;
	}

	//------------------------------------------------------------------------------
	/**
	*/
	inline SizeT
		PrimitiveGroup::GetNumIndices() const
	{
		return this->numIndices;
	}

	//------------------------------------------------------------------------------
	/**
	*/
	inline void
		PrimitiveGroup::SetPrimitiveTopology(PrimitiveTopology::Code t)
	{
		this->primitiveTopology = t;
	}

	//------------------------------------------------------------------------------
	/**
	*/
	inline PrimitiveTopology::Code
		PrimitiveGroup::GetPrimitiveTopology() const
	{
		return this->primitiveTopology;
	}

	//------------------------------------------------------------------------------
	/**
	*/
	inline void
		PrimitiveGroup::SetBoundingBox(const Math::bbox& b)
	{
		this->boundingBox = b;
	}

	//------------------------------------------------------------------------------
	/**
	*/
	inline const Math::bbox&
		PrimitiveGroup::GetBoundingBox() const
	{
		return this->boundingBox;
	}

	//------------------------------------------------------------------------------
	/**
	*/
	inline const std::shared_ptr<VertexBuffer>&
		PrimitiveGroup::GetVertexBuffer() const
	{
		return this->m_vertexBuffer;
	}

	//------------------------------------------------------------------------------
	/**
	*/
	inline const std::shared_ptr<IndexBuffer>&
		PrimitiveGroup::GetIndexBuffer() const
	{
		return this->m_indexBuffer;
	}

	//------------------------------------------------------------------------------
	/**
	*/
	inline SizeT
		PrimitiveGroup::GetNumPrimitives() const
	{
		if (this->numIndices > 0)
		{
			return NumberOfPrimitives(this->primitiveTopology, this->numIndices);
		}
		else
		{
			return NumberOfPrimitives(this->primitiveTopology, this->numVertices);
		}
	}
	//--------------------------------------------------------------------------------
	inline SizeT PrimitiveGroup::NumOfInstVBuffers() const
	{
		return m_InstanceVBuffers.size();
	}
	//--------------------------------------------------------------------------------
	inline const std::shared_ptr<VertexBuffer>& PrimitiveGroup::GetInstanceVBuffer(IndexT index) const
	{
		return m_InstanceVBuffers[index];
	}
	//--------------------------------------------------------------------------------
	inline const std::vector< std::shared_ptr<VertexBuffer> >& PrimitiveGroup::GetInstVBufferArray() const
	{
		return m_InstanceVBuffers;
	}
	//--------------------------------------------------------------------------------
	inline void PrimitiveGroup::SetInstanceCount(SizeT instcount)
	{
		m_InstanceCount = instcount;
	}
	//--------------------------------------------------------------------------------
	inline SizeT PrimitiveGroup::GetInstanceCount() const
	{
		return m_InstanceCount;
	}
}
#endif