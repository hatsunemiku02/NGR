#pragma once
#include <memory>
#include <vector>
#include "d3d12/RenderDeviceD3D12.h"
#include "d3d12/CommandQueue.h"
#include "RenderObj.h"
#include "ViewPort.h"
#include "RenderTarget.h"

class pipeline
{
public:
	pipeline();
	~pipeline();

	void SetViewPort(const std::shared_ptr<ViewPortD3D12>& pViewPort);
	
	void PrepareRenderList(size_t count);
	void AddRenderObj(const std::shared_ptr<RenderObj>& obj);
	void ResetRenderList();

	void Reset();
	void Render();
protected:
	std::unique_ptr<D3D12::GraphicCommandList> m_CommandList;

	std::vector<std::shared_ptr<RenderObj>> m_RenderList;

	std::shared_ptr<ViewPortD3D12> m_pViewPort;

private:


};


inline void pipeline::PrepareRenderList(size_t count)
{
	m_RenderList.reserve(count);
}

inline void pipeline::AddRenderObj(const std::shared_ptr<RenderObj>& obj)
{
	m_RenderList.push_back(obj);
}

inline void pipeline::ResetRenderList()
{
	m_RenderList.clear();
}
