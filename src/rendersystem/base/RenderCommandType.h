#ifndef RENDERCOMMANDTYPE_H_
#define RENDERCOMMANDTYPE_H_
#include "RenderResource.h"

namespace RenderBase
{
	class RenderCommandType : public RenderResource
	{
	public:
		RenderCommandType();

		virtual ~RenderCommandType();

		enum eRenderCMDType
		{
			InvalidRenderCMDType = -1,
			DrawPrimGroup,
			SetGPUProgram,
			SetRenderTarget,
			SetMultiRenderTarget,
			SetTexture,
			SetRenderState,
			CreateConstantBuffer,
			CreateSamplerObject,
			RenderCMDTypeCount
		};
		
		void SetRenderCommandType(eRenderCMDType type);

		const eRenderCMDType GetRenderCommandType() const;
	protected:
		eRenderCMDType m_rcType;
	};

	inline void 
		RenderCommandType::SetRenderCommandType(eRenderCMDType type)
	{
		m_rcType = type;
	}

	inline const RenderCommandType::eRenderCMDType
		RenderCommandType::GetRenderCommandType() const
	{
		return m_rcType;
	}

	inline
		RenderCommandType::RenderCommandType() : m_rcType(InvalidRenderCMDType)
	{

	}

	inline
		RenderCommandType::~RenderCommandType()
	{

	}
}
#endif//RENDERCOMMANDTYPE_H_