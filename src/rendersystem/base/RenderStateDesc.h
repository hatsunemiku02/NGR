#ifndef RENDERSTATEDESC_H_
#define RENDERSTATEDESC_H_
#include "RenderCommandType.h"
#include "RenderDeviceTypes.h"

#include <map>

namespace RenderBase
{

	class RenderStateDesc : public RenderCommandType
	{
	public:
		enum RenderStateFlag
		{
			eInvalidRenderState = 0,
			eRenderSamplerState = 1,
			eRenderDepthAndStencilState = 2,
			eRenderBlendState = 4,
			eRenderRasterizerState = 8,
			eAllRenderState = (eRenderSamplerState|eRenderDepthAndStencilState|eRenderBlendState|eRenderRasterizerState)
		};

		RenderStateDesc();
		virtual ~RenderStateDesc();
		void Setup();

		void SetSamplerState(const uint& mask, const DeviceSamplerState& state);
		

		void SetSamplerState(const std::map<uint, std::vector<DeviceSamplerState> >& state);
		const std::map<uint, std::vector<DeviceSamplerState> >& GetSamplerState() const;
		std::vector<DeviceSamplerState>& GetSamplerState(const uint& mask);

		void SetBlendState(const DeviceBlendState& state);
		const DeviceBlendState& GetBlendState() const;

		void SetRasterizerState(const DeviceRasterizerState& state);
		const DeviceRasterizerState& GetRasterizerState() const;

		void SetDepthAndStencilState(const DeviceDepthAndStencilState& state);
		const DeviceDepthAndStencilState& GetDepthAndStencilState() const;

		void SetUpdateFlag(unsigned int flag);
		unsigned int GetUpdateFlag() const;

	private:
		std::map<uint, std::vector<DeviceSamplerState> >		m_renderSamplerState;
		
		DeviceBlendState		m_renderBlendState;
		DeviceRasterizerState	m_renderRasterizerState;
		DeviceDepthAndStencilState	m_renderDepthStencilState;
		unsigned int			m_updateFlag;
	};


	inline void
		RenderStateDesc::SetSamplerState(const uint& mask, const DeviceSamplerState& state)
	{
		std::map<uint, std::vector<DeviceSamplerState> >::iterator res = m_renderSamplerState.find(mask);
		if (res == m_renderSamplerState.end())
		{
			std::vector<DeviceSamplerState> dsList;
			dsList.push_back(state);
			m_renderSamplerState.insert(std::make_pair(mask, dsList));
		}
		else
		{
			res->second.push_back(state);
		}		
	}

	inline void RenderStateDesc::SetSamplerState(const std::map<uint, std::vector<DeviceSamplerState> >& state)	
	{
		m_renderSamplerState = state;
	}

	inline  std::vector<DeviceSamplerState>& RenderStateDesc::GetSamplerState(const uint& mask)
	{

		return m_renderSamplerState[mask];
	
	}


	inline const std::map<uint, std::vector<DeviceSamplerState> >& RenderStateDesc::GetSamplerState() const
	{
		return m_renderSamplerState;
	}

	inline void
		RenderStateDesc::SetBlendState(const DeviceBlendState& state)
	{
		m_renderBlendState = state;
		m_updateFlag |= eRenderBlendState;
	}

	inline const DeviceBlendState&
		RenderStateDesc::GetBlendState() const
	{
		return m_renderBlendState;
	}

	inline void
		RenderStateDesc::SetRasterizerState(const DeviceRasterizerState& state)
	{
		m_renderRasterizerState = state;
		m_updateFlag |= eRenderRasterizerState;
	}

	inline const DeviceRasterizerState&
		RenderStateDesc::GetRasterizerState() const
	{
		return m_renderRasterizerState;
	}

	inline void
		RenderStateDesc::SetDepthAndStencilState(const DeviceDepthAndStencilState& state)
	{
		m_renderDepthStencilState = state;
		m_updateFlag |= eRenderDepthAndStencilState;
	}

	inline const DeviceDepthAndStencilState&
		RenderStateDesc::GetDepthAndStencilState() const
	{
		return m_renderDepthStencilState;
	}

	inline void
		RenderStateDesc::SetUpdateFlag(unsigned int flag)
	{
		m_updateFlag = flag;
	}
	inline unsigned int
		RenderStateDesc::GetUpdateFlag() const
	{
		return m_updateFlag;
	}
}
#endif
