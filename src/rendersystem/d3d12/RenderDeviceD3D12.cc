#include "stdneb.h"
#include "RenderDeviceD3D12.h"


#if RENDERDEVICE_D3D12

#include "DX12Pso.h"
#include "DX12RootSignature.h"
#include "IndexBufferD3D12.h"
#include "VertexBufferD3D12.h"
#include "RenderTargetD3D12.h"
#include "TextureD3D12.h"
#include "PrimitiveGroupD3D12.h"
#include "DX12ConstantBuffer.h"
#include "CommandQueue.h"
#include "DescriptorHeap.h"
#include "D3D12Window.h"
#include "D3D12Types.h"
#include <wrl.h>
using Microsoft::WRL::ComPtr;

namespace D3D12
{

SizeT RenderDeviceD3D12::s_MaxSwapChainBufferCount = 2;

D3D12::RenderDeviceD3D12* RenderDeviceD3D12::m_spInstance = nullptr;

const SizeT MRT_COUNT = 4;

D3D12::RenderDeviceD3D12* RenderDeviceD3D12::Instance()
{
	return m_spInstance;
}

RenderDeviceD3D12::RenderDeviceD3D12()
	: m_pDevice12(NULL),
	  m_pDxgiFactory(NULL),
	  m_pRtvHeap(NULL),
	  m_pDsvHeap(NULL),
	  m_pCsuHeap(NULL)
// 	  m_pGraphicCommandQueue(NULL),
// 	  m_nCurrBackBuffer(0),
// 	  m_pCurrBackBuffer(),
// 	  m_pCurrRootSig(NULL)
{
//	m_pRenderTargets.resize(MRT_COUNT+1, 0);

	m_spInstance = this;
}

RenderDeviceD3D12::~RenderDeviceD3D12()
{
	FlushCmmdList();

//	delete m_pGraphicCommandQueue;
	delete m_pRtvHeap;
	delete m_pRtvHeap;
	delete m_pCsuHeap;

	if (m_pDevice12 != NULL)
	{
		m_pDevice12->Release();
	}
}

RenderDeviceD3D12::D3D12RenderStateCache::D3D12RenderStateCache()
{
	ClearCache();
}

void RenderDeviceD3D12::D3D12RenderStateCache::ClearCache()
{
	
	primitiveCache.reset();
	for (IndexT i = 0; i < MAX_TEXTURE_COUNT; ++i)
	{
		textureCache[i].reset();
	}
}

_Use_decl_annotations_
void GetHardwareAdapter(IDXGIFactory2* pFactory, IDXGIAdapter1** ppAdapter)
{
	ComPtr<IDXGIAdapter1> adapter;
	*ppAdapter = nullptr;

	for (UINT adapterIndex = 0; DXGI_ERROR_NOT_FOUND != pFactory->EnumAdapters1(adapterIndex, &adapter); ++adapterIndex)
	{
		DXGI_ADAPTER_DESC1 desc;
		adapter->GetDesc1(&desc);

		if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
		{
			// Don't select the Basic Render Driver adapter.
			// If you want a software adapter, pass in "/warp" on the command line.
			continue;
		}

		// Check to see if the adapter supports Direct3D 12, but don't create the
		// actual device yet.
		if (SUCCEEDED(D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_11_0, _uuidof(ID3D12Device), nullptr)))
		{
			break;
		}
	}

	*ppAdapter = adapter.Detach();
}

bool RenderDeviceD3D12::InitDevice()
{
 	UINT dxgiFactoryFlags = 0;
 #if _DEBUG
 	{
 		ComPtr<ID3D12Debug> debugController;
 		if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
 		{
 			debugController->EnableDebugLayer();
 
 			// Enable additional debug layers.
 			dxgiFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
 		}
 	}
 #endif
 	ComPtr<IDXGIFactory4> factory;
 	HRESULT hr = CreateDXGIFactory2(dxgiFactoryFlags,IID_PPV_ARGS(&factory));
 	if (FAILED(hr))
 	{
 		assert(false);
 	}
 	m_pDxgiFactory = factory.Get();
 	ComPtr<IDXGIAdapter1> hardwareAdapter;
 	GetHardwareAdapter(factory.Get(), &hardwareAdapter);
 	ComPtr<ID3D12Device> lvd ;
 	hr = D3D12CreateDevice(hardwareAdapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&lvd));
 	m_pDevice12 = lvd.Get();
 	if (FAILED(hr))
 	{
 		assert(false);
 	}

	D3D12_COMMAND_QUEUE_DESC desc;
	desc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	desc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
	desc.NodeMask = 0;
	hr = m_pDevice12->CreateCommandQueue(&desc, IID_PPV_ARGS(&m_pCommandQueue));
	if (FAILED(hr))
	{
		assert(false);
	}

	hr = m_pDevice12->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_pCommandAllocator));
	if (FAILED(hr))
	{
		assert(false);
	}

	m_pRtvHeap = new RTVHeap();
	m_pDsvHeap = new DSVHeap();
	m_pCsuHeap = new CSUHeap();

	m_pRtvHeap->Init(50);
	m_pDsvHeap->Init(20);
	m_pCsuHeap->Init(100);

	return true;
}

// void RenderDeviceD3D12::ResetCommandList(const std::shared_ptr<Pso>& pso)
// {
// 	const std::shared_ptr<DX12Pso>& ps = std::static_pointer_cast<DX12Pso>(pso);
// 	HRESULT hr = m_pGraphicCommandQueue->GetCommandList()->Reset(m_pCommandAllocator, ps->GetPso());
// 	if (FAILED(hr))
// 	{
// 		assert(false);
// 	}
// }
// 
// void RenderDeviceD3D12::SetPsoRootSignature(const std::shared_ptr<Pso>& pso, const std::shared_ptr<RenderBase::RootSignature>& rs)
// {
// 	const std::shared_ptr<DX12Pso>& ps = std::static_pointer_cast<DX12Pso>(pso);
// 	const std::shared_ptr<DX12RootSignature>& root = std::static_pointer_cast<DX12RootSignature>(rs);
// 	m_pCurrRootSig = root.get();
// 	m_pGraphicCommandQueue->GetCommandList()->SetGraphicsRootSignature(root->GetRootSignature());
// 	m_pGraphicCommandQueue->GetCommandList()->SetPipelineState(ps->GetPso());
// 	m_pGraphicCommandQueue->IncCmmdNum();
// }
// 
// bool RenderDeviceD3D12::BeginFrame()
// {
// 	FlushCmmdList();
// 
// 	ID3D12DescriptorHeap* descriptorHeaps[] = { m_pCsuHeap->GetHeap() };
// 	m_pGraphicCommandQueue->GetCommandList()->SetDescriptorHeaps(_countof(descriptorHeaps), descriptorHeaps);
// 
// 
// 	return RenderDevice::BeginFrame();
// }
// 
// void RenderDeviceD3D12::EndFrame()
// {
// 	for(int i = 0; i < MRT_COUNT; ++i)
// 	{
// 		if (m_pRenderTargets[i])
// 		{
// 			m_pRenderTargets[i] = NULL;
// 		}
// 	}
// 
// 	m_pGraphicCommandQueue->Flush();
// 
// 	RenderDevice::EndFrame();
// }
// 
// void RenderDeviceD3D12::CreateCommandList()
// {
// 	ID3D12GraphicsCommandList* pCommandList;
// 	HRESULT	hr = m_pDevice12->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_pCommandAllocator, NULL, IID_PPV_ARGS(&pCommandList));
// 	if (FAILED(hr))
// 	{
// 		assert(false);
// 	}
// 
// }
// 
// void RenderDeviceD3D12::SetVertexLayout(const std::shared_ptr<VertexLayout>& vl)
// {
// 	
// }
// 
// void RenderDeviceD3D12::SetIndexBuffer(const std::shared_ptr<IndexBuffer>& ib)
// {
// 	assert(ib);
// 	std::shared_ptr<IndexBufferD3D12> pIB = std::dynamic_pointer_cast<IndexBufferD3D12>(ib);
// 
// 	m_pGraphicCommandQueue->GetCommandList()->IASetIndexBuffer(&pIB->GetView());
// 	m_pGraphicCommandQueue->IncCmmdNum();
// }
// 
// void RenderDeviceD3D12::SetPrimitiveGroup(const std::shared_ptr<PrimitiveGroup>& pg)
// {
// 	
// 	this->m_primitiveGroupD12 = std::dynamic_pointer_cast<PrimitiveGroupD3D12>(pg);
// 	std::shared_ptr<PrimitiveGroupD3D12> pg_sjared = m_primitiveGroupD12.lock();
// 	if ( m_stateCache.primitiveCache.lock() != pg_sjared)
// 	{
// 		m_stateCache.primitiveCache = m_primitiveGroupD12;
// 		
// 		const std::shared_ptr<VertexBufferD3D12> pVB12 = pg_sjared->GetVertexBuffer();
// 		m_pGraphicCommandQueue->GetCommandList()->IASetVertexBuffers(0, 1, &pVB12->GetView());
// 		m_pGraphicCommandQueue->IncCmmdNum();
// 		if (pg_sjared->GetIndexBuffer())
// 		{
// 			SetIndexBuffer(pg_sjared->GetIndexBuffer());
// 		}	
// 	}
// }
// 
// void RenderDeviceD3D12::SetGPUProgram(const std::shared_ptr<GPUProgram>& program)
// {
// 	
// }
// 
// std::shared_ptr<PrimitiveGroup> RenderDeviceD3D12::CreatePrimitiveGroup(const std::shared_ptr<VertexBufferData>& vbd, const std::shared_ptr<IndexBufferData>& ibd)
// {
// 	assert(NULL != vbd);
// 	std::shared_ptr<PrimitiveGroupD3D12> prim = std::make_shared< PrimitiveGroupD3D12>();
// 	//prim->LoadBuffers(vbd, ibd);
// 	return prim;
// }
// 
// void RenderDeviceD3D12::ChangePrimitiveGroup(const std::shared_ptr<PrimitiveGroup>& group, const std::shared_ptr<RenderBase::VertexBufferData>& vbd, const std::shared_ptr<RenderBase::IndexBufferData>& ibd)
// {
// 	std::shared_ptr<PrimitiveGroupD3D12> pg12 = std::static_pointer_cast<PrimitiveGroupD3D12>(group);
// 	if (vbd)
// 	{
// 		pg12->DiscardVertexBuffer();
// 	}
// 	if (ibd)
// 	{
// 		pg12->DiscardIndexBuffer();
// 	}
// 	//pg12->LoadBuffers(vbd, ibd);
// }
// 
// std::shared_ptr<RenderBase::RootSignature> RenderDeviceD3D12::CreateRootSignature(const SignatureInfo& info)
// {
// 	std::shared_ptr<DX12RootSignature> pRoot = std::make_shared<DX12RootSignature>();
// 	pRoot->Init(info);
// 	return pRoot;
// }
// 
// void RenderDeviceD3D12::UpdateVertexBuffer(const std::shared_ptr<PrimitiveGroup>& pg, const DataStream& data)
// {
// 	assert(pg);
// 	std::shared_ptr<VertexBufferD3D12> vb12 = std::static_pointer_cast<VertexBufferD3D12>(pg->GetVertexBuffer()); //_Convert<VertexBuffer, VertexBufferD3D12>(pg->GetVertexBuffer());
// 	vb12->UpdateData(data);
// }
// 
// void RenderDeviceD3D12::UpdateIndexBuffer(const std::shared_ptr<PrimitiveGroup>& pg, const DataStream& data)
// {
// 	assert(pg);
// 	std::shared_ptr<IndexBufferD3D12> ib12 = std::static_pointer_cast<IndexBufferD3D12>(pg->GetIndexBuffer()); //_Convert<VertexBuffer, VertexBufferD3D12>(pg->GetVertexBuffer());
// 	ib12->UpdateData(data);
// }
// 
// const std::shared_ptr<GPUProgram> RenderDeviceD3D12::CreateRenderGPUProgram(const std::shared_ptr<GPUProgram>& srcGPUProgram)
// {
// 	return NULL;
// }
// 
// std::shared_ptr<Pso> RenderDeviceD3D12::CreatePso(const RenderBase::PsoData& info, const std::shared_ptr<RenderBase::RootSignature>& pSignature)
// {
// 	std::shared_ptr<DX12Pso> pso = std::make_shared<DX12Pso>();// DX12Pso::Create();
// 	pso->Init(info, pSignature);
// 	return pso;
// }

// const std::shared_ptr<RenderStateDesc> RenderDeviceD3D12::CreateRenderState(const std::shared_ptr<RenderStateDesc>& state)
// {
// 	std::shared_ptr<RenderStateDesc> rsDesc = std::make_shared<RenderStateDesc>();// RenderStateDesc::Create();
// 	rsDesc->Setup();
// 	rsDesc->SetBlendState(state->GetBlendState());
// 	rsDesc->SetSamplerState(state->GetSamplerState());
// 	rsDesc->SetDepthAndStencilState(state->GetDepthAndStencilState());
// 	rsDesc->SetRasterizerState(state->GetRasterizerState());
// 	rsDesc->SetUpdateFlag(state->GetUpdateFlag());
// 	return rsDesc;
// }
// 
// const std::shared_ptr<RenderTarget> RenderDeviceD3D12::CreateRenderTarget(const std::shared_ptr<RenderTarget>& rt)
// {
// 	std::shared_ptr<RenderTargetD3D12> rtD12 = std::make_shared<RenderTargetD3D12>();// RenderTargetD3D12::Create();
// 	rtD12->SetDummy(rt->IsDummy());
// 	rtD12->SetWidth(rt->GetWidth());
// 	rtD12->SetHeight(rt->GetHeight());
// 	rtD12->SetAntiAliasQuality(rt->GetAntiAliasQuality());
// 	rtD12->SetColorBufferFormat(rt->GetColorBufferFormat());
// 	rtD12->SetMipMapsEnabled(rt->AreMipMapsEnabled());
// 	rtD12->SetClearFlags(rt->GetClearFlags());
// 	rtD12->SetClearColor(rt->GetClearColor());
// 	rtD12->SetClearStencil(rt->GetClearStencil());
// 	rtD12->SetClearDepth(rt->GetClearDepth());
// 
// 	if (rt->HasDepthStencilBuffer())
// 	{
// 		if (rt->GetSharedDepthStencil())
// 		{
// 			rtD12->AddSharedDepthStencilBuffer(rt->GetSharedDepthStencil());
// 		}
// 		else
// 		{
// 			rtD12->AddDepthStencilBuffer();
// 		}
// 	}
// 	if (rt->IsResolveTextureDimensionsValid())
// 	{
// 		rtD12->SetResolveTextureWidth(rt->GetResolveTextureWidth());
// 		rtD12->SetResolveTextureHeight(rt->GetResolveTextureHeight());
// 		rtD12->SetResolveTargetCpuAccess(rt->GetResolveTargetCpuAccess());
// 		rtD12->SetResolveRect(rt->GetResolveRect());
// 	}
// 
// 	rtD12->SetScreenSizeRatio(rt->GetScreenSizeRatio());
// 	rtD12->SetDefaultRenderTarget(rt->IsDefaultRenderTarget());
// 	rtD12->LoadBuffers(rt->GetWidth(), rt->GetHeight());
// 	rtD12->AssignD3D12Textures();
// 	return rtD12;
// }

// const std::shared_ptr<Texture> RenderDeviceD3D12::CreateRenderSideTexture(const std::shared_ptr<Texture>& tex)
// {
// 	std::shared_ptr<TextureD3D12> texture = std::make_shared<TextureD3D12>();
// 	texture->Setup(tex);
// 	if (tex->GetPixelFormat() == PixelFormat::InvalidPixelFormat)
// 	{
// 		texture->LoadFileFromBuffers();
// 	}
// 	else
// 	{
// 		texture->LoadBuffers();
// 	}
// 
// 	//texture->SetStream(NULL);
// 	return texture;
// }
// 
// const std::shared_ptr<MultipleRenderTarget> RenderDeviceD3D12::CreateMultipleRenderTarget(const std::shared_ptr<MultipleRenderTarget>& mrt)
// {
// 	return NULL;
// }
// 
// void RenderDeviceD3D12::UpdateTexture(RenderBase::Texture::UpdateFunction texUpdateFunc,std::shared_ptr<Texture> destTex, void* tag)
// {
// 	std::shared_ptr<TextureD3D12> destTexD12 = std::static_pointer_cast<TextureD3D12>(destTex);
// 
// 	assert(destTexD12);
// 	destTexD12->UpdateTexture(texUpdateFunc, tag);
// }
// 
// void RenderDeviceD3D12::UpdateTexture(std::shared_ptr<Texture> srcTex, std::shared_ptr<Texture> destTex)
// {
// 	std::shared_ptr<TextureD3D12> destTexD12 = std::static_pointer_cast<TextureD3D12>(destTex);
// 	assert(destTexD12);
// 
// 	destTexD12->UpdateTexture( srcTex );
// }
// 
// void RenderDeviceD3D12::ChangeTexture(std::shared_ptr<Texture> srcTex, std::shared_ptr<Texture> destTex)
// {
// 	std::shared_ptr<TextureD3D12> destTexD12 = std::static_pointer_cast<TextureD3D12>(destTex);
// 	assert(destTexD12);
// 	destTexD12->Unload();
// 	destTexD12->Setup(srcTex);
// 	destTexD12->LoadBuffers();
// 	//destTexD12->SetStream(NULL);
// }

// std::shared_ptr<ConstantBuffer> RenderDeviceD3D12::CreateConstantBuffer(SizeT nSize, const RenderBase::BufferData& desc)
// {
// 	std::shared_ptr<DX12ConstantBuffer> pBuffer = std::make_shared<DX12ConstantBuffer>();// DX12ConstantBuffer::Create();
// 	pBuffer->CreateBuffer(nSize);
// 	return pBuffer;
// }
// 
// void RenderDeviceD3D12::UpdateConstantBuffer(const std::shared_ptr<ConstantBuffer>& pBuffer, const DataStream& data)
// {
// 	const std::shared_ptr<DX12ConstantBuffer>& pDxBuffer = std::static_pointer_cast<DX12ConstantBuffer>(pBuffer);// pBuffer.downcast<DX12ConstantBuffer>();
// 	pDxBuffer->UpdateBuffer(data);
// }
// 
// void RenderDeviceD3D12::SetVSConstantBuffer(const std::shared_ptr<ConstantBuffer>& pBuffer, IndexT bindPoint)
// {
// 	const std::shared_ptr<DX12ConstantBuffer>& pDxBuffer = std::static_pointer_cast<DX12ConstantBuffer>(pBuffer);
// 	ID3D12Resource* pCBuffer = pDxBuffer->GetBuffer();
// 	const std::map<IndexT, IndexT>& slotMap = m_pCurrRootSig->GetSlotMap();
// 	m_pGraphicCommandQueue->GetCommandList()->SetGraphicsRootConstantBufferView(slotMap.find(bindPoint)->second, pCBuffer->GetGPUVirtualAddress());
// 	m_pGraphicCommandQueue->IncCmmdNum();
// }
// 
// void RenderDeviceD3D12::SetPSConstantBuffer(const std::shared_ptr<ConstantBuffer>& pBuffer, IndexT bindPoint)
// {
// 	const std::shared_ptr<DX12ConstantBuffer>& pDxBuffer = std::static_pointer_cast<DX12ConstantBuffer>(pBuffer);
// 	ID3D12Resource* pCBuffer = pDxBuffer->GetBuffer();
// 	const std::map<IndexT, IndexT>& slotMap = m_pCurrRootSig->GetSlotMap();
// 	m_pGraphicCommandQueue->GetCommandList()->SetGraphicsRootConstantBufferView(slotMap.find(bindPoint)->second, pCBuffer->GetGPUVirtualAddress());
// 	m_pGraphicCommandQueue->IncCmmdNum();
// }

// void RenderDeviceD3D12::SetTexture(const std::shared_ptr<Texture>& tex)
// {	
// 	std::shared_ptr<TextureD3D12> texture = std::static_pointer_cast<TextureD3D12>(tex);
// 
// 	if (texture->GetUnitIndex() < 16)
// 	{
// 		if ( m_stateCache.textureCache[texture->GetUnitIndex()].lock() != texture )
// 		{
// 			if (texture->IsRendertargetRes())
// 			{
// 				m_pGraphicCommandQueue->GetCommandList()->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(texture->GetResource(),
// 																		  D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE));
// 				texture->GetFromRt()->SetResStates(D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
// 			}
// 
// 			m_stateCache.textureCache[texture->GetUnitIndex()] = texture;
// 			m_pGraphicCommandQueue->GetCommandList()->SetGraphicsRootDescriptorTable(texture->GetUnitIndex(), texture->GetGpuHandle());
// 			m_pGraphicCommandQueue->IncCmmdNum();
// 		}
// 	}
// }

// void RenderDeviceD3D12::SetRenderTarget(const std::shared_ptr<RenderTarget>& rt, bool bGammaToLinear /* = false */)
// {
// 	std::shared_ptr<RenderTargetD3D12> rtd3d12 = std::static_pointer_cast<RenderTargetD3D12>(rt);
// 	IndexT i = rt->GetMRTIndex();
// 	D3D12RtInfo* pInfo = rt->IsDefaultRenderTarget() ? rtd3d12->GetBackBufferInfo(m_nCurrBackBuffer) :
// 						 rtd3d12->GetTargetInfo();
// 
// 	m_CurAAQuality = rtd3d12->GetSampleQuality();
// 	m_CurAAType    = rtd3d12->GetSampleType();
// 
// 	if (m_pRenderTargets[i] != rtd3d12)
// 	{
// 		if (m_pRenderTargets[i])
// 		{
// 			m_pRenderTargets[i]->EndPass();
// 		}
// 		m_pRenderTargets[i] = rtd3d12;
// 
// 		if (rtd3d12->GetResStates() != D3D12_RESOURCE_STATE_RENDER_TARGET)
// 		{
// 			m_pGraphicCommandQueue->GetCommandList()->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(pInfo->pResource,
// 																	  rtd3d12->GetResStates(), D3D12_RESOURCE_STATE_RENDER_TARGET));
// 			rtd3d12->SetResStates(D3D12_RESOURCE_STATE_RENDER_TARGET);
// 		}
// 
// 		
// 		m_pGraphicCommandQueue->GetCommandList()->OMSetRenderTargets(1, &pInfo->cpuRtv.handle, false, &pInfo->cpuDsv.handle);
// 	}
// 
// 	if (0 != (rtd3d12->clearFlags & RenderTarget::ClearColor))
// 	{
// 		Math::float4 clearColor = rtd3d12->GetClearColor();
// 		m_pGraphicCommandQueue->GetCommandList()->ClearRenderTargetView(pInfo->cpuRtv.handle, reinterpret_cast<float*>(&clearColor), 0, NULL);
// 	}
// 	if (0 != pInfo->pDepthResource)
// 	{
// 		uint flag = 0;
// 		if (0 != (rtd3d12->clearFlags & RenderTarget::ClearDepth))
// 		{
// 			flag |= D3D12_CLEAR_FLAG_DEPTH;
// 		}
// 		if (0 != (rtd3d12->clearFlags & RenderTarget::ClearStencil))
// 		{
// 			flag |= D3D12_CLEAR_FLAG_STENCIL;
// 		}
// 
// 		if (flag != 0)
// 		{
// 			m_pGraphicCommandQueue->GetCommandList()->ClearDepthStencilView(pInfo->cpuDsv.handle, (D3D12_CLEAR_FLAGS)flag, 1.0f, 0, 0, NULL);
// 		}	
// 	}
// 
// 	if (rt->IsDefaultRenderTarget())
// 	{
// 		m_pCurrBackBuffer = rtd3d12;
// 	}
// 
// 	m_pGraphicCommandQueue->IncCmmdNum();
// }

// void RenderDeviceD3D12::SetMultipleRenderTarget(const std::shared_ptr<MultipleRenderTarget>& mrt, bool resume /* = false */)
// {
// 
// }
// 
// void RenderDeviceD3D12::DisableRenderTarget(int index)
// {
// 
// }

// void RenderDeviceD3D12::SetViewPort(const int& x,const int& y,const int& width,const int& height,const float& minZ,const float& maxZ)
// {
// 	D3D12_VIEWPORT vp;
// 	vp.Width = static_cast<float>(width);
// 	vp.Height = static_cast<float>(height);
// 	vp.TopLeftX = static_cast<float>(x);
// 	vp.TopLeftY = static_cast<float>(y);
// 	vp.MinDepth = minZ;
// 	vp.MaxDepth = maxZ;
// 	m_pGraphicCommandQueue->GetCommandList()->RSSetViewports(1, &vp);
// 
// 	D3D12_RECT rect;
// 	rect.top = 0;
// 	rect.left = 0;
// 	rect.right = width;
// 	rect.bottom = height;
// 	m_pGraphicCommandQueue->GetCommandList()->RSSetScissorRects(1, &rect);
// 	m_pGraphicCommandQueue->IncCmmdNum();
// }

// void RenderDeviceD3D12::Draw(SizeT startVertice,SizeT numVertice,SizeT startIndice,SizeT numIndice)
// {
// 	std::shared_ptr<PrimitiveGroupD3D12> pgd12 = this->m_primitiveGroupD12.lock();
// 	D3D12_PRIMITIVE_TOPOLOGY d3dPrimType = D3D12Types::AsD3D12PrimitiveType(pgd12->GetPrimitiveTopology());
// 	m_pGraphicCommandQueue->GetCommandList()->IASetPrimitiveTopology(d3dPrimType);
// 
// 	assert(pgd12->GetNumVertices() >= numVertice && pgd12->GetNumVertices() >= startVertice);
// 	assert(pgd12->GetNumIndices() >= numIndice && pgd12->GetNumIndices() >= startIndice);
// 
// 		if (pgd12->GetNumIndices() > 0)
// 		{
// 			m_pGraphicCommandQueue->GetCommandList()->DrawIndexedInstanced(numIndice, 1, startIndice, 0, 0);
// 		}
// 		else
// 		{
// 			m_pGraphicCommandQueue->GetCommandList()->DrawInstanced(numVertice, 1, 0, 0);
// 		}
// 	m_pGraphicCommandQueue->IncCmmdNum();
// }

// void RenderDeviceD3D12::ExecuteCmmdList()
// {
// 	std::shared_ptr<RenderTargetD3D12> pcbb = m_pCurrBackBuffer.lock();
// 	m_pGraphicCommandQueue->GetCommandList()->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(pcbb->GetBackBufferInfo(m_nCurrBackBuffer)->pResource,
// 															  D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));
// 	pcbb->SetResStates(D3D12_RESOURCE_STATE_PRESENT);
// 
// 	// Done recording commands.
// 	HRESULT hr = m_pGraphicCommandQueue->GetCommandList()->Close();
// 	if (FAILED(hr))
// 	{
// 	   assert(false);
// 	}
// 
// 	// Add the command list to the queue for execution.
// 	ID3D12CommandList* cmdsLists[] = { m_pGraphicCommandQueue->GetCommandList() };
// 	m_pCommandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);
// }

// void RenderDeviceD3D12::FlushCmmdList()
// {
// 	if (!m_pGraphicCommandQueue->IsEmpty())
// 	{
// 		m_pGraphicCommandQueue->GetCommandList()->Close();
// 		ID3D12CommandList* cmdsLists[] = { m_pGraphicCommandQueue->GetCommandList() };
// 		m_pCommandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);
// 
// 		m_pGraphicCommandQueue->Flush();
// 	}
// 
// 	// Reuse the memory associated with command recording.
// 	// We can only reset when the associated command lists have finished execution on the GPU.
// 	HRESULT hr = m_pCommandAllocator->Reset();
// 	m_pGraphicCommandQueue->GetCommandList()->Reset(m_pCommandAllocator, NULL);
// 	if (FAILED(hr))
// 	{
// 		assert(false);
// 	}
// }

// void RenderDeviceD3D12::Present(WindHandle hwnd)
// {
// 	m_pCurSwapChain->Present( 0, 0 );
// 	m_nCurrBackBuffer = (m_nCurrBackBuffer + 1) % s_MaxSwapChainBufferCount;
// }

float RenderDeviceD3D12::GetHorizontalTexelOffset(void)
{
	// No offset in GL
	return 0.0f;
}

float RenderDeviceD3D12::GetVerticalTexelOffset(void)
{
	// No offset in GL
	return 0.0f;
}

float RenderDeviceD3D12::GetMinimumDepthInputValue()
{
	// Range [-1.0f, 1.0f]
	return 0.0f;
}

float RenderDeviceD3D12::GetMaximumDepthInputValue()
{
	// Range [-1.0f, 1.0f]
	return 1.0f;
}

void RenderDeviceD3D12::DetectGraphicCardCaps()
{

}

// RenderBase::RenderWindow* RenderDeviceD3D12::CreateRenderWindow( WindHandle hWnd )
// {
// 	RenderBase::RenderWindow* wnd = new D3D12Window(hWnd );
// 	return wnd;
// }
// 
// void RenderDeviceD3D12::DestroyRenderWindow(const std::shared_ptr<RenderWindow>& rw)
// {
// 	//delete rw;
// 	assert(0);
// }

bool RenderDeviceD3D12::OnDeviceReset()
{
	return false;
}

bool RenderDeviceD3D12::OnDeviceLost()
{
	return false;
}

bool RenderDeviceD3D12::CheckReset()
{
	return false;
}

// void RenderDeviceD3D12::SetSwapChain(const RenderBase::RenderTargetHandle& handle)
// {
// 	std::shared_ptr< RenderTargetD3D12> rt = std::static_pointer_cast<RenderTargetD3D12>(handle.AsObject());
// 	m_pCurSwapChain = rt->GetSwapChain();
// 	assert(m_pCurSwapChain);
// }
// 
// void RenderDeviceD3D12::ClearCache()
// {
// 	m_stateCache.ClearCache();
// }

}


#endif