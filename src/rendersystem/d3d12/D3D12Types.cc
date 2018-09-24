#include "../config/RenderDeviceConfig.h"
#if RENDERDEVICE_D3D12
#include "stdneb.h"
#include "D3D12Types.h"

namespace D3D12
{
	using namespace RenderBase;

PixelFormat::Code D3D12Types::AsGenesisPixelFormat(DXGI_FORMAT f)
{
		switch (f)
		{
		case DXGI_FORMAT_B8G8R8A8_UNORM:           return PixelFormat::A8R8G8B8;
		case DXGI_FORMAT_B5G6R5_UNORM:             return PixelFormat::R5G6B5;
		case DXGI_FORMAT_B5G5R5A1_UNORM:           return PixelFormat::A1R5G5B5;
		case DXGI_FORMAT_R8G8B8A8_UNORM:           return PixelFormat::R8G8B8A8;
		case DXGI_FORMAT_BC1_UNORM:               return PixelFormat::DXT1;
		case DXGI_FORMAT_BC3_UNORM:               return PixelFormat::DXT3;
		case DXGI_FORMAT_BC5_UNORM:               return PixelFormat::DXT5;
		case DXGI_FORMAT_R16_FLOAT:               return PixelFormat::R16F;
		case DXGI_FORMAT_R16G16_FLOAT:            return PixelFormat::G16R16F;
		case DXGI_FORMAT_R16G16B16A16_FLOAT:      return PixelFormat::A16B16G16R16F;
		case DXGI_FORMAT_R32_FLOAT:               return PixelFormat::R32F;
		case DXGI_FORMAT_R32G32_FLOAT:            return PixelFormat::G32R32F;
		case DXGI_FORMAT_R32G32B32A32_FLOAT:      return PixelFormat::A32B32G32R32F;
		case DXGI_FORMAT_A8_UNORM:                 return PixelFormat::A8;
		case DXGI_FORMAT_R16G16_UNORM:             return PixelFormat::G16R16;
		case DXGI_FORMAT_D24_UNORM_S8_UINT:              return PixelFormat::D24S8;
		case DXGI_FORMAT_R8G8_UNORM:				return PixelFormat::L8A8; //[2012/4/12 zhongdaohuan] mygui 用到的格式


		default:                        return PixelFormat::InvalidPixelFormat;
		}
}

DXGI_FORMAT D3D12Types::AsD3D12PixelFormat(PixelFormat::Code p)
{
		switch (p)
		{
		case PixelFormat::X8R8G8B8:
		case PixelFormat::A8R8G8B8:			return DXGI_FORMAT_B8G8R8A8_UNORM;
		case PixelFormat::R5G6B5:           return DXGI_FORMAT_B5G6R5_UNORM;
		case PixelFormat::A1R5G5B5:         return DXGI_FORMAT_B5G5R5A1_UNORM;
		case PixelFormat::A4R4G4B4:         return DXGI_FORMAT_R8G8B8A8_UNORM;
		case PixelFormat::DXT1:             return DXGI_FORMAT_BC1_UNORM;
		case PixelFormat::DXT3:             return DXGI_FORMAT_BC3_UNORM;
		case PixelFormat::DXT5:             return DXGI_FORMAT_BC5_UNORM;
		case PixelFormat::R16F:             return DXGI_FORMAT_R16_FLOAT;
		case PixelFormat::G16R16F:          return DXGI_FORMAT_R16G16_FLOAT;
		case PixelFormat::A16B16G16R16F:    return DXGI_FORMAT_R16G16B16A16_FLOAT;
		case PixelFormat::R32F:             return DXGI_FORMAT_R32_FLOAT;
		case PixelFormat::G32R32F:          return DXGI_FORMAT_R32G32_FLOAT;
		case PixelFormat::A32B32G32R32F:    return DXGI_FORMAT_R32G32B32A32_FLOAT;
		case PixelFormat::A8:               return DXGI_FORMAT_A8_UNORM;
		case PixelFormat::L8:				return DXGI_FORMAT_R8_UNORM; 
		case PixelFormat::L8A8:				return DXGI_FORMAT_R8G8_UNORM; //[2012/4/12 zhongdaohuan] mygui 用到的格式
		case PixelFormat::G16R16:           return DXGI_FORMAT_R16G16_UNORM;
		case PixelFormat::D24X8:            return DXGI_FORMAT_D24_UNORM_S8_UINT;
		case PixelFormat::D24S8:            return DXGI_FORMAT_D24_UNORM_S8_UINT;

		case PixelFormat::R8G8B8:           return DXGI_FORMAT_R8G8B8A8_UNORM;

		default:                            return DXGI_FORMAT_UNKNOWN;
		}
}

DXGI_FORMAT D3D12Types::AsD3D12VertexDeclarationType(VertexComponent::Format f)
{
		switch (f)
		{
		case VertexComponent::Float:    return DXGI_FORMAT_R32_FLOAT;
		case VertexComponent::Float2:   return DXGI_FORMAT_R32G32_FLOAT;
		case VertexComponent::Float3:   return DXGI_FORMAT_R32G32B32_FLOAT;
		case VertexComponent::Float4:   return DXGI_FORMAT_R32G32B32A32_FLOAT;
		case VertexComponent::UByte4:   return DXGI_FORMAT_R8G8B8A8_UINT;
		case VertexComponent::Short2:   return DXGI_FORMAT_R16G16_UINT;
		case VertexComponent::Short4:   return DXGI_FORMAT_R16G16B16A16_UINT;
		case VertexComponent::UByte4N:  return DXGI_FORMAT_R8G8B8A8_UNORM;
		case VertexComponent::Short2N:  return DXGI_FORMAT_R16G16_UNORM;
		case VertexComponent::Short4N:  return DXGI_FORMAT_R16G16B16A16_UNORM;
		case VertexComponent::ColorBGRA:	return DXGI_FORMAT_R8G8B8A8_UNORM;
		default:                        
			printf("D3D11Types::AsDirect3DVertexDeclarationType(): invalid input parameter!");
			assert(0);
			return DXGI_FORMAT_UNKNOWN;
		}
}

UINT D3D12Types::AsD3D12MultiSampleType(AntiAliasQuality::Code c)
{
	switch (c)
	{
	case AntiAliasQuality::None:
		return 0;
	case AntiAliasQuality::Low:
		return 2;
	case AntiAliasQuality::Medium:
		return 4;
	case AntiAliasQuality::High:
		return 8;
	default:
		return 0;
	}
}

LPCSTR D3D12Types::AsD3D12SemanticName(VertexComponent::SemanticName n)
{
	switch (n)
	{
	case VertexComponent::Position:     return "POSITION";
	case VertexComponent::Normal:       return "NORMAL";
	case VertexComponent::Tangent:      return "TANGENT";
	case VertexComponent::TexCoord:     return "TEXCOORD";
	case VertexComponent::SkinWeights:  return "BLENDWEIGHT";
	case VertexComponent::SkinJIndices: return "BLENDINDICES";
	case VertexComponent::Size:			return "PSIZE";
	case VertexComponent::Color:        return "COLOR";
	default:
		printf("D3D12Types::AsDirect3DVertexDeclarationUsage(): invalid input parameter!");
		assert(0);
		return "INVALID";
	}
}

D3D12_TEXTURE_ADDRESS_MODE D3D12Types::AsD3D12TextureAddress(TextureAddressMode mode)
{
	switch (mode)
	{
	case eTAMCLAMP:
		return D3D12_TEXTURE_ADDRESS_MODE_CLAMP;

	case eTAMWRAP:
		return D3D12_TEXTURE_ADDRESS_MODE_WRAP;

	case eTAMMIRROR:
		return D3D12_TEXTURE_ADDRESS_MODE_MIRROR;

	case eTAMBORDER:
		return D3D12_TEXTURE_ADDRESS_MODE_BORDER;

	default:
		assert(false);
		return D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
	}
}

D3D12_FILTER D3D12Types::AsD3D12TextureFilter(TextureFilter filter)
{
	switch (filter)
	{
	case eTFPOINT:
		return D3D12_FILTER_MIN_MAG_MIP_POINT;
	case eTFLINEAR:
		return D3D12_FILTER_MIN_MAG_MIP_LINEAR;
	case RenderBase::eTFANISOTROPIC:
		return D3D12_FILTER_ANISOTROPIC;
	default:
		return D3D12_FILTER_MIN_MAG_MIP_POINT;
	}
}

D3D12_CULL_MODE D3D12Types::AsD3D12CullMode(CullMode mode)
{
	switch (mode)
	{
	case eCMNONE:
		return D3D12_CULL_MODE_NONE;

	case eCMFRONT:
		return D3D12_CULL_MODE_FRONT;

	case eCMBACK:
		return D3D12_CULL_MODE_BACK;

	default:
		assert(false);
		return D3D12_CULL_MODE_NONE;
	}
}

D3D12_FILL_MODE D3D12Types::AsD3D12FillMode(FillMode mode)
{
	switch (mode)
	{

	case eFMWIREFRAME:
		return D3D12_FILL_MODE_WIREFRAME;

	case eFMSOLID:
		return D3D12_FILL_MODE_SOLID;

	default:
		assert(false);
		return D3D12_FILL_MODE_WIREFRAME;
	}
}

D3D12_BLEND D3D12Types::AsD3D12BlendFactor(BlendFactor factor)
{
	switch (factor)
	{
	case eBFZERO:
		return D3D12_BLEND_ZERO;
	case eBFONE:
		return D3D12_BLEND_ONE;
	case eBFSRCCOLOR:
		return D3D12_BLEND_SRC_COLOR;
	case eBFINVSRCCOLOR:
		return D3D12_BLEND_INV_SRC_COLOR;
	case eBFSRCALPHA:
		return D3D12_BLEND_SRC_ALPHA;
	case eBFINVSRCALPHA:
		return D3D12_BLEND_INV_SRC_ALPHA;
	case eBFDSTALPHA:
		return D3D12_BLEND_DEST_ALPHA;
	case eBFINVDESTALPHA:
		return D3D12_BLEND_INV_DEST_ALPHA;
	case eBFDESTCOLOR:
		return D3D12_BLEND_DEST_COLOR;
	case eBFINVDESTCOLOR:
		return D3D12_BLEND_INV_DEST_COLOR;
	case eBFSRCALPHASAT:
		return D3D12_BLEND_SRC_ALPHA_SAT;

	default:
		assert(false);
		return D3D12_BLEND_ZERO;
	}
}

D3D12_BLEND_OP D3D12Types::AsD3D12BlendOperation(BlendOperation blendop)
{
	switch (blendop)
	{
	case eBOADD:
		return D3D12_BLEND_OP_ADD;

	case eBOSUBSTRACT:
		return D3D12_BLEND_OP_SUBTRACT;

	case eBOREVSUBTRACT:
		return D3D12_BLEND_OP_REV_SUBTRACT;

	case eBOMIN:
		return D3D12_BLEND_OP_MIN;

	case eBOMAX:
		return D3D12_BLEND_OP_MAX;

	default:
		assert(false);
		return D3D12_BLEND_OP_ADD;
	}
}

D3D12_COMPARISON_FUNC D3D12Types::AsD3D12CompareFunction(CompareFunction func)
{
	switch (func)
	{
	case eCFNEVER:
		return D3D12_COMPARISON_FUNC_NEVER;
	case eCFLESS:
		return D3D12_COMPARISON_FUNC_LESS;
	case eCFEQUAL:
		return D3D12_COMPARISON_FUNC_EQUAL;
	case eCFLESSEQUAL:
		return D3D12_COMPARISON_FUNC_LESS_EQUAL;
	case eCFGREATER:
		return D3D12_COMPARISON_FUNC_GREATER;
	case eCFNOTEQUAL:
		return D3D12_COMPARISON_FUNC_NOT_EQUAL;
	case eCFGREATEREQUAL:
		return D3D12_COMPARISON_FUNC_GREATER_EQUAL;
	case eCFALWAYS:
		return D3D12_COMPARISON_FUNC_ALWAYS;

	default:
		assert(false);
		return D3D12_COMPARISON_FUNC_NEVER;
	};
}

D3D12_PRIMITIVE_TOPOLOGY D3D12Types::AsD3D12PrimitiveType(PrimitiveTopology::Code t)
{
	switch (t)
	{
	case PrimitiveTopology::PointList:      return D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;
	case PrimitiveTopology::LineList:       return D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
	case PrimitiveTopology::LineStrip:      return D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP;
	case PrimitiveTopology::TriangleList:   return D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	case PrimitiveTopology::TriangleStrip:  return D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
	default:
		printf("D3D12Types::AsDirect3DPrimitiveType(): unsupported topology '%s'!",
			PrimitiveTopology::ToString(t).c_str());
		assert(0);
		return D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
	}
}

}


#endif