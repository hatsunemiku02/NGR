#pragma once
#include "base/RootSignature.h"
#include "D3D12Heads.h"
#include <map>

#if RENDERDEVICE_D3D12

namespace D3D12
{
	class DX12RootSignature : public RenderBase::RootSignature
{

public:

	DX12RootSignature();

	virtual ~DX12RootSignature();

	virtual void Init(const RenderBase::SignatureInfo& info);

	ID3D12RootSignature* GetRootSignature() const;

	const std::map<IndexT, IndexT>& GetSlotMap() const;

protected:
private:

	ID3D12RootSignature*				m_pRoot;
	std::map<IndexT, IndexT>	m_SlotMap;
};

inline ID3D12RootSignature* DX12RootSignature::GetRootSignature() const
{
	return m_pRoot;
}

inline const std::map<IndexT, IndexT>& DX12RootSignature::GetSlotMap() const
{
	return m_SlotMap;
}

}

#endif