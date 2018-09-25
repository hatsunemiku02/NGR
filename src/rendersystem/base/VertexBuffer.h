
#ifndef VERTEXBUFFER_H
#define VERTEXBUFFER_H

//#include "RenderResource.h"
#include <memory>
#include "VertexComponent.h"
#include "VertexLayout.h"
#include "BufferData.h"

namespace RenderBase
{
	class VertexBuffer
	{
		
	public:
		
		/// destructor
		virtual ~VertexBuffer();

		/// get the vertex layout
		const std::shared_ptr<VertexLayout>& GetVertexLayout() const;

		/// get number of vertices in the buffer
		SizeT GetNumVertices() const;
		///
		SizeT GetVertexDataSize() const;

		BufferData::Usage GetUsage() const;

		///// get the vertex layout
		//void _SetVertexLayout(const std::shared_ptr<VertexLayout>& vl);

		/// get number of vertices in the buffer
		void _SetNumVertices(SizeT num);
		///
		void _SetVertexDataSize(SizeT size);

		void _SetUsage(BufferData::Usage usage);

		/// on device lost
		virtual void OnDeviceLost();
		/// on device reset
		virtual void OnDeviceReset();

	protected:
		/// constructor
		VertexBuffer();
		BufferData::Usage usage;
		SizeT numVertices;
		SizeT vertexDataSize;
		std::shared_ptr<VertexLayout> vertexLayout;
	};

	inline const std::shared_ptr<VertexLayout>&
		VertexBuffer::GetVertexLayout() const
	{
		return this->vertexLayout;
	}

	inline SizeT
		VertexBuffer::GetNumVertices() const
	{
		return this->numVertices;
	}

	inline SizeT
		VertexBuffer::GetVertexDataSize() const
	{
		return this->vertexDataSize;
	}

	inline BufferData::Usage
		VertexBuffer::GetUsage() const
	{
		return this->usage;
	}

	inline void
		VertexBuffer::_SetNumVertices(SizeT num)
	{
		this->numVertices = num;
	}

	inline void VertexBuffer::_SetVertexDataSize(SizeT size)
	{
		this->vertexDataSize = size;
	}

	inline void VertexBuffer::_SetUsage(BufferData::Usage usage)
	{
		this->usage = usage;
	}
}

#endif