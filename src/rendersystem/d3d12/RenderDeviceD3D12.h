#pragma once

#include "../config/RenderDeviceConfig.h"
#include "../base/RenderDisplay.h"
#include "../base/RenderDeviceTypes.h"
#include "../base/RenderDevice.h"


#include "core/types.h"

#if RENDERDEVICE_D3D12


#include "D3D12Heads.h"

namespace RenderBase
{
	class ConstantBuffer;
	class VertexLayout;
	class PrimitiveGroup;
	class IndexBuffer;
	class GPUProgram;
	class RenderTarget;
	class RootSignature;
}

namespace D3D12
{
	using namespace RenderBase;
	#define MAX_TEXTURE_COUNT 16
	class DescriptorHeap;
	class RTVHeap;
	class DSVHeap;
	class CSUHeap;
	class GraphicCommandList;
	class PrimitiveGroupD3D12;
	class RenderTargetD3D12;
	class TextureD3D12;
	class DX12RootSignature;

class RenderDeviceD3D12 : public RenderDevice
{
public:
	static RenderDeviceD3D12* Instance();
	RenderDeviceD3D12();


	virtual ~RenderDeviceD3D12();

	virtual void DetectGraphicCardCaps();
	virtual const GraphicCardCapability& GetGraphicCardCapability();
// 	virtual RenderWindow* CreateRenderWindow(WindHandle wnd);
// 	virtual void DestroyRenderWindow(const std::shared_ptr<RenderWindow>& rw);
// 	virtual void SetRenderTarget(const std::shared_ptr<RenderTarget>& rt, bool bGammaToLinear = false);
// 	virtual void SetMultipleRenderTarget(const std::shared_ptr<MultipleRenderTarget>& mrt, bool resume = false);
// 	virtual void DisableRenderTarget(int index);
// 	virtual void SetViewPort(const int& x,const int& y,const int& width,const int& height,const float& minZ,const float& maxZ);
// 	virtual void FXSetClipPlane(const int& index,const Math::float4& plane) {}
	virtual bool OnDeviceLost();
	virtual bool CheckReset();
	virtual bool OnDeviceReset();
	virtual void SetWriteSRGB(bool bEnable) {}

	virtual bool InitDevice();

	/// begin complete frame
// 	virtual bool BeginFrame();
// 
// 	/// end current frame
// 	virtual void EndFrame();

// 	void CreateCommandList();
// 
// 	/// set current vertex layout
// 	virtual void SetVertexLayout(const std::shared_ptr<VertexLayout>& vl);
// 	/// set current index buffer
// 	virtual void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& ib);
// 	/// set current primitive group
// 	virtual void SetPrimitiveGroup(const std::shared_ptr<PrimitiveGroup>& pg);
// 	/// set current GPU program
// 	virtual void SetGPUProgram(const std::shared_ptr<GPUProgram>& program);
// 	/// draw current primitive
// 	virtual void Draw(SizeT startVertice,SizeT numVertice,SizeT startIndice,SizeT numIndice);
// 
// 	virtual std::shared_ptr<ConstantBuffer> CreateConstantBuffer(SizeT nSize, const RenderBase::BufferData& desc);
// 	virtual void UpdateConstantBuffer(const std::shared_ptr<ConstantBuffer>& pBuffer, const DataStream& data);
// 
// 	/// create Render side Primitive Group
// 	virtual std::shared_ptr<PrimitiveGroup> CreatePrimitiveGroup(const std::shared_ptr<VertexBufferData>& vbd, const std::shared_ptr<IndexBufferData>& ibd);
// 	virtual void ChangePrimitiveGroup(const std::shared_ptr<PrimitiveGroup>& group, const std::shared_ptr<RenderBase::VertexBufferData>& vbd, const std::shared_ptr<RenderBase::IndexBufferData>& ibd = nullptr);
// 	virtual void UpdateVertexBuffer(const std::shared_ptr<PrimitiveGroup>& pg, const DataStream& data);
// 	virtual void UpdateIndexBuffer(const std::shared_ptr<PrimitiveGroup>& pg, const DataStream& data);
// 
// 	/// create render side GPU program
// 	const std::shared_ptr<GPUProgram> CreateRenderGPUProgram(const std::shared_ptr<GPUProgram>& srcGPUProgram);
// 	/// create render side RenderState
// 	const std::shared_ptr<RenderStateDesc> CreateRenderState(const std::shared_ptr<RenderStateDesc>& state);
// 	/// create render side Texture
// 	const std::shared_ptr<Texture> CreateRenderSideTexture(const std::shared_ptr<Texture>& tex);
// 
// 	virtual std::shared_ptr<RenderBase::RootSignature> CreateRootSignature(const SignatureInfo& info);
// 	virtual std::shared_ptr<Pso>			CreatePso(const RenderBase::PsoData& info, const std::shared_ptr<RenderBase::RootSignature>& pSignature);
// 
// 	/// create render side RenderTarget
// 	const std::shared_ptr<RenderTarget> CreateRenderTarget(const std::shared_ptr<RenderTarget>& tex);
// 	/// create render side MultipleRenderTarget
// 	const std::shared_ptr<MultipleRenderTarget> CreateMultipleRenderTarget(const std::shared_ptr<MultipleRenderTarget>& mrt);
// 
// 	virtual void ChangeTexture(std::shared_ptr<Texture> srcTex, std::shared_ptr<Texture> destTex);
// 	/// update render side Texture
// 	virtual void UpdateTexture(RenderBase::Texture::UpdateFunction texUpdateFunc,std::shared_ptr<Texture> destTex, void* tag);
// 	/// update render side Texutre by texture data
// 	virtual void UpdateTexture(std::shared_ptr<Texture> srcTex, std::shared_ptr<Texture> destTex);
// 
// 	virtual void SetVertexShaderConstantVectorF(const int& reg, float* val, const int& vec4count) {}
// 	virtual void SetPixelShaderConstantVectorF(const int& reg, float* val, const int& vec4count) {}
// 
// 	virtual void SetVertexShaderConstantFloat(const int& reg, float* val) {}
// 	virtual void SetPixelShaderConstantFloat(const int& reg, float* val) {}
// 
// 	virtual void SetVertexShaderConstantMatrixF(const int& reg, float* val, const int& matrixCount) {}
// 	virtual void SetPixelShaderConstantMatrixF(const int& reg, float* val, const int& matrixCount) {}
// 
// 	virtual  void SetVSConstantBuffer(const std::shared_ptr<ConstantBuffer>& pBuffer, IndexT bindPoint);
// 	virtual  void SetPSConstantBuffer(const std::shared_ptr<ConstantBuffer>& pBuffer, IndexT bindPoint);
// 
// 	virtual void SetTexture(const std::shared_ptr<Texture>& tex);
// 
// 	virtual void SetBlendState(const DeviceBlendState& state) {}
// 	virtual void SetRasterState(const DeviceRasterizerState& state) {}
// 	virtual void SetDepthAndStencilState(const DeviceDepthAndStencilState& state) {}
// 	virtual void SetTextureSamplerState(const DeviceSamplerState& state) {}

	virtual float GetHorizontalTexelOffset();
	virtual float GetVerticalTexelOffset();
	virtual float GetMinimumDepthInputValue();
	virtual float GetMaximumDepthInputValue();

// 	/// check if inside BeginFrame
// 	virtual void Present(WindHandle hwnd);
// 
// 	void SetSwapChain(const RenderBase::RenderTargetHandle& handle);
// 
// 	virtual void ExecuteCmmdList();
// 	virtual void FlushCmmdList();
// 
// 	virtual void ResetCommandList(const std::shared_ptr<Pso>& pso);
// 	virtual void SetPsoRootSignature(const std::shared_ptr<Pso>& pso, const std::shared_ptr<RenderBase::RootSignature>& rs);

	ID3D12Device* GetDevice() const;

	IDXGIFactory4* GetDXGI() const;

 	ID3D12CommandQueue* GetCmdQueue() const;
 	ID3D12CommandAllocator* GetCmdAllocator() const;
// 	void SetMainWindowHandle(WindHandle hwnd);
// 	WindHandle GetWindowHandle() const;

	DescriptorHeap*		GetRtvHeap() const;
	DescriptorHeap*		GetDsvHeap() const;
	DescriptorHeap*		GetCsuHeap() const;

//	GraphicCommandList* GetGraphicQueue() const;

	void ClearCache();

	static SizeT s_MaxSwapChainBufferCount;

	uint	GetAAQuality() const;
	uint	GetAAType()	const;

protected:

	struct D3D12RenderStateCache
	{
		std::weak_ptr<PrimitiveGroupD3D12> primitiveCache;
		std::weak_ptr<TextureD3D12>  textureCache[MAX_TEXTURE_COUNT];
		D3D12RenderStateCache();
		void ClearCache();
	};

private:

	ID3D12Device*               m_pDevice12;
	IDXGISwapChain*				m_pCurSwapChain;
	IDXGIFactory4*				m_pDxgiFactory;

	
	DescriptorHeap*				m_pRtvHeap;
	DescriptorHeap*				m_pDsvHeap;
	DescriptorHeap*				m_pCsuHeap;

	ID3D12CommandAllocator*		m_pCommandAllocator;
	ID3D12CommandQueue*			m_pCommandQueue;

	uint						m_CurAAQuality;
	uint						m_CurAAType;

	GraphicCardCapability		m_graphicCardCaps;

	static RenderDeviceD3D12*  m_spInstance;

	// 	GraphicCommandList*		m_pGraphicCommandQueue;
	// 	std::weak_ptr<PrimitiveGroupD3D12>	m_primitiveGroupD12;
	// 	D3D12RenderStateCache		m_stateCache;
	// 	std::vector< std::shared_ptr<RenderTargetD3D12> > m_pRenderTargets;
	// 	uint						m_nCurrBackBuffer;
	// 	DX12RootSignature*			m_pCurrRootSig;
	// 
	// 	std::weak_ptr<RenderTargetD3D12> m_pCurrBackBuffer;
	// 
	// 	WindHandle				    m_hMainWindow;
};

inline ID3D12Device* RenderDeviceD3D12::GetDevice() const
{
	return m_pDevice12;
}

inline IDXGIFactory4* RenderDeviceD3D12::GetDXGI() const
{
	return m_pDxgiFactory;
}

// inline void RenderDeviceD3D12::SetMainWindowHandle(WindHandle hwnd)
// {
// 	m_hMainWindow = (HWND)hwnd;
// }
// 
// inline WindHandle RenderDeviceD3D12::GetWindowHandle() const
// {
// 	return m_hMainWindow;
// }

inline DescriptorHeap*	RenderDeviceD3D12::GetRtvHeap() const
{
	return m_pRtvHeap;
}

inline DescriptorHeap*	RenderDeviceD3D12::GetDsvHeap() const
{
	return m_pDsvHeap;
}

inline DescriptorHeap*	RenderDeviceD3D12::GetCsuHeap() const
{
	return m_pCsuHeap;
}

inline ID3D12CommandQueue* RenderDeviceD3D12::GetCmdQueue() const
{
	return m_pCommandQueue;
}

inline ID3D12CommandAllocator* RenderDeviceD3D12::GetCmdAllocator() const
{
	return m_pCommandAllocator;
}
// 
// inline GraphicCommandList* RenderDeviceD3D12::GetGraphicQueue() const
// {
// 	return m_pGraphicCommandQueue;
// }
// 
inline const GraphicCardCapability&  RenderDeviceD3D12::GetGraphicCardCapability()
{
	return m_graphicCardCaps;
}

inline uint RenderDeviceD3D12::GetAAQuality() const
{
	return m_CurAAQuality;
}

inline uint RenderDeviceD3D12::GetAAType() const
{
	return m_CurAAType;
}

}

#endif