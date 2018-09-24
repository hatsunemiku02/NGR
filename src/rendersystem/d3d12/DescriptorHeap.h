#pragma once

#include <vector>

#if RENDERDEVICE_D3D12

#include "D3D12Heads.h"

namespace D3D12
{
	struct CPUHandle
	{
		CD3DX12_CPU_DESCRIPTOR_HANDLE handle;
		bool						  bValid;
		IndexT						  ind;
		CPUHandle()
			: bValid(true),
			  ind(InvalidIndex)
		{

		}
	};

class DescriptorHeap 
{
public:

	DescriptorHeap();

	~DescriptorHeap();

	virtual void Init(uint nCount);

	CPUHandle& GetCPUHandle();

	void ResetCPUHanlde(IndexT ind);

	CD3DX12_GPU_DESCRIPTOR_HANDLE GetGpuHandleFromCpu(const CPUHandle& handle);

	ID3D12DescriptorHeap*	GetHeap() const;

protected:

	ID3D12DescriptorHeap*			m_pDescriptorHeap;
	UINT							m_nDescriptorSize;
	std::vector<CPUHandle>			m_CPUHandleList;
};

inline void DescriptorHeap::ResetCPUHanlde(IndexT ind)
{
	if (ind != InvalidIndex)
	{
		m_CPUHandleList[ind].bValid = true;
		m_CPUHandleList[ind].ind	 = InvalidIndex;
	}
}

inline ID3D12DescriptorHeap* DescriptorHeap::GetHeap() const
{
	return m_pDescriptorHeap;
}

	class RTVHeap : public DescriptorHeap
{
public:
	RTVHeap();

	virtual ~RTVHeap();

	virtual void Init(uint nCount);

protected:
private:
};

	class DSVHeap : public DescriptorHeap
{
public:
	DSVHeap();

	virtual ~DSVHeap();

	virtual void Init(uint nCount);

protected:
private:
};

	class CSUHeap : public DescriptorHeap
{
public:
	CSUHeap();

	virtual ~CSUHeap();

	virtual void Init(uint nCount);
protected:
private:
};

}

#endif
