#pragma once
#include "D3D12Heads.h"
#include "base/ConstantBuffer.h"
#include "DescriptorHeap.h"

#if RENDERDEVICE_D3D12

namespace D3D12
{
	class DX12ConstantBuffer : public RenderBase::ConstantBuffer
{

public:

	DX12ConstantBuffer();

	virtual ~DX12ConstantBuffer();

	virtual void CreateBuffer(SizeT nSize);

	virtual void UpdateBuffer(const RenderBase::DataStream& data);

	virtual void Discard();

	ID3D12Resource*	GetBuffer() const;

	const CPUHandle& GetHandle() const;

private:

	ID3D12Resource*			m_pCBuffer;
	CPUHandle				m_CpuCbv;
	byte*					m_pMappedData;
};

inline ID3D12Resource* DX12ConstantBuffer::GetBuffer() const
{
	return m_pCBuffer;
}
inline const D3D12::CPUHandle& DX12ConstantBuffer::GetHandle() const
{
	return m_CpuCbv;
}


}

#endif