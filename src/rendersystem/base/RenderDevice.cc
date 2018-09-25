

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
