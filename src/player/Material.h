#pragma once
#include "base/RenderDeviceTypes.h"

class Material
{
public:
	Material();
	~Material();
	
	void SetShader(void* vs,uint vl,void* ps,uint pl);

	RenderBase::PsoData GetPsoData();
	RenderBase::SignatureInfo GetRootSigInfo();
protected:
	RenderBase::PsoData m_PsoData;
	RenderBase::SignatureInfo m_SigInfo;
};


inline RenderBase::PsoData Material::GetPsoData()
{
	return m_PsoData;
}

inline RenderBase::SignatureInfo Material::GetRootSigInfo()
{
	return m_SigInfo;
}