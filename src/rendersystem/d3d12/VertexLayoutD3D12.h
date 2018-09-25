#pragma once

#include "D3D12Heads.h"

#if RENDERDEVICE_D3D12

#include "base/VertexLayout.h"
#include <vector>
#include <map>

class RenderBase::VertexComponent;

namespace D3D12
{
	class VertexLayoutD3D12 : public RenderBase::VertexLayout
	{

	public:

		/// constructor
		VertexLayoutD3D12();
		/// destructor
		virtual ~VertexLayoutD3D12();

		void GenerateDeclarationD3D12();

	private:

		std::vector<D3D12_INPUT_ELEMENT_DESC> m_InputDescCollection;

	};


}

#endif