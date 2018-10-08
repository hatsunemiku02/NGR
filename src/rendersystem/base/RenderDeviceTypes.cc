

#include "stdneb.h"
#include "RenderDeviceTypes.h"
#include "SamplerObject.h"
#include "DepthStencilObject.h"
#include "BlendObject.h"
#include "RasterizerObject.h"
#ifdef __OSX__
#include "../rendersystem/config/RenderDeviceConfig.h"
#else
#include "config/RenderDeviceConfig.h"
#endif
namespace RenderBase
{

	FillMode FillModeConverter::FromString(std::string str)
	{
		if (str == "POINT") return eFMPOINT;
		else if (str == "LINE") return eFMWIREFRAME;
		else if (str == "FILL") return eFMSOLID;
		else
		{
			printf("FillModeConverter::FromString():Invalid FillMode: %s",str.c_str());
			assert(0);
			return eFMSOLID;
		}
	}

	std::string FillModeConverter::ToString(FillMode mode)
	{
		switch (mode)
		{
		case eFMPOINT:     return "POINT";
		case eFMWIREFRAME:    return "LINE";
		case eFMSOLID:     return "FILL";

		default:
			printf("FillModeConverter::ToString(): Invalid FillMode!");
			assert(0);
			return "";
		}
	}

	CullMode CullModeConverter::FromString(std::string str)
	{
		if (str == "NONE") return eCMNONE;
		else if (str == "FRONT") return eCMFRONT;
		else if (str == "BACK") return eCMBACK;
		else
		{
			printf("CullModeConverter::FromString():Invalid CullMode: %s",str.c_str());
			assert(0);
			return eCMNONE;
		}
	}

	std::string CullModeConverter::ToString(CullMode mode)
	{
		switch (mode)
		{
		case eCMNONE:     return "NONE";
		case eCMFRONT:    return "FRONT";
		case eCMBACK:     return "BACK";

		default:
			printf("CullModeConverter::ToString(): Invalid CullMode!");
			assert(0);
			return "";
		}
	}

	BlendOperation BlendOperationConverter::FromString(std::string str)
	{
		if (str == "ADD") return eBOADD;
		else if (str == "SUBTRACT") return eBOSUBSTRACT;
		else if (str == "REVSUBTRACT") return eBOREVSUBTRACT;
		else if (str == "MIN") return eBOMIN;
		else if (str == "MAX")	return eBOMAX;
		else
		{
			printf("BlendOperationConverter::FromString():Invalid BlendOperation: %s",str.c_str());
			assert(0);
			return eBOADD;
		}
	}

	std::string BlendOperationConverter::ToString(BlendOperation bo)
	{
		switch (bo)
		{
		case eBOADD:				return "ADD";
		case eBOSUBSTRACT:				return "SUBTRACT";
		case eBOREVSUBTRACT:			return "REVSUBTRACT";
		case eBOMIN:				return "MIN";
		case eBOMAX:				return "MAX";

		default:
			printf("BlendOperationConverter::ToString(): Invalid BlendOperation!");
			assert(0);
			return "";
		}

	}

	BlendFactor AlphaBlendFactorConverter::FromString(std::string str)
	{
		if (str == "ZERO") return eBFZERO;
		else if (str == "ONE") return eBFONE;
		else if (str == "SRCALPHA") return eBFSRCALPHA;
		else if (str == "DESTALPHA") return eBFDSTALPHA;
		else if (str == "INVSRCALPHA") return eBFINVSRCALPHA;
		else if (str == "INVDESTALPHA") return eBFINVDESTALPHA;
		else if (str == "SRCCOLOR") return eBFSRCCOLOR;
		else if (str == "DESTCOLOR") return eBFDESTCOLOR;
		else if (str == "INVSRCCOLOR") return eBFINVSRCCOLOR;
		else if (str == "INVDESTCOLOR") return eBFINVDESTCOLOR;
		else if (str == "SRCALPHASAT") return eBFSRCALPHASAT;
		else
		{
			printf("AlphaBlendFactorConverter::FromString():Invalid AlphaBlendFactor: %s",str.c_str());
			assert(0);
			return eBFZERO;
		}
	}

	std::string AlphaBlendFactorConverter::ToString(BlendFactor abf)
	{
		switch (abf)
		{
		case eBFZERO:     return "ZERO";
		case eBFONE:		return "ONE";
		case eBFSRCALPHA:			return "SRCALPHA";
		case eBFDSTALPHA:		return "DESTALPHA";
		case eBFINVSRCALPHA:			return "INVSRCALPHA";
		case eBFINVDESTALPHA:		return "INVDESTALPHA";
		case eBFSRCCOLOR:	return "SRCCOLOR";
		case eBFDESTCOLOR:		return "DESTCOLOR";
		case eBFINVSRCCOLOR:		return "INVSRCCOLOR";
		case eBFINVDESTCOLOR:		return "INVDESTCOLOR";
		case eBFSRCALPHASAT:		return "DESTCOLOR";

		default:
			printf("AlphaBlendFactorConverter::ToString(): Invalid AlphaBlendFactor!");
			assert(0);
			return "";
		}

	}

	ColorMask ColorMaskConverter::FromString(std::string str)
	{
		if (str == "RGBA") return eCOLORMASKRGBA;
		else if (str == "R") return eCOLORMASKRED;
		else if (str == "G") return eCOLORMASKGREEN;
		else if (str == "B") return eCOLORMASKBLUE;
		else if (str == "A") return eCOLORMASKALPHA;
		else if (str == "RGB") return eCOLORMASKRGB;
		else
		{
			printf("ColorMaskConverter::FromString():Invalid ColorMask: %s",str.c_str());
			assert(0);
			return eCOLORMASKRGBA;
		}
	}

	std::string ColorMaskConverter::ToString(ColorMask cm)
	{
		switch (cm)
		{
		case eCOLORMASKRGBA:		return "RGBA";
		case eCOLORMASKRED:     return "R";
		case eCOLORMASKGREEN:		return "G";
		case eCOLORMASKBLUE:			return "B";
		case eCOLORMASKALPHA:		return "A";
		case eCOLORMASKRGB:			return "RGB";
		default:
			printf("ColorMaskConverter::ToString(): invalid ColorMask!");
			assert(0);
			return "";
		}
	}

	CompareFunction CompareFunctionConverter::FromString(std::string str)
	{
		if (str == "NEVER") return eCFNEVER;
		else if (str == "ALWAYS") return eCFALWAYS;
		else if (str == "LESS") return eCFLESS;
		else if (str == "LESSEQUAL") return eCFLESSEQUAL;
		else if (str == "EQUAL") return eCFEQUAL;
		else if (str == "NOTEQUAL") return eCFNOTEQUAL;
		else if (str == "GREATEREQUAL") return eCFGREATEREQUAL;
		else if (str == "GREATER") return eCFGREATER;
		else
		{
			printf("CompareFunctionConverter::FromString():Invalid CompareFunction: %s",str.c_str());
			assert(0);
			return eCFNEVER;
		}
	}

	std::string CompareFunctionConverter::ToString(CompareFunction cf)
	{
		switch (cf)
		{
		case eCFNEVER:     return "NEVER";
		case eCFALWAYS:		return "ALWAYS";
		case eCFLESS:			return "LESS";
		case eCFLESSEQUAL:		return "LESSEQUAL";
		case eCFEQUAL:			return "EQUAL";
		case eCFNOTEQUAL:		return "NOTEQUAL";
		case eCFGREATEREQUAL:	return "GREATEREQUAL";
		case eCFGREATER:		return "GREATER";

		default:
			printf("CompareFunctionConverter::ToString(): Invalid CompareFunction!");
			assert(0);
			return "";
		}

	}

	TextureAddressMode TextureAddressModeConverter::FromString(std::string str)
	{
		if (str == "WRAP") return eTAMWRAP;
		else if (str == "CLAMP") return eTAMCLAMP;
		else if (str == "MIRROR") return eTAMMIRROR;
		else if (str == "BORDER") return eTAMBORDER;
		else
		{
			printf("TextureAddressModeConverter::FromString():Invalid TextureAddressMode: %s",str.c_str());
			assert(0);
			return eTAMWRAP;
		}
	}

	std::string TextureAddressModeConverter::ToString(TextureAddressMode tam)
	{
		switch (tam)
		{
		case eTAMWRAP:      return "WRAP";
		case eTAMCLAMP:		return "CLAMP";
		case eTAMMIRROR:	return "MIRROR";
		case eTAMBORDER:		return "BORDER";

		default:
			printf("TextureAddressModeConverter::ToString(): invalid TextureAddressMode!");
			assert(0);
			return "";
		}

	}

	TextureFilter TextureFilterOperationConverter::FromString(std::string str)
	{
		if (str == "POINT") return eTFPOINT;
		else if (str == "LINEAR") return eTFLINEAR;
		else if (str == "ANISOTROPIC") return eTFANISOTROPIC;
		else
		{
			printf("TextureFilterOperationConverter::FromString():Invalid TextureFilterOperation: %s",str.c_str());
			assert(0);
			return eTFPOINT;
		}
	}

	std::string TextureFilterOperationConverter::ToString(TextureFilter tfo)
	{
		printf("TexFilterOpConverter::ToString():not implemented!");
		assert(0);
		return "";
	}

	DeviceRasterizerState::DeviceRasterizerState()
		: m_fillMode(eFMSOLID),
		m_cullMode(eCMBACK),
		m_slopScaleDepthBias(0),
		m_depthBias(0),
		m_scissorTestEnable(false),
		m_multisampleEnable(true)
	
	{
	}

	DeviceRasterizerState::~DeviceRasterizerState()
	{
		
	}

	DeviceDepthAndStencilState::DeviceDepthAndStencilState()
		: m_depthEnable(true),
		m_depthWriteMask(true),
		m_zFunc(eCFLESSEQUAL),
		m_stencilRef(0),
		m_stencilEnable(false),
		m_stencilFunc(eCFALWAYS),
		m_stencilReadMask(0xFFFF),
		m_stencilWriteMask(0xFFFF),
		m_stencilFail(eSOKEEP),
		m_stencilZFail(eSOKEEP),
		m_stencilPass(eSOKEEP),
		m_stencilTwoEnable(false),
		m_StencilTwoFunc(eCFALWAYS),
		m_stencilTwoReadMask(0xFFFF),
		m_stencilTwoWriteMask(0xFFFF),
		m_stencilTwoFail(eSOKEEP),
		m_stencilTwoZFail(eSOKEEP),
		m_stencilTwoPass(eSOKEEP)
		
		
	{
	}

	DeviceDepthAndStencilState::~DeviceDepthAndStencilState()
	{
		
	}

	DeviceBlendState::DeviceBlendState()
		: m_alphaTestEnable(false),
		m_alphaFunc(eCFALWAYS),
		m_alphaRef(0),
		m_separateAlphaBlendEnable(false)
	{
		m_alphaBlendEnable.resize(10,false);
		m_blendOP.resize(10,eBOADD);
		m_srcBlend.resize(10,eBFZERO);
		m_destBlend.resize(10,eBFONE);
		m_blendOPAlpha.resize(10,eBOADD);
		m_srcBlendAlpha.resize(10,eBFZERO);
		m_destBlendAlpha.resize(10,eBFONE);
		m_colorWriteMask.resize(10,eCOLORMASKRGBA);

	}

	DeviceBlendState::~DeviceBlendState()
	{
		
	}

#if RENDERDEVICE_D3D9 || RENDERDEVICE_OPENGLES30
	DeviceSamplerState::DeviceSamplerState()
	{
		m_textureIndexEnable.Fill(0,16,false);
		m_addressU.Fill(0,16,eTAMWRAP);
		m_addressV.Fill(0,16,eTAMWRAP);
		m_addressW.Fill(0,16,eTAMWRAP);
		m_Filter.Fill(0,16,eTFPOINT);
		m_maxAnisotropy.Fill(0,16,16);
		m_textureType.Fill(0, 16, "Texture2D");	
	}

	DeviceSamplerState::~DeviceSamplerState()
	{

	}
#elif  RENDERDEVICE_D3D11 || RENDERDEVICE_D3D12
	DeviceSamplerState::DeviceSamplerState()
		: m_addressU(eTAMWRAP),
		  m_addressV(eTAMWRAP),
		  m_addressW(eTAMWRAP),
		  m_Filter(eTFPOINT),
		  m_maxAnisotropy(16),
		  m_bindpoint(0)
		
	{

	}
	DeviceSamplerState::~DeviceSamplerState()
	{
		
	}
#endif



	//------------------------------------------------------------------------------
	/**
	*/
	PrimitiveTopology::Code
		PrimitiveTopology::FromString(const std::string& str)
	{
		if (str == "PointList")             return PointList;
		else if (str == "LineList")         return LineList;
		else if (str == "LineStrip")        return LineStrip;
		else if (str == "TriangleList")     return TriangleList;
		else if (str == "TriangleStrip")    return TriangleStrip;
		else
		{
			printf("PrimitiveTopology::FromString(): invalid string '%s'!", str.c_str());
			assert(0);
			return InvalidPrimitiveTopology;
		}
	}

	//------------------------------------------------------------------------------
	/**
	*/
	std::string
		PrimitiveTopology::ToString(Code code)
	{
		switch (code)
		{
		case PointList:     return "PointList";
		case LineList:      return "LineList";
		case LineStrip:     return "LineStrip";
		case TriangleList:  return "TriangleList";
		case TriangleStrip: return "TriangleStrip";

		default:
			printf("PrimitiveTopology::ToString(): invalid topology code!");
			assert(0);
			return "";
		}
	}

	//------------------------------------------------------------------------------
	/**
	*/
	ImageFileFormat::Code
		ImageFileFormat::FromString(const std::string& str)
	{
		if (str == "bmp") return BMP;
		else if (str == "jpg") return JPG;
		else if (str == "png") return PNG;
		else if (str == "dds") return DDS;
		else
		{
			return InvalidImageFileFormat;
		}
	}

	//------------------------------------------------------------------------------
	/**
	*/
	std::string
		ImageFileFormat::ToString(ImageFileFormat::Code c)
	{
		switch (c)
		{
		case BMP:   return "bmp";
		case JPG:   return "jpg";
		case PNG:   return "png";
		case DDS:   return "dds";
		default:
			printf("ImageFileFormat::ToString(): invalid image file format code!");
			assert(0);
			return "";
		}
	}

	//------------------------------------------------------------------------------
	/**
	*/
// 	ImageFileFormat::Code
// 		ImageFileFormat::FromMediaType(const IO::MediaType& mediaType)
// 	{
// 		assert(mediaType.IsValid());
// 		if (mediaType.GetType() != "image")
// 		{
// 			return InvalidImageFileFormat;
// 		}
// 		else
// 		{
// 			const String& subType = mediaType.GetSubType();
// 			if (subType == "bmp")       return BMP;
// 			else if (subType == "jpeg") return JPG;
// 			else if (subType == "png")  return PNG;
// 			else if (subType == "dds")  return DDS; // hmm... non-standard
// 			else
// 			{
// 				return InvalidImageFileFormat;
// 			}
// 		}
// 	}
// 
// 	//------------------------------------------------------------------------------
// 	/**
// 	*/
// 	IO::MediaType
// 		ImageFileFormat::ToMediaType(Code c)
// 	{
// 		switch (c)
// 		{
// 		case BMP:   return IO::MediaType("image", "bmp");
// 		case JPG:   return IO::MediaType("image", "jpeg");
// 		case PNG:   return IO::MediaType("image", "png");
// 		case DDS:   return IO::MediaType("image", "dds");   // hmm... non-standard
// 		default:
// 			n_error("ImageFileFormat::ToMediaType(): invalid image file format code!");
// 			return IO::MediaType();
// 		}
// 	}

	//------------------------------------------------------------------------------
	/**
	*/
	AntiAliasQuality::Code
		AntiAliasQuality::FromString(const std::string& str)
	{
		if ("None" == str) return None;
		else if ("Low" == str) return Low;
		else if ("Medium" == str) return Medium;
		else if ("High" == str) return High;
		else
		{
			printf("Invalid antialias quality string '%s'!", str.c_str());
			assert(0);
			return None;
		}
	}

	//------------------------------------------------------------------------------
	/**
	*/
	std::string
		AntiAliasQuality::ToString(Code code)
	{
		switch (code)
		{
		case None:      return "None";
		case Low:       return "Low";
		case Medium:    return "Medium";
		case High:      return "High";
		default:
			printf("Invalid antialias quality code!");
			assert(0);
			return "";
		}
	}

	//------------------------------------------------------------------------------
	/**
	*/
	bool 
		BoolModeConverter::FromString(std::string str)
	{
		if ("FALSE" == str) return false;
		else if ("TRUE" == str) return true;
		else
		{
			printf("Invalid bool string '%s'!", str.c_str());
			assert(0);
			return false;
		}

	}

	//------------------------------------------------------------------------------
	/**
	*/
	std::string 
		BoolModeConverter::ToString(bool b)
	{
		if (b)
		{
			return "TRUE";
		} 
		else
		{
			return "FALSE";
		}
	}


}