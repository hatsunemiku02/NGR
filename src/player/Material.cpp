#include "Material.h"

Material::Material()
{

}

Material::~Material()
{

}

void Material::SetShader(void* vs, uint vl, void* ps, uint pl)
{
	m_PsoData.vsByteCode = vs;
	m_PsoData.vsLength = vl;
	m_PsoData.psByteCode = ps;
	m_PsoData.psLength = pl;
}



