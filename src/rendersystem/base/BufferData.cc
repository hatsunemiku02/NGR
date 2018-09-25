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

		//С��λ�Ŀ����У��ȺŸ�ֵ�ȵ����ڴ濽������Ҫ��ܶ�ܶࡣ
		//��xmemcpy��һ�����߾�����ô�ɵģ��ų�С��120�ֽ����ڵĿ�����gcc���ڴ濽����10����
		//�ȺŸ�ֵ���ȱ���<=128�ֽڣ������ǻ����ֳ��ı���

		//����Ⱦϵͳ�У���������ݵ���С��λ��4byte, ��һ��int32�Ĵ�С
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
		case 6://VertexComponent::SkinJIndices��VertexComponent::SkinWeights�Ǻ���һ��ģ����Ի���6�������
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
			printf("error vertex data size");//ֻ�����ϵ��Ǽ��ִ�С
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