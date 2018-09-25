#pragma once

#ifndef GPUPROGRAM_H_
#define GPUPROGRAM_H_
#include "RenderDeviceTypes.h"
#include "RenderCommandType.h"
namespace RenderBase
{
	class GPUProgram : public RenderCommandType
	{
	public:
		/// constructor
		GPUProgram();
		/// destructor
		virtual ~GPUProgram();
		/// set GPU code file path
		void SetCodePath(const ResourcePath& path);
		/// get GPU code file path
		const ResourcePath& GetCodePath() const;
		/// set vertex shader code
		void SetVertexCode(const std::string& vcode);
		/// set pixel shader code
		void SetPixelCode(const std::string& pcode);
		/// get vertex shader code
		const std::string& GetVertexCode() const;
		/// get pixel shader code
		const std::string& GetPixelCode() const;

		const std::string& GetID(void) const;

		void SetID(const std::string& id);

	protected:
		ResourcePath m_codePath;
		std::string m_vertexCode;
		std::string m_pixelCode;
		std::string m_id;
	};

	inline void GPUProgram::SetCodePath(const ResourcePath& path)
	{
		m_codePath = path;
	}

	inline const ResourcePath& GPUProgram::GetCodePath() const
	{
		return m_codePath;
	}


	inline void GPUProgram::SetVertexCode(const std::string& vcode)
	{
		m_vertexCode = vcode;
	}

	inline const std::string& GPUProgram::GetVertexCode() const
	{
		return m_vertexCode;
	}

	inline void GPUProgram::SetPixelCode(const std::string& pcode)
	{
		m_pixelCode = pcode;
	}

	inline const std::string& GPUProgram::GetPixelCode() const
	{
		return m_pixelCode;
	}
	
	inline const std::string& GPUProgram::GetID(void) const
	{
		return m_id;
	}

	inline void GPUProgram::SetID(const std::string& id)
	{
		m_id = id;
	}
}

#endif