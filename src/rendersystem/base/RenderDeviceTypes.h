#ifndef RENDERDEVICETYPES_H_
#define RENDERDEVICETYPES_H_
#include <string>
#include <vector>
#include <memory>
#include "../config/RenderDeviceConfig.h"
#include "../foundation/math/float4.h"
//#include "../foundation/io/mediatype.h"

#include "../rendersystem/base/PixelFormat.h"
#include "../rendersystem/base/RenderCommandType.h"

namespace RenderBase
{
	typedef std::string ResourcePath;
	class SamplerObject;
	class BlendObject;
	class RasterizerObject;
	class DepthStencilObject;

	class RenderResourceHandle
	{
	public:
		RenderResourceHandle()
			: mRO(){
		}
		RenderResourceHandle(std::weak_ptr<RenderCommandType> ro)
			: mRO(ro){
		}

// 		const Core::RefCounted* AsObject(void) const{
// 			return static_cast<const Core::RefCounted*>(mRO);
// 		}
// 		Core::RefCounted* AsObject(void){
// 			return static_cast<Core::RefCounted*>(mRO);
// 		}

		std::shared_ptr<RenderCommandType> AsObject(void) const{
			return mRO.lock();
		}

		RenderCommandType::eRenderCMDType GetType() const{	
			return (mRO.expired())? RenderCommandType::InvalidRenderCMDType : mRO.lock()->GetRenderCommandType();
		}
		bool IsValid() const{
			return !mRO.expired();
		}

		operator bool() const{
			return !mRO.expired();
		}

		bool operator!=(const RenderResourceHandle& rhs) const{
			return this->mRO.lock() != rhs.mRO.lock();
		}

		bool operator==(const RenderResourceHandle& rhs) const{
			return this->mRO.lock() == rhs.mRO.lock();
		}

		bool operator<(const RenderResourceHandle& rhs) const{
			return this->mRO.lock() < rhs.mRO.lock();
		}
		//------------------------------------------------------------------------
		bool operator<=(const RenderResourceHandle& rhs) const{
			return this->mRO.lock() <= rhs.mRO.lock();
		}
		//------------------------------------------------------------------------
		bool operator>(const RenderResourceHandle& rhs) const{
			return this->mRO.lock() > rhs.mRO.lock();
		}
		//------------------------------------------------------------------------
		bool operator>=(const RenderResourceHandle& rhs) const{
			return this->mRO.lock() >= rhs.mRO.lock();
		}
	protected:
		// 线程安全： 绝对不允许直接在RenderSystem以外访问RenderCommandType的数据
		// 所有的访问都要通过RenderSystem来访问
		std::weak_ptr<RenderCommandType> mRO;
		friend class RenderSystem;
	};

	typedef RenderResourceHandle PrimitiveHandle;
	typedef RenderResourceHandle RenderTargetHandle;
	typedef RenderResourceHandle MultipleRenderTargetHandle;
	typedef RenderResourceHandle GPUProgramHandle;
	typedef RenderResourceHandle TextureHandle;
	typedef RenderResourceHandle RenderStateDescHandle;
	typedef RenderResourceHandle ConstanBufferHandle;
	typedef RenderResourceHandle SamplerObjectHandle;
	typedef RenderResourceHandle BlendObjectHandle;
	typedef RenderResourceHandle DepthStencilHandle;
	typedef RenderResourceHandle RasterizerHandle;
	typedef RenderResourceHandle RootSignatureHandle;
	typedef	RenderResourceHandle PsoHandle;

	enum FillMode
	{
		eFMPOINT,
		eFMWIREFRAME,
		eFMSOLID
	};

	struct FillModeConverter
	{
		/// convert from string
		static FillMode FromString(std::string str);
		/// convert to string
		static std::string ToString(FillMode code);
	};

	enum CullMode
	{
		eCMNONE,
		eCMFRONT,
		eCMBACK
	};

	struct CullModeConverter
	{
		/// convert from string
		static CullMode FromString(std::string str);
		/// convert to string
		static std::string ToString(CullMode code);
	};

	enum BlendOperation
	{
		eBOADD		= 1,
		eBOSUBSTRACT		= 2,
		eBOREVSUBTRACT	= 3,
		eBOMIN		= 4,
		eBOMAX		= 5,
	};

	struct BlendOperationConverter
	{
		/// convert from string
		static BlendOperation FromString(std::string str);
		/// convert to string
		static std::string ToString(BlendOperation bo);
	};

	enum BlendFactor
	{
		eBFZERO,
		eBFONE,
		eBFSRCCOLOR,
		eBFINVSRCCOLOR,		
		eBFSRCALPHA,		
		eBFINVSRCALPHA,
		eBFDSTALPHA,
		eBFINVDESTALPHA,		
		eBFDESTCOLOR,
		eBFINVDESTCOLOR,
		eBFSRCALPHASAT
	};

	struct AlphaBlendFactorConverter
	{
		/// convert from string
		static BlendFactor FromString(std::string str);
		/// convert to string
		static std::string ToString(BlendFactor bo);
	};

	enum CompareFunction
	{
		eCFNEVER,		
		eCFLESS,
		eCFEQUAL,
		eCFLESSEQUAL,
		eCFGREATER,
		eCFNOTEQUAL,
		eCFGREATEREQUAL,	
		eCFALWAYS,
	};

	struct CompareFunctionConverter
	{
		/// convert from string
		static CompareFunction FromString(std::string str);
		/// convert to string
		static std::string ToString(CompareFunction cf);
	};


	enum StencilOperation
	{
		eSOKEEP,
		eSOZERO,
		eSOREPLACE,
		eSOINCR,
		eSODECR,
		eSOINCRWRAP,
		eSODECRSAT,
		eSOINVERT

	};

	enum ColorMask
	{
		eCOLORMASKRED   = 1UL << 3,
		eCOLORMASKGREEN = 1UL << 2,
		eCOLORMASKBLUE  = 1UL << 1,
		eCOLORMASKALPHA = 1UL << 0,
		eCOLORMASKRGB   = eCOLORMASKRED | eCOLORMASKGREEN | eCOLORMASKBLUE,
		eCOLORMASKRGBA   = eCOLORMASKRED | eCOLORMASKGREEN | eCOLORMASKBLUE | eCOLORMASKALPHA
	};

	struct ColorMaskConverter
	{
		/// convert from string
		static ColorMask FromString(std::string str);
		/// convert to string
		static std::string ToString(ColorMask cm);
	};


	enum TextureAddressMode
	{
		eTAMWRAP,
		eTAMMIRROR,
		eTAMCLAMP,
		eTAMBORDER
	};

	struct TextureAddressModeConverter
	{
		/// convert from string
		static TextureAddressMode FromString(std::string str);
		/// convert to string
		static std::string ToString(TextureAddressMode tam);
	};

	enum TextureFilter
	{
		eTFPOINT,
		eTFLINEAR,
		eTFANISOTROPIC,
	};

	struct TextureFilterOperationConverter
	{
		/// convert from string
		static TextureFilter FromString(std::string str);
		/// convert to string
		static std::string ToString(TextureFilter tfo);
	};

	struct DeviceRasterizerState
	{
		FillMode			m_fillMode;
		CullMode			m_cullMode;
		float				m_slopScaleDepthBias;
		float				m_depthBias;
		bool				m_scissorTestEnable;
		bool				m_multisampleEnable;
		
		bool operator == (const DeviceRasterizerState& rhs)
		{
			return m_fillMode == rhs.m_fillMode && m_cullMode == rhs.m_cullMode;
		}	

		DeviceRasterizerState();
		~DeviceRasterizerState();
	};

	struct DeviceDepthAndStencilState
	{
		bool				m_depthEnable;
		bool				m_depthWriteMask;
		CompareFunction		m_zFunc;
		
		int                 m_stencilRef;
		bool				m_stencilEnable;
		CompareFunction		m_stencilFunc;
		unsigned short		m_stencilReadMask;
		unsigned short		m_stencilWriteMask;
		StencilOperation	m_stencilFail;
		StencilOperation	m_stencilZFail;
		StencilOperation	m_stencilPass;

		bool				m_stencilTwoEnable;
		CompareFunction		m_StencilTwoFunc;
		unsigned short		m_stencilTwoReadMask;
		unsigned short		m_stencilTwoWriteMask;
		StencilOperation	m_stencilTwoFail;
		StencilOperation	m_stencilTwoZFail;
		StencilOperation	m_stencilTwoPass;
		
		bool operator == (const DeviceDepthAndStencilState& rhs)
		{
			return m_depthEnable == rhs.m_depthEnable && m_depthWriteMask == rhs.m_depthWriteMask
				   && m_zFunc == rhs.m_zFunc;
		}

		DeviceDepthAndStencilState();
		~DeviceDepthAndStencilState();
	};

	struct DeviceBlendState
	{
		bool				m_alphaTestEnable;
		bool				m_separateAlphaBlendEnable;
		CompareFunction		m_alphaFunc;
		float				m_alphaRef;

		std::vector<bool>				m_alphaBlendEnable;
		std::vector<BlendOperation>		m_blendOP;
		std::vector<BlendFactor>		m_srcBlend;
		std::vector<BlendFactor>		m_destBlend;
		std::vector<BlendOperation>		m_blendOPAlpha;
		std::vector<BlendFactor>		m_srcBlendAlpha;
		std::vector<BlendFactor>		m_destBlendAlpha;
		std::vector<unsigned short int>	m_colorWriteMask;
		
		bool operator == (const DeviceBlendState& rhs)
		{
			return	m_alphaBlendEnable == rhs.m_alphaBlendEnable && m_blendOP[0] == rhs.m_blendOP[0] &&
					m_srcBlend[0] == rhs.m_srcBlend[0] && m_destBlend[0] == rhs.m_destBlend[0] 
					&& m_colorWriteMask[0] == rhs.m_colorWriteMask[0];
		}

		DeviceBlendState();
		~DeviceBlendState();
	};

	struct  BlendState
	{
		BlendOperation		m_blendOP;
		BlendFactor			m_srcBlend;
		BlendFactor			m_destBlend;
		BlendOperation		m_blendOPAlpha;
		BlendFactor			m_srcBlendAlpha;
		BlendFactor			m_destBlendAlpha;
		unsigned short int	m_colorWriteMask;

	};

#if RENDERDEVICE_OPENGLES30 || RENDERDEVICE_D3D9
	struct DeviceSamplerState
	{
		std::vector<bool>				m_textureIndexEnable;
		std::vector<TextureAddressMode> m_addressU;
		std::vector<TextureAddressMode> m_addressV;
		std::vector<TextureAddressMode> m_addressW;
		std::vector<std::string>		m_textureType;
		std::vector<TextureFilter>		m_Filter;
		std::vector<unsigned char>		m_maxAnisotropy;	
		DeviceSamplerState();
		~DeviceSamplerState();
	};
#elif RENDERDEVICE_D3D11 || RENDERDEVICE_D3D12
	struct DeviceSamplerState 
	{
		TextureAddressMode m_addressU;
		TextureAddressMode m_addressV;
		TextureAddressMode m_addressW;
		std::string	   m_textureType;
		TextureFilter	   m_Filter;
		unsigned char	   m_maxAnisotropy;
		IndexT			   m_bindpoint;
			
		bool operator == (const DeviceSamplerState& rhs)
		{
			return	m_addressU == rhs.m_addressU && m_addressV == rhs.m_addressV &&
					m_addressW == rhs.m_addressW && m_Filter == rhs.m_Filter;
		}

		DeviceSamplerState();
		~DeviceSamplerState();
	};
#endif

	class SignatureInfo
	{
	public:
		std::vector<DeviceSamplerState> samplerInfo;
		std::vector<IndexT> srvInfo;
		std::vector<IndexT> cbvInfo;
	};


	struct SamplerState
	{
		TextureAddressMode addressU;
		TextureAddressMode addressV;
		TextureAddressMode addressW;
		TextureFilter	   Filter;
	};

	class PrimitiveTopology
	{
	public:
		/// enumeration
		enum Code
		{
			InvalidPrimitiveTopology,

			PointList,
			LineList,
			LineStrip,
			TriangleList,
			TriangleStrip,
		};

		/// convert from string
		static Code FromString(const std::string& str);
		/// convert to string
		static std::string ToString(Code code);

	};


	class ImageFileFormat
	{
	public:
		/// image file formats
		enum Code
		{
			BMP,
			JPG,
			PNG,
			DDS,

			InvalidImageFileFormat,
		};

		/// convert from string
		static Code FromString(const std::string& str);
		/// convert to string
		static std::string ToString(Code c);
		/// convert from media type (MIME)
		//static Code FromMediaType(const IO::MediaType& mediaType);
		/// convert to media type (MIME)
		//static IO::MediaType ToMediaType(Code c);
	};

	class AntiAliasQuality
	{
	public:
		/// enum
		enum Code
		{
			None,
			Low,
			Medium,
			High,
		};

		/// convert from string
		static Code FromString(const std::string& str);
		/// convert to string
		static std::string ToString(Code code);
	};

	struct BoolModeConverter
	{ 
		/// convert from string
		static bool FromString(std::string str);
		/// convert to string
		static std::string ToString(bool b);
	};

	struct VertexLayoutDesc 
	{
		IndexT si;
		IndexT offset;
		std::string name;
	};

	class PsoData 
	{
	public:

		std::string					vsCode;
		std::string					psCode;
		void* vsByteCode;
		void* psByteCode;
		uint  vsLength;
		uint  psLength;
		std::vector<VertexLayoutDesc>	vertexLayout;
		DeviceBlendState				renderBlendState;
		DeviceRasterizerState			renderRasterizerState;
		DeviceDepthAndStencilState		renderDepthStencilState;
		PixelFormat::Code				rtvFormat;
		PixelFormat::Code				dsvFormat;

	protected:
	private:
	};

}

#endif //RENDERDEVICETYPES_H_