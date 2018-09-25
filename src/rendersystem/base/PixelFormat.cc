

#include "stdneb.h"
#include <assert.h>
#include <algorithm>
#ifdef __OSX__
#include "../rendersystem/base/PixelFormat.h"
#include "../rendersystem/base/PixelFormatDesc.h"
#include "math/scalar.h"
#else
#include "rendersystem/base/PixelFormat.h"
#include "rendersystem/base/PixelFormatDesc.h"
#endif
namespace RenderBase
{

	const PixelFormatDescription& getDescriptionFor(PixelFormat::Code format)
	{
		const int ord = (int)format;
		assert( ord >= 0 && ord < PixelFormat::DescPixelFormatsCount );
		return _pixelFormats[ord];
	}
	//------------------------------------------------------------------------------
	/**
	Convert a pixel format string into a pixel format code.
	*/
	PixelFormat::Code
	PixelFormat::FromString(const std::string& str)
	{
		for ( int i = 0; i < PixelFormat::DescPixelFormatsCount; ++i )
		{
			const PixelFormatDescription& desc = getDescriptionFor( (PixelFormat::Code)i );
			if ( str == desc.name )
			{
				return (PixelFormat::Code)i;
			}
		}

		return PixelFormat::InvalidPixelFormat;
	}
	//------------------------------------------------------------------------

    unsigned int PixelFormat::GetFlags( Code format )
	{
		return getDescriptionFor(format).flags; 
	}

	//------------------------------------------------------------------------
	bool 
	PixelFormat::IsCompressed(Code code)
	{
		return (GetFlags(code) & PFF_COMPRESSED) > 0;
	}
	//------------------------------------------------------------------------
	SizeT 
	PixelFormat::GetNumElemBytes(Code code)
	{
		 return getDescriptionFor(code).elemBytes;
	}
	//------------------------------------------------------------------------
	void 
	PixelFormat::GetBitMasks(Code format, uint32 rgba[4])
	{
		const PixelFormatDescription &des = getDescriptionFor(format);
		rgba[0] = des.rmask;
		rgba[1] = des.gmask;
		rgba[2] = des.bmask;
		rgba[3] = des.amask;
	}
	//------------------------------------------------------------------------
	void 
	PixelFormat::GetBitDepths(Code format, int rgba[4])
	{
		const PixelFormatDescription &des = getDescriptionFor(format);
		rgba[0] = des.rbits;
		rgba[1] = des.gbits;
		rgba[2] = des.bbits;
		rgba[3] = des.abits;
	}

	//------------------------------------------------------------------------
	SizeT 
	PixelFormat::GetMemorySize(SizeT width, SizeT height, SizeT depth, Code format)
	{
		if(IsCompressed(format))
		{
			switch(format)
			{
				//  un compressed dxt
			case A8R8G8B8:
				return width*height*4;
			case R8G8B8:
				return width*height*3;
				// DXT formats work by dividing the image into 4x4 blocks, then encoding each
				// 4x4 block with a certain number of bytes. 
			case DXT1:
				return ((width+3)/4)*((height+3)/4)*8 * depth;
			case DXT2:
			case DXT3:
			case DXT4:
			case DXT5:
				return ((width+3)/4)*((height+3)/4)*16 * depth;

				// Size calculations from the PVRTC OpenGL extension spec
				// http://www.khronos.org/registry/gles/extensions/IMG/IMG_texture_compression_pvrtc.txt
				// Basically, 32 bytes is the minimum texture size.  Smaller textures are padded up to 32 bytes
			case PVRTC_RGB2:
			case PVRTC_RGBA2:
				assert(depth == 1);
				return ( std::max<size_t>( width, 16 ) * std::max<size_t>( height, 8 ) * 2 + 7 ) / 8;
			case PVRTC_RGB4:
			case PVRTC_RGBA4:
				assert(depth == 1);
				return (std::max((int)width, 8) * std::max((int)height, 8) * 4 + 7) / 8;
			case ETC1_RGB8:
				assert(depth == 1);
				return ( 8 * ((width + 3) >> 2) * ((height + 3) >> 2) );
			case ETC2_RGB8:
				assert(depth == 1);
				return (std::max<size_t>(width, 4) * std::max<size_t>(height, 4) * 4 + 7) / 8;
			case ETC2_RGBA8:
			assert(depth == 1);
				return (std::max<size_t>(width, 4) * std::max<size_t>(height, 4) * 8 + 7) / 8;
			default:
				printf("PixelFormat::GetMemorySize: Invalid compressed pixel format" );
				assert(0);
				return 0;
			}
		}
		else
		{
			return width*height*depth*GetNumElemBytes(format);
		}
	}	
	//------------------------------------------------------------------------------
	/**
	Convert pixel format code into a string.
	*/
	std::string
	PixelFormat::ToString(PixelFormat::Code code)
	{
		return getDescriptionFor(code).name;
	}
}
