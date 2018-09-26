#include "RootSigCreator.h"




std::shared_ptr<D3D12::DX12RootSignature> RootSigCreator::GetRootSig(const RenderBase::SignatureInfo& siginfo)
{
// 	if (m_RootSigMap.find(siginfo)!=m_RootSigMap.end())
// 	{
// 		std::shared_ptr<D3D12::DX12RootSignature> ptr = std::make_shared<D3D12::DX12RootSignature>();
// 		ptr->Init(siginfo);
// 		m_RootSigMap.insert(std::make_pair(siginfo, ptr));
// 	}
// 	else
// 	{
// 		return m_RootSigMap[siginfo];
// 	}
	return nullptr;
}

