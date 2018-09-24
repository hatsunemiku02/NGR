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
#ifndef RENDERSYSTEM_H_
#define RENDERSYSTEM_H_
#include "core/debug.h"
#include "base/RenderDeviceTypes.h"
#include "base/PrimitiveGroup.h"
#include "base/RenderTarget.h"
#include "base/MultipleRenderTarget.h"
#include "base/RenderDevice.h"
#include "base/Texture.h"
#include "util/stl.h"
#include "base/GraphicCardCapability.h"
#include "base/RenderWindow.h"

namespace RenderBase
{
	class ConstantBuffer;

	class RenderSystem 
	{
		
	public:
		static RenderSystem* Instance();

		/// constructor
		RenderSystem();
		/// destructor
		virtual ~RenderSystem();

		void ChangeSize(int width, int height);

		/// open the interface object
		virtual void Open(int width, int height);
		/// close the interface object
		virtual void Close();

		void SetDeviceLostCallBack(deviceLostCallBackFunc func);

		float GetHorizontalTexelOffset();
		float GetVerticalTexelOffset();
		float GetMinimumDepthInputValue();
		float GetMaximumDepthInputValue();

		void SetBlendState(const DeviceBlendState* state, float alphaRef);
		void SetRasterState(const DeviceRasterizerState* state);
		void SetDepthState(const DeviceDepthAndStencilState* state);
		void SetSamplerState(const DeviceSamplerState* state, int stencilRef);

		void SetVertexShaderConstantVectorF(const int& reg, float* val, const int& vec4count);
		void SetPixelShaderConstantVectorF(const int& reg, float* val, const int& vec4count);

		void SetVertexShaderConstantFloat(const int& reg, float* val);
		void SetPixelShaderConstantFloat(const int& reg, float* val);

		void SetVertexShaderConstantMatrixF(const int& reg, float* val, const int& matrixCount);
		void SetPixelShaderConstantMatrixF(const int& reg, float* val, const int& matrixCount);

		void BeginFrame();
		void EndFrame();
		bool IsBeginFrame() const;
		
		void Clear();

		GPUProgramHandle CreateShaderProgram(const std::shared_ptr<GPUProgram>& program);
		void _SetShaderProgram(GPUProgramHandle handle);

		std::weak_ptr<PrimitiveGroup>  GetPrimitiveGroup(const PrimitiveHandle& handle) const;
		std::weak_ptr<Texture> GetTexture(const TextureHandle& handle) const;
		std::weak_ptr<ConstantBuffer> GetConstantBuffer(const ConstanBufferHandle& handle) const;

		ConstanBufferHandle CreateConstantBufferHandle(SizeT nSize, const RenderBase::BufferData& desc);
		void				UpdateConstantBuffer(const ConstanBufferHandle& handle, const DataStream& data);

		void				SetVSConstantBuffer(const ConstanBufferHandle& handle, IndexT bindpoint);
		void				SetPSConstantBuffer(const ConstanBufferHandle& handle, IndexT bindpoint);

		RenderBase::PsoHandle	CreatePso(const RenderBase::PsoData& psoData, const RenderBase::RootSignatureHandle& rs);
		RenderBase::RootSignatureHandle CreateRootSignature(const RenderBase::SignatureInfo& info);

		void SetPsoRootSignature(const RenderBase::PsoHandle& psh, const RenderBase::RootSignatureHandle& rsh);
		void ResetCommandList(const RenderBase::PsoHandle& psh);
		void ExecuteCmmdList();
		void FlushCmmdList();

		PrimitiveHandle CreatePrimitiveHandle(const std::shared_ptr<VertexBufferData>& vbd, const std::shared_ptr<IndexBufferData>& ibd = NULL);
		void ChangePrimitiveHandle(RenderBase::PrimitiveHandle& handle, const std::shared_ptr<RenderBase::VertexBufferData>& vbd, const std::shared_ptr<RenderBase::IndexBufferData>& ibd = NULL);
		void UpdateVertexBuffer(PrimitiveHandle& handle, const DataStream& data);
		void UpdateIndexBuffer(PrimitiveHandle& handle, const DataStream& data);
		
		void _DrawPrimitive(PrimitiveHandle handle,SizeT startVertice,SizeT endVertice,SizeT startIndice,SizeT endIndice);
		void _DrawPrimitive(PrimitiveHandle handle);
		void _DrawPrimitiveHWInstance(PrimitiveHandle handle);
		

		RenderStateDescHandle CreateRenderStateObject( const std::shared_ptr<RenderStateDesc>& rsObject);
		void _SetRenderState(const std::shared_ptr<RenderStateDesc>& rsObject, const uint& shaderMask);

		RenderTargetHandle CreateRenderTarget( const std::shared_ptr<RenderTarget>& rt, TextureHandle& texhandle );
		void _SetRenderTarget(RenderTargetHandle handle,SizeT index,uint clearflag, bool bGammaToLinear = false);
		void _SetRenderTargetClearColor(RenderTargetHandle handle,const Math::float4& clearColor);
		void _ReSizeRenderTarget(RenderTargetHandle handle,const int& width,const int& height);
		void CopyRenderTarget(RenderTargetHandle srcHandle, const Math::float4& srcRect, RenderTargetHandle desHandle, const Math::float4& desRect);
		
		MultipleRenderTargetHandle CreateMultipleRenderTarget(const std::shared_ptr<MultipleRenderTarget>& mrt, std::vector< RenderBase::RenderTargetHandle >& handles);
		void _SetMultipleRenderTarget(MultipleRenderTargetHandle handle, bool resume = false);
		void _ReSizeMultipleRenderTarget(MultipleRenderTargetHandle handle,const int& width,const int& height);

		TextureHandle CreateTexture( const std::shared_ptr<Texture>& tex);
		void _SetTexture(TextureHandle handle,SizeT index);
		void UpdateTexture(TextureHandle texHandle, RenderBase::Texture::UpdateFunction texUpdateFunc, void* tag);
		void UpdateTexture(TextureHandle texHandle, std::shared_ptr<RenderBase::Texture> texture);
		void ChangeTexture(RenderBase::TextureHandle texHandle, std::shared_ptr<Texture> texture);

		void _RemoveResouce(const RenderResourceHandle& handle);

		void _SetDisplayMode(const DisplayMode& mode);
		void _SetViewPort(const int& x,const int& y,const int& width,const int& height,const float& minZ,const float& maxZ);

		//fixed pipeline functions
		void _FXSetClipPlane(const int& index,const Math::float4& plane);

		const GraphicCardCapability& GetGraphicCardCapability();

		std::shared_ptr<RenderTarget>	GetRenderTarget( RenderTargetHandle ) ;
		bool OutPutRenderTarget( RenderTargetHandle , const std::string& path ) ;

		void _OnWindowChanged();
		
		RenderWindow* CreateRenderWindow( WindHandle hWnd );

		void DestroyRenderWindow(const std::shared_ptr<RenderWindow>& rw);

		void SetWireFrameMode(bool wireframe = false);

		void OnDeviceLost();

		bool CheckReset();

		void OnDeviceReset();

		void SetWriteSRGB(bool bEnable);
		void SetSRGBRead(TextureHandle texHandle, bool bRead);
		bool LockTexture(TextureHandle texHandle, ubyte* pData, IndexT mipLevel = 0);
		bool LockAllTexture(TextureHandle texHandle, ubyte* pData);
		void Resolve(TextureHandle texHandle);
	protected:
		bool _AddRenderHandle( const RenderBase::RenderResourceHandle& handle);
		bool _RemoveRenderHandle( const RenderBase::RenderResourceHandle& handle);
		bool _HasRenderHandle(const RenderBase::RenderResourceHandle& handle);
	private:
		static RenderSystem* m_pInstance;

		friend class RenderSystemThreadHandler;

		std::shared_ptr<RenderDevice> m_renderDevice;
		std::shared_ptr<RenderDisplay> m_renderDisplay;
		RenderBase::RenderTargetHandle m_dummyRenderTargetHandle;

		typedef Util::STL_set<RenderResourceHandle> RenderResourceHandleSet;
		RenderResourceHandleSet::type m_renderHandles;

		void _OnDeviceLost();
		void _OnDeviceReset();
	private:
		RenderResourceHandleSet::type& GetRenderResourceHandles();

		WindHandle m_mainHWND;
		
	public:
		void SetMainWindowHandle(WindHandle hwnd);
		WindHandle GetMainWindowHandle(void)const;

	};

	inline void RenderSystem::ChangeSize(int width, int height)
	{
		m_renderDevice->SetSize(width, height);
	}

	inline const GraphicCardCapability& RenderSystem::GetGraphicCardCapability()
	{
		assert(m_renderDevice);
		return m_renderDevice->GetGraphicCardCapability();
	}

	extern RenderSystem* rendersystem;
	inline RenderSystem& GetRenderSystem()
	{
		assert( rendersystem );
		return *rendersystem;
	}

	inline RenderSystem::RenderResourceHandleSet::type& RenderSystem::GetRenderResourceHandles()
	{
		return m_renderHandles;
	}

	inline void RenderSystem::SetWireFrameMode(bool wireframe /* = false */)
	{
		m_renderDevice->SetDrawWireFrame(wireframe);
	}

	inline float RenderSystem::GetHorizontalTexelOffset()
	{
		return m_renderDevice->GetHorizontalTexelOffset();
	}
	inline float RenderSystem::GetVerticalTexelOffset()
	{
		return m_renderDevice->GetVerticalTexelOffset();
	}
	inline float RenderSystem::GetMinimumDepthInputValue()
	{
		return m_renderDevice->GetMinimumDepthInputValue();
	}
	inline float RenderSystem::GetMaximumDepthInputValue()
	{
		return m_renderDevice->GetMinimumDepthInputValue();
	}
	inline void RenderSystem::SetWriteSRGB(bool bEnable)
	{
		m_renderDevice->SetWriteSRGB(bEnable);
	}
}

#endif //RENDERSYSTEM_H_
