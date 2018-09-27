#pragma once
#include <memory>
#include <vector>
#include "d3d12/RenderDeviceD3D12.h"
#include "d3d12/CommandQueue.h"
#include "RenderObj.h"
#include "ViewPort.h"
#include "RenderTarget.h"
#include "PipeLine2MatInfo.h"

class Pipeline
{
public:
	Pipeline();
	~Pipeline();

	void SetViewPort(const std::shared_ptr<ViewPortD3D12>& pViewPort);

	void SetRenderTarget(const std::shared_ptr<RenderTarget>& pRenderTarget);
	void SetRenderToScreen(RenderBase::PixelFormat::Code format, HWND hwnd);

	void PrepareRenderList(size_t count);
	void AddRenderObj(const std::shared_ptr<RenderObj>& obj);
	void ResetRenderList();

	void Reset();
	void Render();

	Pipeline2MatInfo GenerateMatExternalInfo();

protected:
	void SetMaterial(const std::shared_ptr<RenderObj>& obj);
	void SetVertexBuffer(const std::shared_ptr<RenderObj>& obj);
	void RenderOneItem(const std::shared_ptr<RenderObj>& obj);


	std::unique_ptr<D3D12::GraphicCommandList> m_CommandList;

	std::vector<std::shared_ptr<RenderObj>> m_RenderList;

	std::shared_ptr<ViewPortD3D12> m_pViewPort;

	std::shared_ptr<RenderTarget> m_pRenderTarget;

	IDXGISwapChain3*	m_pDefaultSwapChain;
	std::vector<std::shared_ptr<RenderTarget>> m_pDefaultRenderTargets;


	
private:


};


inline void Pipeline::PrepareRenderList(size_t count)
{
	m_RenderList.reserve(count);
}

inline void Pipeline::AddRenderObj(const std::shared_ptr<RenderObj>& obj)
{
	m_RenderList.push_back(obj);
}

inline void Pipeline::ResetRenderList()
{
	m_RenderList.clear();
}
