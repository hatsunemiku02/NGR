#ifndef RENDERDEVICE_H_
#define RENDERDEVICE_H_
#include "../config/RenderDeviceConfig.h"
#include "../base/RenderDisplay.h"
#include "../base/BufferData.h"
#include "../base/RenderDeviceTypes.h"
#include "../base/GraphicCardCapability.h"
#include "core/types.h"
#include "VertexBuffer.h"
#include "VertexLayout.h"
#include "IndexBuffer.h"
#include "PrimitiveGroup.h"
#include "RenderTarget.h"
#include "MultipleRenderTarget.h"
#include "GPUProgram.h"
#include "Texture.h"
#include "RootSignature.h"
#include "Pso.h"
#include "RenderStateDesc.h"

#ifdef __ANDROID__

#ifdef _DEBUG

#include <typeinfo>

#endif 

#endif 

namespace RenderBase
{
typedef void (*deviceLostCallBackFunc)();
class RenderWindow;
class ConstantBuffer;


class RenderDevice 
{
public:
	
	RenderDevice();
	virtual ~RenderDevice();

	virtual bool InitDevice() = 0;

	/** Returns the horizontal texel offset value required for mapping 
	texel origins to pixel origins in this rendersystem.
	@remarks
	Since rendersystems sometimes disagree on the origin of a texel, 
	mapping from texels to pixels can sometimes be problematic to 
	implement generically. This method allows you to retrieve the offset
	required to map the origin of a texel to the origin of a pixel in
	the horizontal direction.
	*/
	virtual float GetHorizontalTexelOffset() = 0;
	/** Returns the vertical texel offset value required for mapping 
	texel origins to pixel origins in this rendersystem.
	@remarks
	Since rendersystems sometimes disagree on the origin of a texel, 
	mapping from texels to pixels can sometimes be problematic to 
	implement generically. This method allows you to retrieve the offset
	required to map the origin of a texel to the origin of a pixel in
	the vertical direction.
	*/
	virtual float GetVerticalTexelOffset() = 0;

	/** Gets the minimum (closest) depth value to be used when rendering
	using identity transforms.
	@remarks
	When using identity transforms you can manually set the depth
	of a vertex; however the input values required differ per
	rendersystem. This method lets you retrieve the correct value.
	@see Renderable::getUseIdentityView, Renderable::getUseIdentityProjection
	*/
	virtual float GetMinimumDepthInputValue() = 0;
	/** Gets the maximum (farthest) depth value to be used when rendering
	using identity transforms.
	@remarks
	When using identity transforms you can manually set the depth
	of a vertex; however the input values required differ per
	rendersystem. This method lets you retrieve the correct value.
	@see Renderable::getUseIdentityView, Renderable::getUseIdentityProjection
	*/
	virtual float GetMaximumDepthInputValue() = 0;

	SizeT GetNumTriangles();

	SizeT GetNumVertexs();

	SizeT GetNumDrawCalls();

	SizeT GetNumRenderTargets();

	// 测试当前硬件的性能，需要在设备初始化之后立刻调用
	virtual void DetectGraphicCardCaps() = 0;

	// 获取当前硬件的性能参数
	virtual const GraphicCardCapability& GetGraphicCardCapability() = 0;

// 	virtual RenderWindow* CreateRenderWindow(WindHandle wnd) = 0;
// 
// 	virtual void DestroyRenderWindow(const std::shared_ptr<RenderWindow>& rw) = 0;
	
	/// Create true primitive group
// 	virtual std::shared_ptr<PrimitiveGroup> CreatePrimitiveGroup(const std::shared_ptr<VertexBufferData>& vbd, const std::shared_ptr<IndexBufferData>& ibd) = 0;
// 	virtual void ChangePrimitiveGroup(const std::shared_ptr<PrimitiveGroup>& group,const std::shared_ptr<RenderBase::VertexBufferData>& vbd, const std::shared_ptr<RenderBase::IndexBufferData>& ibd = nullptr) = 0;
// 	virtual void UpdateVertexBuffer(const std::shared_ptr<PrimitiveGroup>& pg, const DataStream& data) = 0;
// 	virtual void UpdateIndexBuffer(const std::shared_ptr<PrimitiveGroup>& pg, const DataStream& data) = 0;
// 
// 	virtual void ChangeTexture(std::shared_ptr<Texture> srcTex, std::shared_ptr<Texture> destTex) = 0;
// 	virtual void UpdateTexture(RenderBase::Texture::UpdateFunction texUpdateFunc,std::shared_ptr<Texture> destTex, void* tag) = 0;
// 	virtual void UpdateTexture(std::shared_ptr<Texture> srcTex, std::shared_ptr<Texture> destTex) = 0;
// 	virtual const std::shared_ptr<GPUProgram> CreateRenderGPUProgram(const std::shared_ptr<GPUProgram>& srcGPUProgram) = 0;
// 	virtual const std::shared_ptr<RenderStateDesc> CreateRenderState(const std::shared_ptr<RenderStateDesc>& state) = 0;
// 	virtual const std::shared_ptr<Texture> CreateRenderSideTexture(const std::shared_ptr<Texture>&  tex) = 0;
// 	virtual const std::shared_ptr<RenderTarget> CreateRenderTarget(const std::shared_ptr<RenderTarget>& rt) = 0;
// 	virtual const std::shared_ptr<MultipleRenderTarget> CreateMultipleRenderTarget(const std::shared_ptr<MultipleRenderTarget>& mrt) = 0;
// 
// 	virtual void SetBlendState(const DeviceBlendState& state) = 0;
// 	virtual void SetRasterState(const DeviceRasterizerState& state) = 0;
// 	virtual void SetDepthAndStencilState(const DeviceDepthAndStencilState& state) = 0;
// 	virtual void SetTextureSamplerState(const DeviceSamplerState& state) = 0;
// 	virtual void SetTexture(const  std::shared_ptr<Texture>& tex) = 0;
// 	virtual void SetRenderTarget(const std::shared_ptr<RenderTarget>& rt, bool bGammaToLinear = false) = 0;
// 	virtual void SetMultipleRenderTarget(const std::shared_ptr<MultipleRenderTarget>& mrt, bool resume = false) = 0;
// 	virtual void DisableRenderTarget(int index) = 0;


// 	virtual std::shared_ptr<RootSignature> CreateRootSignature(const SignatureInfo& info);
// 	virtual std::shared_ptr<Pso>			CreatePso(const PsoData& info, const std::shared_ptr<RootSignature>& rs);
// 
// 	virtual void SetVertexShaderConstantVectorF(const int& reg, float* val, const int& vec4count) = 0;
// 	virtual void SetPixelShaderConstantVectorF(const int& reg, float* val, const int& vec4count) = 0;
// 
// 	virtual void SetVertexShaderConstantFloat(const int& reg, float* val) = 0;
// 	virtual void SetPixelShaderConstantFloat(const int& reg, float* val) = 0;
// 
// 	virtual void SetVertexShaderConstantMatrixF(const int& reg, float* val, const int& matrixCount) = 0;
// 	virtual void SetPixelShaderConstantMatrixF(const int& reg, float* val, const int& matrixCount) = 0;
// 
// 	virtual void ResetCommandList(const std::shared_ptr<Pso>& pso);
// 	virtual void SetPsoRootSignature(const std::shared_ptr<Pso>& pso, const std::shared_ptr<RootSignature>& rs);
			
// 	/// begin complete frame
// 	virtual bool BeginFrame();
// 	/// end current frame
// 	virtual void EndFrame();
// 	bool IsBeginFrame() const;

	virtual void ExecuteCmmdList();
	virtual void FlushCmmdList();

// 	/// set the current vertex stream source
// 	virtual void SetStreamSource(IndexT streamIndex, const VertexBuffer* vb, IndexT offsetVertexIndex);
// 	/// set the current vertex stream source Frequency 
// 	//virtual void SetStreamSourceFreq(IndexT streamIndex, IndexT setting);
// 	/// set current vertex layout
// 	virtual void SetVertexLayout(const std::shared_ptr<VertexLayout>& vl) = 0;
// 	/// set current index buffer
// 	virtual void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& ib) = 0;
// 	/// set current primitive group
// 	virtual void SetPrimitiveGroup(const std::shared_ptr<PrimitiveGroup>& pg) = 0;
// 	/// set current GPU program
// 	virtual void SetGPUProgram(const std::shared_ptr<GPUProgram>& program) = 0;
// 	/// draw current primitives
// 	virtual void Draw(SizeT startVertice,SizeT endVertice,SizeT startIndice,SizeT endIndice) = 0;
// 	/// draw indexed, instanced primitives
// 	virtual void DrawIndexedInstanced(SizeT numInstances);
// 
// 	/// present the rendered scene
// 	virtual void Present(WindHandle hwnd ) = 0;
// 	/// set viewport
// 	virtual void SetViewPort(const int& x,const int& y,const int& width,const int& height,const float& minZ,const float& maxZ) = 0;
// 	
// 	//////////////////////////////////////////////////////////////////////////
// 	//Fixed Pipeline Functions
// 	virtual void FXSetClipPlane(const int& index,const Math::float4& plane) = 0;

	virtual bool OnDeviceLost() = 0;

	virtual bool CheckReset() = 0;

	virtual bool OnDeviceReset() = 0;

// 	virtual bool LockTexture(const std::shared_ptr<Texture>& tex, ubyte* pData, IndexT mipLevel = 0);
// 	virtual bool LockAllTexture(const std::shared_ptr<Texture>& tex, ubyte* pData);
// 
// 	virtual std::shared_ptr<ConstantBuffer> CreateConstantBuffer(SizeT nSize, const RenderBase::BufferData& desc);
// 	virtual void UpdateConstantBuffer(const std::shared_ptr<ConstantBuffer>& pBuffer, const DataStream& data);
// 	virtual void SetVSConstantBuffer(std::weak_ptr<ConstantBuffer> pBuffer, IndexT bindPoint);
// 	virtual void SetPSConstantBuffer(std::weak_ptr<ConstantBuffer> pBuffer, IndexT bindPoint);

	//////////////////////////////////////////////////////////////////////////
	//Set DrawWireFrame
	void SetDrawWireFrame(bool wireFrame = false);

	void SetSize(int width, int height);

	int GetWidth() const;
	int GetHeight() const;

	bool IsDeviceLost();
	void SetDeviceLost();

	virtual void SetWriteSRGB(bool bEnable) = 0;

	void SetDeviceLostCallBack(deviceLostCallBackFunc func);

// 	template<typename From, typename To>
// 	static To* _Convert(From* value);
// 
// 	template<typename From, typename To>
// 	static const To* _Convert(const From* value);
// 	protected:

	const VertexBuffer* streamVertexBuffers[MaxNumVertexStreams];
	IndexT streamVertexOffsets[MaxNumVertexStreams];
	bool m_bDrawWireFrame;
	int m_width;
	int m_height;
	bool m_bDeviceLost;
	bool m_bBeginFrame;
	deviceLostCallBackFunc m_deviceCallBack;
};

	inline void RenderDevice::SetSize(int width, int height)
	{
		this->m_width = width;
		this->m_height = height;
	}

	inline int RenderDevice::GetWidth() const
	{
		return m_width;
	}

	inline int RenderDevice::GetHeight() const
	{
		return m_height;
	}

	inline
		void RenderDevice::SetDrawWireFrame(bool wireFrame)
	{
		m_bDrawWireFrame = wireFrame;
	}

	inline bool RenderDevice::IsDeviceLost()
	{
		return m_bDeviceLost;
	}

	inline void RenderDevice::SetDeviceLost()
	{
		m_bDeviceLost = true;
	}

	inline void RenderDevice::SetDeviceLostCallBack(deviceLostCallBackFunc func)
	{
		m_deviceCallBack = func;
	}

// 	template<typename From, typename To>
// 	inline 
// 		const To* RenderDevice::_Convert(const From* value)
// 	{
// #ifdef _DEBUG
// 		const To* temp = dynamic_cast<const To*>(value);
// 		if (NULL == temp)
// 		{
// 			n_error("Type Error: the value is not %s", typeid(To).name());
// 		}
// 		return temp;
// #else
// 		return static_cast<const To*>(value);
// #endif // _DEBUG
// 	}

}
#endif
