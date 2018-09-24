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
#include "RenderDevice.h"
#include "ConstantBuffer.h"
#ifdef __OSX__
//#include "../../profilesystem/ProfileSystem.h"
#else
//#include "profilesystem/ProfileSystem.h"
#endif

namespace RenderBase
{
//------------------------------------------------------------------------------
/**
*/
RenderDevice::RenderDevice() :
	m_bDrawWireFrame(false),
	m_bDeviceLost( false ),
	//m_onDeviceLostFunc(NULL),
	//m_onDeviceResetFunc(NULL),
	m_deviceCallBack(NULL),
	m_bBeginFrame(false)
{
   
}

//------------------------------------------------------------------------------
/**
*/
RenderDevice::~RenderDevice()
{

}
//------------------------------------------------------------------------------
/**
*/
void
RenderDevice::SetStreamSource(IndexT streamIndex, const VertexBuffer* vb, IndexT offsetVertexIndex)
{
    assert((streamIndex >= 0) && (streamIndex < MaxNumVertexStreams));
    this->streamVertexBuffers[streamIndex] = vb;
    this->streamVertexOffsets[streamIndex] = offsetVertexIndex;
}
//--------------------------------------------------------------------------------
//void RenderDevice::SetStreamSourceFreq(IndexT streamIndex, IndexT setting)
//{
//	assert((streamIndex >= 0) && (streamIndex < MaxNumVertexStreams));
//
//}
//------------------------------------------------------------------------------
/**
*/
void
RenderDevice::DrawIndexedInstanced(SizeT numInstances)
{
    //assert(this->inBeginPass);
    // override in subclass!
}
//------------------------------------------------------------------------------
bool RenderDevice::BeginFrame()
{
	m_bBeginFrame = true;
	return true;
}
//------------------------------------------------------------------------------
void RenderDevice::EndFrame()
{
	m_bBeginFrame = false;
}
//------------------------------------------------------------------------------
bool RenderDevice::IsBeginFrame() const
{
	return m_bBeginFrame;
}
//------------------------------------------------------------------------------
SizeT RenderDevice::GetNumTriangles()
{
	return 0;// PROFILER_GETDEVICESTATEVAL(tris);
}
//------------------------------------------------------------------------------
SizeT RenderDevice::GetNumVertexs()
{
	return 0;// PROFILER_GETDEVICESTATEVAL(verts);
}
//------------------------------------------------------------------------------
SizeT RenderDevice::GetNumDrawCalls()
{
	return 0;// PROFILER_GETDEVICESTATEVAL(calls);
}
//------------------------------------------------------------------------------
SizeT RenderDevice::GetNumRenderTargets()
{
	return 0;// PROFILER_GETDEVICESTATEVAL(renderTargetNum);
}
//------------------------------------------------------------------------------
bool RenderDevice::LockTexture(const std::shared_ptr<Texture>& tex, ubyte* pData, IndexT mipLevel /* = 0 */)
{
	return false;
}
//------------------------------------------------------------------------------
bool RenderDevice::LockAllTexture(const std::shared_ptr<Texture>& tex, ubyte* pData)
{
	return false;
}
//------------------------------------------------------------------------------
std::shared_ptr<ConstantBuffer> RenderDevice::CreateConstantBuffer(SizeT nSize, const RenderBase::BufferData& desc)
{
	return NULL;
}
//------------------------------------------------------------------------------
void RenderDevice::UpdateConstantBuffer(const std::shared_ptr<ConstantBuffer>& pBuffer, const DataStream& data)
{

}
//------------------------------------------------------------------------------
void RenderDevice::SetVSConstantBuffer(std::weak_ptr<ConstantBuffer> pBuffer, IndexT bindPoint)
{

}
//------------------------------------------------------------------------------
void RenderDevice::SetPSConstantBuffer(std::weak_ptr<ConstantBuffer> pBuffer, IndexT bindPoint)
{

}
//------------------------------------------------------------------------------
std::shared_ptr<RootSignature> RenderDevice::CreateRootSignature(const SignatureInfo& info)
{
	return NULL;
}
//------------------------------------------------------------------------------
std::shared_ptr<Pso> RenderDevice::CreatePso(const PsoData& info, const std::shared_ptr<RootSignature>& rs)
{
	return NULL;
}
//------------------------------------------------------------------------------
void RenderDevice::ResetCommandList(const std::shared_ptr<Pso>& pso)
{

}
//------------------------------------------------------------------------------
void RenderDevice::SetPsoRootSignature(const std::shared_ptr<Pso>& pso, const std::shared_ptr<RootSignature>& rs)
{

}
//------------------------------------------------------------------------------
void RenderDevice::ExecuteCmmdList()
{

}
//------------------------------------------------------------------------------
void RenderDevice::FlushCmmdList()
{

}
}
