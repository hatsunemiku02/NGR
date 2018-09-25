#ifndef __BUFFERDATA_H__
#define __BUFFERDATA_H__
#ifdef __OSX__
#include "../rendersystem/base/VertexComponent.h"
#include "../rendersystem/base/RenderDeviceTypes.h"
#else
#include "rendersystem/base/VertexComponent.h"
#include "rendersystem/base/RenderDeviceTypes.h"
#endif
namespace RenderBase
{
	const int MaxNumVertexStreams = 2;

	struct BufferData
	{
		enum Usage
		{
			Defalut,
			Static,
			Dynamic,
			Staging,
		};

		enum CPU_ACCESS
		{
			CPU_NONE = 0,
			CPU_READ,
			CPU_WRITE
		};
		Usage usage;
		CPU_ACCESS access;
	public:
		inline BufferData(Usage _usage, CPU_ACCESS ca = CPU_NONE)
			:usage(_usage),
			 access(ca)
		{

		}
	};
	struct DataStream
	{
		void* data;
		int sizeInByte;
		inline DataStream()
			:data(NULL)
			,sizeInByte(0)
		{

		}
	};

	typedef std::vector<VertexComponent> VertexComponents;
	struct VertexStream
	{
		void* data;
		int offsetInByte;
		int elemSizeInByte;
		inline VertexStream()
			:data(NULL)
			,offsetInByte(0)
			,elemSizeInByte(0)
		{

		}
	};

	typedef std::vector<VertexStream> VertexStreams;

	struct VertexBufferData : public BufferData
	{
		struct Vertex
		{
			VertexComponents vertexComponents;
			VertexStreams vertexComponentStreams;
		};
		Vertex vertex;
		//VertexStream uniqueStream;
		RenderBase::PrimitiveTopology::Code topology;
		int vertexCount;
		inline VertexBufferData()
			: BufferData(Static)
			, topology(RenderBase::PrimitiveTopology::InvalidPrimitiveTopology)
		{

		}
	};

	typedef void* IndexStream;
	struct IndexBufferData : public BufferData
	{
		enum IndexType
		{
			Unknown,
			Int16,
			Int32,
		};
		int indexCount;
		IndexType indexType;
		IndexStream stream;
		inline IndexBufferData()
			: BufferData(Static)
			, indexCount(0)
			, stream(NULL)
			, indexType(Unknown)
		{

		}

		static int SizeOf(IndexType type);
	};

	struct VERTEX_DESC
	{
		BufferData::Usage usage;
		RenderBase::PrimitiveTopology::Code topology;
		int vertexCount;
		int vertexSizeInbyte;
	};
	struct INDEX_DESC
	{
		BufferData::Usage usage;
		IndexBufferData::IndexType indexType;
		int indexCount;
	};

	struct TEXTURE_DESC
	{
		SizeT width;
		SizeT height;
		SizeT depth;
		SizeT numMipLevels;
		PixelFormat::Code pixelFormat;
	};

	void DirectCopyVertexDataToGraphicBuffer(const VertexBufferData& source, void* to, int vertexByteSize);
}


#endif	//__BUFFERDATA_H__