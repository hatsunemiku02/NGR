#pragma once
#include <memory>
#include <unordered_map>
#include "d3d12/DX12RootSignature.h"


struct HashSigInfo
{
	std::size_t operator()(const RenderBase::SignatureInfo &key) const {
		using std::size_t;
		using std::hash;
		std::size_t ret = 0;
		for (int i=0;i<key.cbvInfo.size();i++)
		{
			ret += std::hash<int>()(key.cbvInfo[i]);
		}
		for (int i = 0; i < key.srvInfo.size(); i++)
		{
			ret += std::hash<int>()(key.srvInfo[i]);
		}
		for (int i = 0; i < key.samplerInfo.size(); i++)
		{
			//ret ^= std::hash<int>()(key.samplerInfo[i]);
		}
		return ret;
	}
};

struct EqualSigInfo
{
	bool operator()(const RenderBase::SignatureInfo &lhs, const RenderBase::SignatureInfo &rhs) const {
		bool ret = true;
		if (lhs.cbvInfo.size() == rhs.cbvInfo.size()
			&& lhs.srvInfo.size() == rhs.srvInfo.size()
			&& lhs.samplerInfo.size() == rhs.samplerInfo.size())
		{
		//¡¢
		}
		else
		{
			ret = false;
		}
		return ret;
	}
};

class  RootSigCreator
{
public:
	RootSigCreator();
	~RootSigCreator();

	std::shared_ptr<D3D12::DX12RootSignature> GetRootSig(const RenderBase::SignatureInfo& siginfo);
protected:

	//std::unordered_map<RenderBase::SignatureInfo, std::shared_ptr<D3D12::DX12RootSignature>> m_RootSigMap;
	std::unordered_map<std::string, std::shared_ptr<D3D12::DX12RootSignature>> m_RootSigMap;
private:
};