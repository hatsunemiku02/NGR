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
#include "RenderSystem.h"
#include "config/RenderDeviceConfig.h"

#if RENDERDEVICE_D3D9
#include "d3d9/RenderDeviceD3D9.h"
#include "d3d9/PrimitiveGroupD3D9.h"
#elif RENDERDEVICE_D3D11
#include "d3d11/RenderDeviceD3D11.h"
#elif RENDERDEVICE_D3D12
#include "d3d12/RenderDeviceD3D12.h"
#include "base/ConstantBuffer.h"
#include "base/RootSignature.h"
#include "base/Pso.h"
#endif
#if RENDERDEVICE_OPENGLES20
#include "gles20/RenderDeviceGLES20.h"
#elif RENDERDEVICE_OPENGLES30
#include "gles30/RenderDeviceGLES30.h"
#endif
#if RENDERDEVICE_NULL
#include "null/RenderDeviceNull.h"
#endif

#include "base/ConstantBuffer.h"
#include "base/PrimitiveGroup.h"
//#include "RenderMessageProtocol.h"

namespace RenderBase
{

	RenderSystem* RenderSystem::m_pInstance = nullptr;

	RenderSystem* RenderSystem::Instance()
	{
		return m_pInstance;
	}




#if RENDERDEVICE_D3D9
	using namespace D3D9;
#endif

#if RENDERDEVICE_D3D11
	using namespace D3D11;
#elif RENDERDEVICE_D3D12
	using namespace D3D12;
#endif

#if RENDERDEVICE_OPENGLES20 || RENDERDEVICE_OPENGLES30
	using namespace GLES;
#endif

#if RENDERDEVICE_NULL
	using namespace NullDevice;
#endif

 	void RenderSystem::Open(int width, int height)
 	{
		
#if RENDERDEVICE_D3D9
		m_renderDevice = RenderDeviceD3D9::Create();
		m_renderDevice.cast<RenderDeviceD3D9>()->SetMainWindowHandle(m_mainHWND);
#endif

#if RENDERDEVICE_D3D11
		m_renderDevice = RenderDeviceD3D11::Create();
		m_renderDevice.cast<RenderDeviceD3D11>()->SetMainWindowHandle(m_mainHWND);
#endif

#if RENDERDEVICE_D3D12
		m_renderDevice = std::make_shared<RenderDeviceD3D12>();
	    std::static_pointer_cast<RenderDeviceD3D12>(m_renderDevice)->SetMainWindowHandle(m_mainHWND);
#endif

#if RENDERDEVICE_NULL
		m_renderDevice = RenderDeviceNull::Create();
		m_renderDevice.cast<RenderDeviceNull>()->SetMainWindowHandle(m_mainHWND);
#endif

#if RENDERDEVICE_OPENGLES20
		m_renderDevice = RenderDeviceGLES20::Create();
		m_renderDevice.cast<RenderDeviceGLES20>()->SetMainWindowHandle(m_mainHWND);
#elif RENDERDEVICE_OPENGLES30
		m_renderDevice = RenderDeviceGLES30::Create();
		m_renderDevice.cast<RenderDeviceGLES30>()->SetMainWindowHandle(m_mainHWND);
#endif
		m_renderDevice->SetSize(width, height);
		m_renderDevice->InitDevice();

		m_renderDevice->DetectGraphicCardCaps();

		if (!m_dummyRenderTargetHandle.IsValid())
		{
			std::shared_ptr<RenderBase::RenderTarget> renderTarget = std::make_shared<RenderBase::RenderTarget>();// RenderBase::RenderTarget::Create();
			renderTarget->SetDefaultRenderTarget(true);
			renderTarget->SetClearColor(Math::float4(0.f,0.f,1.f,1.f));
			renderTarget->SetClearFlags(RenderBase::RenderTarget::ClearAll);
			renderTarget->SetWidth(1);
			renderTarget->SetHeight(1);
			renderTarget->SetDummy(true);
			RenderBase::TextureHandle dummytexhandle;
			m_dummyRenderTargetHandle = CreateRenderTarget(renderTarget,dummytexhandle);
		}
 	}


	void RenderSystem::SetMainWindowHandle(WindHandle hwnd)
	{
		m_mainHWND = hwnd;
#if RENDERDEVICE_D3D11
		if (m_renderDevice.isvalid())	
		{
			m_renderDevice.cast<RenderDeviceD3D11>()->SetMainWindowHandle(m_mainHWND);
		}	
#elif RENDERDEVICE_D3D12
		if (m_renderDevice)
		{
			std::static_pointer_cast<RenderDeviceD3D12>(m_renderDevice)->SetMainWindowHandle(m_mainHWND);
		}
#endif
	}
	WindHandle RenderSystem::GetMainWindowHandle(void)const
	{
		return m_mainHWND;
	}


	void RenderSystem::Close()
	{

		for ( RenderResourceHandleSet::type::iterator itor = m_renderHandles.begin();
			itor != m_renderHandles.end(); ++itor )
		{
			printf("memory leak is coming!!");
		//	itor->mRO->Release();
		}

		m_renderHandles.clear();
	}

	RenderSystem::RenderSystem()
	{
		m_pInstance = this;
		m_renderDisplay = std::make_shared<RenderDisplay>();// RenderDisplay::Create();
		
	}

	RenderSystem::~RenderSystem()
	{
		m_renderDisplay = 0;
		m_renderDevice = 0;
	}

	void RenderSystem::BeginFrame()
	{
		m_renderDevice->BeginFrame();
	}

	void RenderSystem::EndFrame()
	{
		m_renderDevice->EndFrame();
	}

	bool RenderSystem::IsBeginFrame() const
	{
		return m_renderDevice->IsBeginFrame();
	}

	void RenderSystem::Clear()
	{
		
	}

	std::weak_ptr<PrimitiveGroup> RenderSystem::GetPrimitiveGroup(const PrimitiveHandle& handle) const
	{
		if (RenderBase::RenderCommandType::DrawPrimGroup == handle.GetType())
		{
			return std::static_pointer_cast<PrimitiveGroup>(handle.mRO.lock());
		}
		return std::move(std::weak_ptr<PrimitiveGroup>());
	}

	std::weak_ptr<Texture> RenderSystem::GetTexture(const TextureHandle& handle) const
	{
		if (RenderBase::RenderCommandType::SetTexture == handle.GetType())
		{
			return std::weak_ptr<Texture>(std::static_pointer_cast<Texture>(handle.mRO.lock()));
		}
		return std::move(std::weak_ptr<Texture>());
	}

	ConstanBufferHandle RenderSystem::CreateConstantBufferHandle(SizeT nSize, const RenderBase::BufferData& desc)
	{
		std::shared_ptr<ConstantBuffer> pBuffer = m_renderDevice->CreateConstantBuffer(nSize, desc);
		pBuffer->SetRenderCommandType(RenderCommandType::CreateConstantBuffer);
		ConstanBufferHandle handle;
		handle.mRO =std::static_pointer_cast<RenderCommandType>(pBuffer);

		bool bOK = _AddRenderHandle(handle);
		assert(bOK);

		return handle;

	}

	void RenderSystem::UpdateConstantBuffer(const ConstanBufferHandle& handle, const DataStream& data)
	{
		std::weak_ptr<ConstantBuffer> pBuffer = GetConstantBuffer(handle);
		m_renderDevice->UpdateConstantBuffer(pBuffer.lock(), data);
	}

	std::weak_ptr<ConstantBuffer> RenderSystem::GetConstantBuffer(const ConstanBufferHandle& handle) const
	{
		if (RenderBase::RenderCommandType::CreateConstantBuffer == handle.GetType())
		{
			return  std::static_pointer_cast<ConstantBuffer>(handle.mRO.lock()) ;//   static_cast<ConstantBuffer*>(handle.mRO);
		}
		return std::weak_ptr<ConstantBuffer>();
	}

	void RenderSystem::SetVSConstantBuffer(const ConstanBufferHandle& handle, IndexT bindpoint)
	{
		std::weak_ptr<ConstantBuffer> pBuffer = GetConstantBuffer(handle);
		assert(pBuffer.expired());
		m_renderDevice->SetVSConstantBuffer(pBuffer, bindpoint);
	}

	void RenderSystem::SetPSConstantBuffer(const ConstanBufferHandle& handle, IndexT bindpoint)
	{
		std::weak_ptr<ConstantBuffer> pBuffer = GetConstantBuffer(handle);
		assert(pBuffer.expired());
		m_renderDevice->SetPSConstantBuffer(pBuffer, bindpoint);
	}

	PrimitiveHandle RenderSystem::CreatePrimitiveHandle(const std::shared_ptr<VertexBufferData>& vbd, const std::shared_ptr<IndexBufferData>& ibd /* = NULL */)
	{
		std::shared_ptr<PrimitiveGroup> pg = m_renderDevice->CreatePrimitiveGroup(vbd, ibd);
		pg->SetRenderCommandType(RenderCommandType::DrawPrimGroup);
		PrimitiveHandle handle;
		handle.mRO = pg;

		bool bOK = _AddRenderHandle(handle);
		assert(bOK);

		return std::move(handle);
	}

	void RenderSystem::ChangePrimitiveHandle(RenderBase::PrimitiveHandle& handle, const std::shared_ptr<RenderBase::VertexBufferData>& vbd, const std::shared_ptr<RenderBase::IndexBufferData>& ibd /* = NULL */)
	{
		assert(RenderBase::RenderCommandType::DrawPrimGroup == handle.GetType());
		
		m_renderDevice->ChangePrimitiveGroup(std::static_pointer_cast<PrimitiveGroup>(handle.mRO.lock()), vbd, ibd);
		
	}

	void RenderSystem::UpdateVertexBuffer(PrimitiveHandle& handle, const DataStream& data)
	{
		std::weak_ptr<PrimitiveGroup> pg = GetPrimitiveGroup(handle);
		m_renderDevice->UpdateVertexBuffer(pg.lock(), data);
	}

	void RenderSystem::UpdateIndexBuffer(PrimitiveHandle& handle, const DataStream& data)
	{
		std::weak_ptr<PrimitiveGroup> pg = GetPrimitiveGroup(handle);
		m_renderDevice->UpdateIndexBuffer(pg.lock(), data);
	}

	GPUProgramHandle RenderSystem::CreateShaderProgram( const std::shared_ptr<GPUProgram>& program )
	{
		std::shared_ptr<GPUProgram> gpu_program = m_renderDevice->CreateRenderGPUProgram(program);

		std::shared_ptr<RenderCommandType> rcType = std::static_pointer_cast<GPUProgram>(gpu_program);
		rcType->SetRenderCommandType(RenderCommandType::SetGPUProgram);

		GPUProgramHandle handle;
		handle.mRO = rcType;
		
		bool bOK = _AddRenderHandle(handle);
		assert(bOK);

		return std::move(handle);
	}

	RenderStateDescHandle RenderSystem::CreateRenderStateObject( const std::shared_ptr<RenderStateDesc>& rsObject)
	{
		assert(rsObject != nullptr);
		std::shared_ptr<RenderStateDesc> rsd = m_renderDevice->CreateRenderState(rsObject);
		const std::shared_ptr<RenderCommandType> rcType = std::static_pointer_cast<RenderCommandType>(rsd);
		rcType->SetRenderCommandType(RenderCommandType::SetRenderState);
		
		RenderStateDescHandle handle;
		handle.mRO = rcType;
		
		bool bOK = _AddRenderHandle(handle);
		assert(bOK);

		return handle;
	}

	RenderTargetHandle RenderSystem::CreateRenderTarget( const std::shared_ptr<RenderTarget>& rt, TextureHandle& texhandle)
	{
		assert(rt);
		
		//generate render target guid
		std::shared_ptr<RenderTarget> deviceRT = m_renderDevice->CreateRenderTarget(rt);
		std::shared_ptr<RenderCommandType> rcType = std::static_pointer_cast<RenderCommandType>(deviceRT);// deviceRT.upcast<RenderCommandType>();
		rcType->SetRenderCommandType(RenderCommandType::SetRenderTarget);

		RenderTargetHandle handle;
		handle.mRO = rcType;
		
		bool bOK = _AddRenderHandle(handle);
		assert(bOK);

		if (!rt->IsDefaultRenderTarget())
		{
			//generate render texture guid
			std::shared_ptr<RenderCommandType> rcType = std::static_pointer_cast<RenderCommandType>(deviceRT->GetResolveTexture());
			texhandle.mRO = rcType;
			
			bool bOK = _AddRenderHandle(texhandle);
			assert(bOK);
		}
		return handle;
	}

	MultipleRenderTargetHandle RenderSystem::CreateMultipleRenderTarget(const std::shared_ptr<MultipleRenderTarget>& mrt, std::vector< RenderBase::RenderTargetHandle >& handles)
	{
		assert(mrt);

		for (IndexT i = 0; i < handles.size(); ++i)
		{
			std::shared_ptr<RenderCommandType> rcType = handles[i].mRO.lock();
			std::shared_ptr<RenderTarget> rt =std::static_pointer_cast<RenderTarget>(rcType);
			assert(rt);

			mrt->AddRenderTarget(rt);
		}

		std::shared_ptr<MultipleRenderTarget> deviceMRT = m_renderDevice->CreateMultipleRenderTarget(mrt);
		std::shared_ptr<RenderCommandType> rcType = std::static_pointer_cast<RenderCommandType>(deviceMRT);
		rcType->SetRenderCommandType(RenderCommandType::SetMultiRenderTarget);

		MultipleRenderTargetHandle handle;
		handle.mRO = rcType;

		bool bOK = _AddRenderHandle(handle);
		assert(bOK);

		return handle;
	}

	TextureHandle RenderSystem::CreateTexture( const std::shared_ptr<Texture>& tex)
	{
		assert(tex!=nullptr);
		std::shared_ptr<RenderCommandType> rcType = std::static_pointer_cast<RenderCommandType>(m_renderDevice->CreateRenderSideTexture(tex));
		rcType->SetRenderCommandType(RenderCommandType::SetTexture);
		
		TextureHandle handle;
		handle.mRO = rcType;
		
		bool bOK = _AddRenderHandle(handle);
		assert(bOK);

		return handle;
	}

	void RenderSystem::UpdateTexture(TextureHandle texHandle, RenderBase::Texture::UpdateFunction texUpdateFunc, void* tag)
	{
		if (!texHandle.IsValid())
		{
			n_warning("RenderSystem::UpdateTexture():texHandle is invalid!\n");
			return;
		}

		std::shared_ptr<Texture> destTex = std::static_pointer_cast<Texture>(texHandle.mRO.lock());
		assert(destTex != nullptr);
		m_renderDevice->UpdateTexture(texUpdateFunc,destTex, tag);

	}

	void RenderSystem::UpdateTexture(TextureHandle texHandle, std::shared_ptr<RenderBase::Texture> texture)
	{
		if (!texHandle.IsValid())
		{
			n_warning("RenderSystem::UpdateTexture():texHandle is invalid!\n");
			return;
		}

		std::shared_ptr<Texture> destTex = std::static_pointer_cast<Texture>(texHandle.mRO.lock());
		assert(destTex != nullptr);
		m_renderDevice->UpdateTexture( texture, destTex );

	}

	void RenderSystem::ChangeTexture(RenderBase::TextureHandle texHandle, std::shared_ptr<Texture> texture)
	{
		if (!texHandle.IsValid())
		{
			n_warning("RenderSystem::UpdateTexture():texHandle is invalid!\n");
			return;
		}

		std::shared_ptr<Texture> destTex = std::static_pointer_cast<Texture>(texHandle.mRO.lock());
		assert(destTex != nullptr);
		m_renderDevice->ChangeTexture( texture, destTex );
	}

	void RenderSystem::SetSRGBRead(TextureHandle texHandle, bool bRead)
	{
		if (!texHandle.IsValid())
		{
			n_warning("RenderSystem::UpdateTexture():texHandle is invalid!\n");
			return;
		}

		std::shared_ptr<Texture> destTex = std::static_pointer_cast<Texture>(texHandle.mRO.lock());
		assert(destTex);
		destTex->SetSRGBRead(bRead);
	}

	void RenderSystem::_DrawPrimitive(PrimitiveHandle handle,SizeT startVertice,SizeT endVertice,SizeT startIndice,SizeT endIndice)
	{

		assert( handle.IsValid() );
		assert(startVertice >= 0);
		assert(endVertice >= 0);
		assert(startIndice >= 0);
		assert(endIndice >= 0);

		std::shared_ptr<PrimitiveGroup> primGroup = std::static_pointer_cast<PrimitiveGroup>(handle.mRO.lock());
		assert(primGroup);

		m_renderDevice->SetPrimitiveGroup(primGroup);

		m_renderDevice->Draw(startVertice, endVertice, startIndice, endIndice);
	}
	//--------------------------------------------------------------------------------
	void RenderSystem::_DrawPrimitive(PrimitiveHandle handle)
	{
		std::shared_ptr<PrimitiveGroup> primGroup = std::static_pointer_cast<PrimitiveGroup>(handle.mRO.lock());
		assert(primGroup);

		m_renderDevice->SetPrimitiveGroup(primGroup);
		m_renderDevice->Draw(primGroup->GetBaseVertex(), primGroup->GetNumVertices(), primGroup->GetBaseIndex(), primGroup->GetNumIndices());
	}
	//--------------------------------------------------------------------------------
	void RenderSystem::_DrawPrimitiveHWInstance(PrimitiveHandle handle)
	{	

		
	}
	//--------------------------------------------------------------------------------

	void RenderSystem::_SetRenderTarget(RenderTargetHandle handle,SizeT index,uint clearflag, bool bGammaToLinear)
	{
		if (!handle.IsValid())
		{

			if (0 != index)
			{
				m_renderDevice->DisableRenderTarget(index);
				return;
			}
			handle = m_dummyRenderTargetHandle;
		}
		
		std::shared_ptr<RenderTarget> rt = std::static_pointer_cast<RenderTarget>(handle.mRO.lock());
		assert(rt);

		if (handle != m_dummyRenderTargetHandle)
		{
			rt->SetMRTIndex(index);
			
		}
		rt->SetClearFlags(clearflag);
		m_renderDevice->SetRenderTarget(rt, bGammaToLinear);
	}
	//--------------------------------------------------------------------------------
	void RenderSystem::Resolve(TextureHandle texHandle)
	{
		std::shared_ptr<RenderTarget> rt = std::static_pointer_cast<RenderTarget>(texHandle.mRO.lock());
		assert(rt);
		rt->Resolve();
	}
	//--------------------------------------------------------------------------------
	void RenderSystem::_SetRenderTargetClearColor(RenderTargetHandle handle,const Math::float4& clearColor)
	{
		std::shared_ptr<RenderTarget> rt = std::static_pointer_cast<RenderTarget>(handle.mRO.lock());
		assert(rt);
		
		rt->SetClearColor(clearColor);
	}

	//--------------------------------------------------------------------------------
	void RenderSystem::_ReSizeRenderTarget(RenderTargetHandle handle,const int& width,const int& height)
	{
		std::shared_ptr<RenderTarget> rt = std::static_pointer_cast<RenderTarget>(handle.mRO.lock());
		assert(rt);
		rt->SetWidth(width);
		rt->SetHeight(height);
	}

	//--------------------------------------------------------------------------------
	void RenderSystem::CopyRenderTarget(RenderTargetHandle srcHandle, const Math::float4& srcRect, RenderTargetHandle desHandle, const Math::float4& desRect)
	{
		std::shared_ptr<RenderTarget> srcRT = std::static_pointer_cast<RenderTarget>(srcHandle.mRO.lock());
		assert(srcRT);
		std::shared_ptr<RenderTarget> desRT = std::static_pointer_cast<RenderTarget>(desHandle.mRO.lock());
		assert(desRT);
		desRT->CopyFrom(srcRect,srcRT,desRect);

	}

	void RenderSystem::_SetMultipleRenderTarget(MultipleRenderTargetHandle handle, bool resume)
	{
		std::shared_ptr<MultipleRenderTarget> mrt = std::static_pointer_cast<MultipleRenderTarget>(handle.mRO.lock());

		m_renderDevice->SetMultipleRenderTarget(mrt, resume);
	}

	void RenderSystem::_ReSizeMultipleRenderTarget(MultipleRenderTargetHandle handle,const int& width,const int& height)
	{
		//TODO
		n_error("RenderSystem::_ReSizeMultipleRenderTarget() has not implemented!");
	}

	void RenderSystem::_SetTexture(TextureHandle handle,SizeT index)
	{
		assert(handle.IsValid());

		std::shared_ptr<Texture> tex = std::static_pointer_cast<Texture>(handle.mRO.lock());
		assert(tex);

		assert(index >= 0 && index <= 12);

		tex->SetUnitIndex(index);
		m_renderDevice->SetTexture(tex);
	}

	void RenderSystem::_SetShaderProgram(GPUProgramHandle handle)
	{
		assert(handle.IsValid());

		
		std::shared_ptr<GPUProgram> program = std::static_pointer_cast<GPUProgram>(handle.mRO.lock());
		assert(program);

		m_renderDevice->SetGPUProgram(program);
	}

	void RenderSystem::SetVertexShaderConstantVectorF(const int& reg, float* val, const int& vec4count)
	{
		m_renderDevice->SetVertexShaderConstantVectorF(reg,val,vec4count);
	}

	void RenderSystem::SetPixelShaderConstantVectorF(const int& reg, float* val, const int& vec4count)
	{
		m_renderDevice->SetPixelShaderConstantVectorF(reg,val,vec4count);
	}

	void RenderSystem::SetVertexShaderConstantFloat(const int& reg, float* val)
	{
		m_renderDevice->SetVertexShaderConstantFloat(reg, val);
	}

	void RenderSystem::SetPixelShaderConstantFloat(const int& reg, float* val)
	{
		m_renderDevice->SetPixelShaderConstantFloat(reg, val);
	}

	void RenderSystem::SetVertexShaderConstantMatrixF(const int& reg, float* val, const int& matrixCount)
	{
		m_renderDevice->SetVertexShaderConstantMatrixF(reg, val, matrixCount);
	}

	void RenderSystem::SetPixelShaderConstantMatrixF(const int& reg, float* val, const int& matrixCount)
	{
		m_renderDevice->SetPixelShaderConstantMatrixF(reg, val, matrixCount);
	}

	void RenderSystem::_SetRenderState(const std::shared_ptr<RenderStateDesc>& rsObject, const uint& shaderMask)
	{
		assert(rsObject);
		unsigned int flag = rsObject->GetUpdateFlag();
		assert(RenderStateDesc::eInvalidRenderState != flag);
		if (flag & RenderStateDesc::eRenderSamplerState)
		{
#if RENDERDEVICE_D3D9 || RENDERDEVICE_OPENGLES30
			m_renderDevice->SetTextureSamplerState(rsObject->GetSamplerState(shaderMask));
#elif RENDERDEVICE_D3D11
				const std::vector<DeviceSamplerState>& dsList = rsObject->GetSamplerState(shaderMask);
				for (IndexT i = 0; i < dsList.Size(); ++i)
				{
				m_renderDevice->SetTextureSamplerState(dsList[i]);
				}
#endif
				
		}
		if (flag & RenderStateDesc::eRenderBlendState)
		{
			m_renderDevice->SetBlendState(rsObject->GetBlendState());
		}
		if (flag & RenderStateDesc::eRenderDepthAndStencilState)
		{
			m_renderDevice->SetDepthAndStencilState(rsObject->GetDepthAndStencilState());
		}
		if (flag & RenderStateDesc::eRenderRasterizerState)
		{
			m_renderDevice->SetRasterState(rsObject->GetRasterizerState());
		}

	}

	void RenderSystem::_RemoveResouce(const RenderResourceHandle& handle)
	{
		if ( handle.IsValid() )
		{
			bool bOK = _RemoveRenderHandle( handle );
			assert(bOK);
		}
	}

	void RenderSystem::_SetDisplayMode(const DisplayMode& mode)
	{

	}

	void RenderSystem::_SetViewPort(const int& x,const int& y,const int& width,const int& height,const float& minZ,const float& maxZ)
	{
		m_renderDevice->SetViewPort(x,y,width,height,minZ,maxZ);
	}
	//////////////////////////////////////////////////////////////////////////

	void RenderSystem::_FXSetClipPlane(const int& index,const Math::float4& plane)
	{
		m_renderDevice->FXSetClipPlane(index,plane);
	}

	//------------------------------------------------------------------------
	bool RenderSystem::_AddRenderHandle( const RenderBase::RenderResourceHandle& handle)
	{
		assert(handle);

		RenderResourceHandleSet::InsertResult res = m_renderHandles.insert(handle);

		if( res.second )
		{
			//res.first->mRO->AddRef();	//	make a new insert
			assert(0);
			return true;
		}
		else
		{
			return false;
		}
	}
	//------------------------------------------------------------------------
	bool RenderSystem::_RemoveRenderHandle( const RenderBase::RenderResourceHandle& handle)
	{
		assert(handle);
		RenderResourceHandleSet::type::iterator itor = m_renderHandles.find(handle);
		if ( itor != m_renderHandles.end() )
		{
			RenderResourceHandle oldHandle = *itor;
			assert(oldHandle);
			m_renderHandles.erase(itor);
			//oldHandle.mRO->Release();
			assert(0);
			return true;
		}
		else
		{
			return false;
		}
	}
	//------------------------------------------------------------------------
	bool RenderSystem::_HasRenderHandle(const RenderBase::RenderResourceHandle& handle)
	{
		return m_renderHandles.find( handle ) != m_renderHandles.end();
	}

	std::shared_ptr<RenderTarget>	RenderSystem::GetRenderTarget( RenderTargetHandle hdl )
	{
		if ( !hdl.IsValid() )
		{
			return nullptr ; 
		}
		std::shared_ptr<RenderTarget> rt = std::static_pointer_cast<RenderTarget>(hdl.mRO.lock());
		assert(rt);
		return rt ;
	}

	bool RenderSystem::OutPutRenderTarget( RenderTargetHandle hdl , const std::string& path )
	{
		std::shared_ptr<RenderTarget> rt = GetRenderTarget( hdl ) ;
		if( !rt )
			return false ;

		return false;

#if RENDERDEVICE_D3D9
		Core::Rtti* pRtti = rt->GetRtti() ;

		if( *pRtti != RenderTargetD3D9::RTTI )
			return false ;
		std::shared_ptr<RenderTargetD3D9>	dx9rt = rt.downcast<RenderTargetD3D9>() ;

		if( !dx9rt.isvalid() )
			return false ;

		IDirect3DSurface9* sfs = dx9rt->GetD3DRT() ;

		HRESULT res = D3DXSaveSurfaceToFile( path.Get() , D3DXIFF_TGA , sfs , NULL , NULL ) ;

		return ( res == S_OK ) ;
#endif

	}

	void RenderSystem::_OnDeviceLost()
	{
		RenderBase::RenderSystem::RenderResourceHandleSet::type &handles = RenderSystem::Instance()->GetRenderResourceHandles();
		RenderBase::RenderSystem::RenderResourceHandleSet::type::iterator it = handles.begin();
		while(it != handles.end())
		{
			(*it).mRO.lock()->OnDeviceLost();
			++it;
		}
	}

	void RenderSystem::_OnDeviceReset()
	{
		RenderBase::RenderSystem::RenderResourceHandleSet::type &handles = RenderSystem::Instance()->GetRenderResourceHandles();
		RenderBase::RenderSystem::RenderResourceHandleSet::type::iterator it = handles.begin();

		std::vector< std::shared_ptr<RenderTarget> > rtShareDepth;
		std::vector< std::shared_ptr<RenderTarget> > rtNotShareDepth;

		while(it != handles.end())
		{
			std::shared_ptr<RenderCommandType> rcType = (*it).mRO.lock();// (RenderCommandType*));

			if (typeid(rcType.get()) == typeid(RenderTarget))
			{
				const std::shared_ptr<RenderTarget>& rt = std::static_pointer_cast<RenderTarget>(rcType);
				if (rt->GetSharedDepthStencil()!=nullptr)
				{
					rtShareDepth.push_back(rt);
				}
				else
				{
					rtNotShareDepth.push_back(rt);
				}
			}
			else
			{
				rcType->OnDeviceReset();
			}		
			++it;
		}

		for (IndexT i = 0; i < rtNotShareDepth.size(); ++i)
		{
			rtNotShareDepth[i]->OnDeviceReset();
		}

		for (IndexT i = 0; i < rtShareDepth.size(); ++i)
		{
			rtShareDepth[i]->OnDeviceReset();
		}
	}

	void RenderSystem::SetDeviceLostCallBack(deviceLostCallBackFunc func)
	{
		m_renderDevice->SetDeviceLostCallBack(func);
	}


	void RenderSystem::_OnWindowChanged()
	{
#if RENDERDEVICE_D3D9
		// part this operator
		//m_renderDevice.cast<RenderDeviceD3D9>()->SetDeviceLost();
#endif		
	}

	RenderWindow* RenderSystem::CreateRenderWindow( WindHandle hWnd )
	{
		return m_renderDevice->CreateRenderWindow(hWnd);
	}


	void RenderSystem::DestroyRenderWindow(const std::shared_ptr<RenderWindow>& rw)
	{
		m_renderDevice->DestroyRenderWindow(rw);
	}

	void RenderSystem::OnDeviceLost()
	{
		m_renderDevice->OnDeviceLost();
		_OnDeviceLost();
	}

	bool RenderSystem::CheckReset()
	{
		return m_renderDevice->CheckReset();
	}

	void RenderSystem::OnDeviceReset()
	{
		m_renderDevice->OnDeviceReset();
		_OnDeviceReset();
	}

	bool RenderSystem::LockTexture(TextureHandle texHandle, ubyte* pData, IndexT mipLevel)
	{
		if (!texHandle.IsValid())
		{
			n_warning("RenderSystem::UpdateTexture():texHandle is invalid!\n");
			return NULL;
		}

		std::shared_ptr<RenderCommandType> rcType = texHandle.mRO.lock();
		std::shared_ptr<Texture> destTex = std::static_pointer_cast<Texture>(rcType);
		return m_renderDevice->LockTexture(destTex, pData, mipLevel);
	}

	bool RenderSystem::LockAllTexture(TextureHandle texHandle, ubyte* pData)
	{
		if (!texHandle.IsValid())
		{
			n_warning("RenderSystem::UpdateTexture():texHandle is invalid!\n");
			return NULL;
		}

		std::shared_ptr<RenderCommandType> rcType = texHandle.mRO.lock();
		std::shared_ptr<Texture> destTex = std::static_pointer_cast<Texture>(rcType);
		return m_renderDevice->LockAllTexture(destTex, pData);
	}

	RenderBase::PsoHandle RenderSystem::CreatePso(const RenderBase::PsoData& psoData, const RenderBase::RootSignatureHandle& rs)
	{
		std::shared_ptr<RenderCommandType> rcType = rs.mRO.lock();
		const std::shared_ptr<RootSignature>& root = std::static_pointer_cast<RootSignature>(rcType);
		std::shared_ptr<RenderBase::Pso> pso = m_renderDevice->CreatePso(psoData, root);

		PsoHandle handle;
		handle.mRO = pso;

		bool bOK = _AddRenderHandle(handle);
		assert(bOK);

		return handle;
	}

	RenderBase::RootSignatureHandle RenderSystem::CreateRootSignature(const RenderBase::SignatureInfo& info)
	{
		std::shared_ptr<RootSignature> rs = m_renderDevice->CreateRootSignature(info);
		RootSignatureHandle handle;
		handle.mRO = rs;

		bool bOK = _AddRenderHandle(handle);
		assert(bOK);

		return handle;
	}

	void RenderSystem::ResetCommandList(const RenderBase::PsoHandle& psh)
	{
		std::shared_ptr<RenderCommandType> rcType = psh.mRO.lock();
		const std::shared_ptr<Pso>& pso = std::static_pointer_cast<Pso>(rcType);
		m_renderDevice->ResetCommandList(pso);
	}

	void RenderSystem::SetPsoRootSignature(const RenderBase::PsoHandle& psh, const RenderBase::RootSignatureHandle& rsh)
	{
		std::shared_ptr<RenderCommandType> rcType = psh.mRO.lock();
		const std::shared_ptr<Pso>& pso = std::static_pointer_cast<Pso>(rcType);

		std::shared_ptr<RenderCommandType> rcType2 = rsh.mRO.lock();
		const std::shared_ptr<RootSignature>& root = std::static_pointer_cast<RootSignature>(rcType2);

		m_renderDevice->SetPsoRootSignature(pso, root);
	}

	void RenderSystem::ExecuteCmmdList()
	{
		m_renderDevice->ExecuteCmmdList();
	}

	void RenderSystem::FlushCmmdList()
	{
		m_renderDevice->FlushCmmdList();
	}
}
