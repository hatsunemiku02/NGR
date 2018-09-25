#pragma once
#include <memory>
#include "d3d12/PrimitiveGroupD3D12.h"
#include "d3d12/DX12Pso.h"
#include "d3d12/DX12RootSignature.h"
#include "Material.h"
#include "ViewPort.h"
class RenderObj
{
public:
	RenderObj();
	~RenderObj();

	void Init(const std::shared_ptr<D3D12::PrimitiveGroupD3D12>& pg);

	std::shared_ptr<D3D12::DX12Pso> m_pPipeStateObj;
	std::shared_ptr<D3D12::DX12RootSignature> m_pRootSig;

	std::shared_ptr<D3D12::PrimitiveGroupD3D12> m_pPrimitiveGroup;
	std::shared_ptr<Material> m_pMaterial;

	void GenerateInternal(const std::shared_ptr<ViewPortD3D12>& pViewPort);
protected:
	
	
private:
};
