#include "stdneb.h"
#include "RenderTargetD3D12.h"
#include "RenderDeviceD3D12.h"
#include "D3D12Types.h"
#include "TextureD3D12.h"
#include "CommandQueue.h"

#if RENDERDEVICE_D3D12

namespace D3D12
{

void D3D12RtInfo::Clear()
{
	if (pResource != NULL)
	{
		pResource->Release();
		pResource = NULL;
	}

	if (pDepthResource != NULL)
	{
		pDepthResource->Release();
		pDepthResource = NULL;
	}

	ID3D12Device* pDevice = RenderDeviceD3D12::Instance()->GetDevice();
	DescriptorHeap* pRtvHeap = RenderDeviceD3D12::Instance()->GetRtvHeap();
	DescriptorHeap* pSrvHeap = RenderDeviceD3D12::Instance()->GetCsuHeap();
	DescriptorHeap* pDsvHeap = RenderDeviceD3D12::Instance()->GetDsvHeap();

	pRtvHeap->ResetCPUHanlde(cpuRtv.ind);
	pSrvHeap->ResetCPUHanlde(cpuSrv.ind);
	pDsvHeap->ResetCPUHanlde(cpuDsv.ind);
}

RenderTargetD3D12::RenderTargetD3D12()
	:	m_MultiSampleType(1),
		m_MultiSampleQuality(0),
		m_bNeedsResolve(false),
		m_pSwapChain(nullptr),
		m_pD3D12Rt(new D3D12RtInfo()),
		m_pD3d12ResovleRt(nullptr),
		m_CurState(D3D12_RESOURCE_STATE_COMMON)
{

}

RenderTargetD3D12::~RenderTargetD3D12()
{
	Discard();
}

void RenderTargetD3D12::LoadBuffers(SizeT _width, SizeT _height)
{
	// if we're the default render target, query display device
	// for setup parameters
	if (this->isDefaultRenderTarget)
	{
		// NOTE: the default render target will never be anti-aliased!
		// this assumes a render pipeline where the actual rendering goes
		// into an offscreen render target and is then resolved to the back buffer
		this->SetWidth(_width);
		this->SetHeight(_height);
		this->SetAntiAliasQuality(AntiAliasQuality::None);
	}

	if (!this->resolveTexture)
	{
		this->resolveTexture = std::make_shared<TextureD3D12>();
		assert(this->resolveTexture);
	}

	if (!this->isDefaultRenderTarget && screenSizeRatio > 0.f)
	{
		if (this->resolveTextureDimensionsValid)
		{
			if ((this->resolveTextureWidth != this->width) || (this->resolveTextureHeight != this->height))
			{
				//do nothing
			}
			else
			{
				this->SetResolveTextureWidth(SizeT(_width*screenSizeRatio));
				this->SetResolveTextureHeight(SizeT(_height*screenSizeRatio));
				this->resolveRect.right = int(_width*screenSizeRatio);
				this->resolveRect.bottom = int(_height*screenSizeRatio);
			}
		}

		this->SetWidth(int(_width*screenSizeRatio));
		this->SetHeight(int(_height*screenSizeRatio));
	}

	this->setupMultiSampleType();
	// check if a resolve texture must be allocated
	if (this->m_MultiSampleType > 1)
	{
		this->m_bNeedsResolve = true;
	}
	else
	{
		this->m_bNeedsResolve = false;
	}

	if (!this->m_bNeedsResolve)
	{
		if (!this->isDefaultRenderTarget || this->IsDummy())
		{
			_CreateRT(m_pD3D12Rt);
		} 
		else
		{
			_CreateBackbuffer();
		}
	} 
	else
	{
		_CreateRT(m_pD3D12Rt, m_MultiSampleType, m_MultiSampleQuality);
		m_pD3d12ResovleRt = new D3D12RtInfo;
		_CreateRT(m_pD3d12ResovleRt);
	}

	RenderDeviceD3D12::Instance()->GetGraphicQueue()->IncCmmdNum();

	RenderTarget::setup();
}

void RenderTargetD3D12::_CreateRT(D3D12RtInfo* pInfo, uint multiSample /* = 1 */, uint quality /* = 0 */)
{
	ID3D12Device* pDevice = RenderDeviceD3D12::Instance()->GetDevice();
	DescriptorHeap* pRtvHeap = RenderDeviceD3D12::Instance()->GetRtvHeap();
	DescriptorHeap* pSrvHeap = RenderDeviceD3D12::Instance()->GetCsuHeap();

	D3D12_RESOURCE_DESC texDesc;
	ZeroMemory(&texDesc, sizeof(D3D12_RESOURCE_DESC));
	texDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	texDesc.Alignment = 0;
	texDesc.Width = this->width;
	texDesc.Height = this->height;
	texDesc.DepthOrArraySize = 1;
	texDesc.MipLevels = 1;
	texDesc.Format = D3D12Types::AsD3D12PixelFormat(this->colorBufferFormat);
	texDesc.SampleDesc.Count = multiSample;
	texDesc.SampleDesc.Quality = quality;
	texDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	texDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;

	D3D12_CLEAR_VALUE optClear;
	optClear.Format = texDesc.Format;
	optClear.Color[0] = clearColor.x();
	optClear.Color[1] = clearColor.y();
	optClear.Color[2] = clearColor.z();
	optClear.Color[3] = clearColor.w();

	HRESULT hr = pDevice->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&texDesc,
		D3D12_RESOURCE_STATE_RENDER_TARGET,
		&optClear,
		IID_PPV_ARGS(&pInfo->pResource));
	m_CurState = D3D12_RESOURCE_STATE_RENDER_TARGET;

	if (FAILED(hr))
	{
		assert(false);
	}

	D3D12_RENDER_TARGET_VIEW_DESC rtDesc;
	rtDesc.Format = D3D12Types::AsD3D12PixelFormat(this->colorBufferFormat);
	rtDesc.ViewDimension = multiSample > 1 ? D3D12_RTV_DIMENSION_TEXTURE2DMS : D3D12_RTV_DIMENSION_TEXTURE2D;
	rtDesc.Texture2D.MipSlice = 0;
	rtDesc.Texture2D.PlaneSlice = 0;

	pInfo->cpuRtv = pRtvHeap->GetCPUHandle();
	pDevice->CreateRenderTargetView(pInfo->pResource, &rtDesc, pInfo->cpuRtv.handle);

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.Format = D3D12Types::AsD3D12PixelFormat(this->colorBufferFormat); 
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = 1;
	srvDesc.Texture2D.ResourceMinLODClamp = 0.0f;
	srvDesc.Texture2D.PlaneSlice = 0;

	pInfo->cpuSrv = pSrvHeap->GetCPUHandle();
	pDevice->CreateShaderResourceView(pInfo->pResource, &srvDesc, pInfo->cpuSrv.handle);
	pInfo->gpuSrv = pSrvHeap->GetGpuHandleFromCpu(pInfo->cpuSrv);

	if (this->HasDepthStencilBuffer())
	{
		if (sharedDepthStencilBufferTarget)
		{
			const std::shared_ptr<RenderTargetD3D12>& pSharedDepthStencilBufferTarget = std::static_pointer_cast<RenderTargetD3D12>(this->sharedDepthStencilBufferTarget);
			pInfo->pDepthResource = pSharedDepthStencilBufferTarget->m_pD3D12Rt->pDepthResource;
			pInfo->pDepthResource->AddRef();
			pInfo->cpuDsv = pSharedDepthStencilBufferTarget->m_pD3D12Rt->cpuDsv;
		} 
		else
		{
			_CreateDepthStencilView(*pInfo);
		}
	}
}

void RenderTargetD3D12::_CreateBackbuffer()
{

	m_SwapChainRTList.resize(RenderDeviceD3D12::s_MaxSwapChainBufferCount, D3D12RtInfo());

	ID3D12Device* pDevice   = RenderDeviceD3D12::Instance()->GetDevice();
	IDXGIFactory* pFactory = RenderDeviceD3D12::Instance()->GetDXGI();
	DescriptorHeap* pHeap   = RenderDeviceD3D12::Instance()->GetRtvHeap();

	DXGI_SWAP_CHAIN_DESC sd;
	sd.BufferDesc.Width = this->width;
	sd.BufferDesc.Height = this->height;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferDesc.Format = D3D12Types::AsD3D12PixelFormat(RenderBase::PixelFormat::X8R8G8B8);
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = RenderDeviceD3D12::s_MaxSwapChainBufferCount;
	sd.OutputWindow = (HWND)RenderDeviceD3D12::Instance()->GetWindowHandle();;
	sd.Windowed = true;
	sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	GraphicCommandList* pQueue = RenderDeviceD3D12::Instance()->GetGraphicQueue();

	HRESULT hr = pFactory->CreateSwapChain(RenderDeviceD3D12::Instance()->GetCmdQueue(), &sd, &m_pSwapChain);
	if (FAILED(hr))
	{
		assert(false);
	}

	for (IndexT i = 0; i < RenderDeviceD3D12::s_MaxSwapChainBufferCount; ++i)
	{
		D3D12RtInfo& info = m_SwapChainRTList[i];
		info.cpuRtv = pHeap->GetCPUHandle();
		hr = m_pSwapChain->GetBuffer(i, IID_PPV_ARGS(&info.pResource));
		if (FAILED(hr))
		{
			assert(false);
		}

		pDevice->CreateRenderTargetView(info.pResource, NULL, info.cpuRtv.handle);

		_CreateDepthStencilView(info);
	}

}

void RenderTargetD3D12::_CreateDepthStencilView(D3D12RtInfo& info, uint multiSample /* = 1 */, uint quality /* = 0 */)
{
	ID3D12Device* pDevice = RenderDeviceD3D12::Instance()->GetDevice();
	DescriptorHeap* pDsvHeap = RenderDeviceD3D12::Instance()->GetDsvHeap();

	D3D12_RESOURCE_DESC depthStencilDesc;
	depthStencilDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	depthStencilDesc.Alignment = 0;
	depthStencilDesc.Width = this->width;
	depthStencilDesc.Height = this->height;
	depthStencilDesc.DepthOrArraySize = 1;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.SampleDesc.Count = multiSample;
	depthStencilDesc.SampleDesc.Quality = quality;
	depthStencilDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	depthStencilDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

	D3D12_CLEAR_VALUE optClear;
	optClear.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	optClear.DepthStencil.Depth = 1.0f;
	optClear.DepthStencil.Stencil = 0;

	HRESULT hr = pDevice->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&depthStencilDesc,
		D3D12_RESOURCE_STATE_COMMON,
		&optClear,
		IID_PPV_ARGS(&info.pDepthResource));

	if (FAILED(hr))
	{
		assert(false);
	}

	info.cpuDsv = pDsvHeap->GetCPUHandle();

	D3D12_DEPTH_STENCIL_VIEW_DESC ds = {};
	ds.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	ds.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	ds.Flags = D3D12_DSV_FLAG_NONE;


	pDevice->CreateDepthStencilView(info.pDepthResource, &ds, info.cpuDsv.handle);

	GraphicCommandList* pQueue = RenderDeviceD3D12::Instance()->GetGraphicQueue();
	ID3D12GraphicsCommandList* pCommandList = pQueue->GetCommandList();
	pCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(info.pDepthResource, 
								  D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_DEPTH_WRITE) );
}

void RenderTargetD3D12::setupMultiSampleType()
{
	if (this->antiAliasQuality == RenderBase::AntiAliasQuality::None)
	{
		this->m_MultiSampleType = 1;
		this->m_MultiSampleQuality = 0;
	}
	else
	{
		this->m_MultiSampleType = D3D12Types::AsD3D12MultiSampleType(this->antiAliasQuality);

		D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS levels;
		levels.Format = D3D12Types::AsD3D12PixelFormat(this->colorBufferFormat);
		levels.SampleCount = this->m_MultiSampleType;
		levels.Flags = D3D12_MULTISAMPLE_QUALITY_LEVELS_FLAG_NONE;
		levels.NumQualityLevels = 0;
		HRESULT hr = RenderDeviceD3D12::Instance()->GetDevice()->CheckFeatureSupport(D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS, 
					&levels, sizeof(levels));
		if (FAILED(hr))
		{
			assert(false);
		}
		
		this->m_MultiSampleQuality = levels.NumQualityLevels;
	}
}

void RenderTargetD3D12::Discard()
{
	m_pD3D12Rt->Clear();
	delete[] m_pD3D12Rt;
	if (m_pD3d12ResovleRt != NULL)
	{
		m_pD3d12ResovleRt->Clear();
		delete m_pD3d12ResovleRt;
	}

	if (m_pSwapChain != NULL)
	{
		m_pSwapChain->Release();
		m_pSwapChain = NULL;
	}

	m_pD3D12Rt = NULL;
	m_pD3d12ResovleRt = NULL;
	RenderTarget::Discard();
}

void RenderTargetD3D12::AssignD3D12Textures(bool bIsLostDevice /* = false */)
{
	if (this->isDefaultRenderTarget)
	{
		return;
	}

	this->resolveTexture->Setup();
	this->resolveTexture->SetWidth(this->width);
	this->resolveTexture->SetHeight(this->height);

	std::shared_ptr<TextureD3D12> td3d12 = std::static_pointer_cast<TextureD3D12>(resolveTexture);
	if (m_bNeedsResolve)
	{
		td3d12->SetupFromD3D12Texture(m_pD3d12ResovleRt->pResource, m_pD3d12ResovleRt->gpuSrv);
	} 
	else
	{
		td3d12->SetupFromD3D12Texture(m_pD3D12Rt->pResource, m_pD3D12Rt->gpuSrv);
	}

	td3d12->SetRendertargetRes(true);
	td3d12->SetFromRt(this);
}

void RenderTargetD3D12::EndPass()
{

}

void RenderTargetD3D12::CopyFrom(const Math::float4& srcRect, const std::shared_ptr<RenderTarget>srcRT, const Math::float4& desRect)
{

}

}



#endif