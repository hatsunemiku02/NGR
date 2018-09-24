#pragma once

#include "base/RenderTarget.h"
#include "DescriptorHeap.h"

#if RENDERDEVICE_D3D12

namespace D3D12
{
	using namespace RenderBase;

	struct D3D12RtInfo
	{
		CPUHandle						cpuRtv;
		CPUHandle						cpuSrv;
		CPUHandle						cpuDsv;
		ID3D12Resource*					pResource;
		ID3D12Resource*					pDepthResource;
		CD3DX12_GPU_DESCRIPTOR_HANDLE	gpuSrv;
		D3D12RtInfo()
			: pResource(NULL),
			  pDepthResource(NULL)
		{

		}

		void Clear();
	};

	class RenderTargetD3D12 : public RenderTarget
{
public:

	RenderTargetD3D12();

	virtual ~RenderTargetD3D12();
	virtual void Discard();  
	/// setup the render target object
	//void LoadBuffers();
	virtual void LoadBuffers(SizeT _width, SizeT _height);

	/// binding textures;
	void AssignD3D12Textures(bool bIsLostDevice = false);

	D3D12RtInfo* GetTargetInfo() const;
	D3D12RtInfo* GetBackBufferInfo(IndexT ind) const;
	IDXGISwapChain* GetSwapChain() const;

	virtual void EndPass();

	virtual void CopyFrom(const Math::float4& srcRect, const std::shared_ptr<RenderTarget>srcRT, const Math::float4& desRect);

	uint	GetSampleQuality() const;
	uint	GetSampleType() const;

	void	SetResStates(D3D12_RESOURCE_STATES states);
	D3D12_RESOURCE_STATES GetResStates() const;

protected:
	friend class RenderDeviceD3D12;
	void setupMultiSampleType();
	void _CreateRT(D3D12RtInfo* pInfo, uint multiSample = 1, uint quality = 0);
	void _CreateBackbuffer();
	void _CreateDepthStencilView(D3D12RtInfo& info, uint multiSample = 1, uint quality = 0);
private:

	UINT						m_MultiSampleQuality;
	UINT						m_MultiSampleType;
	DXGI_FORMAT					m_d3d12ColorBufferFormat;
	bool						m_bNeedsResolve;
	IDXGISwapChain*				m_pSwapChain;
	D3D12RtInfo*				m_pD3D12Rt;
	D3D12RtInfo*				m_pD3d12ResovleRt;
	std::vector<D3D12RtInfo>	m_SwapChainRTList;

	D3D12_RESOURCE_STATES		m_CurState;
};

inline D3D12RtInfo* RenderTargetD3D12::GetTargetInfo() const
{
	return m_pD3D12Rt;
}

inline D3D12RtInfo* RenderTargetD3D12::GetBackBufferInfo(IndexT ind) const
{
	const D3D12RtInfo *ret = &(m_SwapChainRTList[ind]);
	return const_cast<D3D12RtInfo *> (ret);
}

inline IDXGISwapChain* RenderTargetD3D12::GetSwapChain() const
{
	return m_pSwapChain;
}

inline uint RenderTargetD3D12::GetSampleQuality() const
{
	return m_MultiSampleQuality;
}

inline uint RenderTargetD3D12::GetSampleType() const
{
	return m_MultiSampleType;
}

inline void RenderTargetD3D12::SetResStates(D3D12_RESOURCE_STATES states)
{
	m_CurState = states;
}

inline D3D12_RESOURCE_STATES RenderTargetD3D12::GetResStates() const
{
	return m_CurState;
}

}

#endif