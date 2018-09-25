/****************************************************************************
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
#ifdef __OSX__
#include "../rendersystem/base/BufferData.h"
#else
#include "rendersystem/base/BufferData.h"
#endif
#include "memory/memory.h"

namespace RenderBase
{
	void _copyVertexData(const VertexStream& from, void* to, int vertexByteSize, int vertexCount)
	{

		//小单位的拷贝中，等号赋值比调用内存拷贝函数要快很多很多。
		//叫xmemcpy的一个工具就是这么干的，号称小于120字节以内的拷贝比gcc的内存拷贝快10倍。
		//等号赋值长度必须<=128字节，并且是机器字长的倍数

		//在渲染系统中，顶点的数据的最小单位是4byte, 即一个int32的大小
		assert(vertexByteSize > 0);
		assert(NULL != from.data);
		assert(NULL != to);
		assert((from.elemSizeInByte % 4) == 0);
		assert((vertexByteSize % 4) == 0);

		int data_stride4 = from.elemSizeInByte / 4;
		int vertex_stride4 = vertexByteSize / 4;
		const uint32* buffer32 = (const uint32*)(from.data);
		uint32* dest = (uint32*)(((uint8*)to) + from.offsetInByte);

		switch(data_stride4)
		{
		case 1:
			{		
				while(vertexCount--)
				{
#if __OSX__||__WIN64__
					Memory::CopySmallOne((uint32*)buffer32, (uint32*)dest);
#else
					Memory::CopySmallOne(buffer32, dest);
#endif
					++buffer32;
					dest += vertex_stride4;
				}
				break;
			}
		case 2:
			{
				while(vertexCount--)
				{
#if __OSX__||__WIN64__
					Memory::CopySmallTwo((uint32*)buffer32, (uint32*)dest);
#else
					Memory::CopySmallTwo(buffer32, dest);
#endif

					buffer32 += 2;
					dest += vertex_stride4;
				}
				break;
			}
		case 3:
			{
				while(vertexCount--)
				{
#if __OSX__||__WIN64__ 
					Memory::CopySmallThree((uint32*)buffer32, (uint32*)dest);
#else
					Memory::CopySmallThree(buffer32, dest);
#endif

					buffer32 += 3;
					dest += vertex_stride4;
				}
				break;
			}
		case 4:
			{
				while(vertexCount--)
				{
#if __OSX__||__WIN64__
					Memory::CopySmallFour((uint32*)buffer32, (uint32*)dest);
#else
					Memory::CopySmallFour(buffer32, dest);
#endif
					buffer32 += 4;
					dest += vertex_stride4;
				}
				break;
			}
		case 5:
			{
				while(vertexCount--)
				{
#if __OSX__||__WIN64__
					Memory::CopySmallFive((uint32*)buffer32, (uint32*)dest);
#else
					Memory::CopySmallFive(buffer32, dest);
#endif

					buffer32 += 5;
					dest += vertex_stride4;
				}
				break;
			}
		case 6://VertexComponent::SkinJIndices和VertexComponent::SkinWeights是合在一起的，所以会有6的情况。
			{
				while(vertexCount--)
				{
#if __OSX__||__WIN64__
					Memory::CopySmallSix((uint32*)buffer32, (uint32*)dest);
#else
					Memory::CopySmallSix(buffer32, dest);
#endif

					buffer32 += 6;
					dest += vertex_stride4;
				}
				break;
			}
		default:
			printf("error vertex data size");//只有以上的那几种大小
			assert(0);
			break;
		}

	}

	void DirectCopyVertexDataToGraphicBuffer(const VertexBufferData& source, void* to, int vertexByteSize)
	{
		const VertexStreams& dataStreams = source.vertex.vertexComponentStreams;
// 		if (source.vertex.vertexComponentStreams.size() == 1
// 			&& source.vertex.vertexComponentStreams[0].elemSizeInByte == vertexByteSize
// 			&& source.vertex.vertexComponentStreams[0].data)
// 		{
// 			memcpy(dataStreams[0].data, to, vertexByteSize * source.vertexCount);
// 			//Memory::Copy(dataStreams[0].data,to, vertexByteSize * source.vertexCount);
// 		}
// 		else
		{
			for (int i = 0; i < dataStreams.size(); ++i)
			{
				VertexStream stream = dataStreams[i];
				if (stream.data)
				{
					_copyVertexData(stream, to, vertexByteSize, source.vertexCount);
				}
			}

		}


	}

	int IndexBufferData::SizeOf(IndexType type)
	{
		switch(type)
		{
		case Int16:
			return 2;
		case Int32:
			return 4;
		default:
			return 0;
		}
	}
}