/****************************************************************************
Copyright (c) 2007, Radon Labs GmbH
Copyright (c) 2011-2013,WebJet Business Division,CYOU
 
http://

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/
#ifndef VERTEXCOMPONENT_H_
#define VERTEXCOMPONENT_H_
#include "core/types.h"
#include <assert.h>
#include <string>
#include <vector>
//------------------------------------------------------------------------------
namespace RenderBase
{

class VertexComponent
{
public:
    /// component semantic
    enum SemanticName
    {
        Position = 0,
        Normal,
        Tangent,
        Binormal,
        TexCoord,        
        Color,
        SkinWeights,
        SkinJIndices,
		Size,

        Invalid,
    };



    /// component format
    enum Format
    {
        Float,      //> one-component float, expanded to (float, 0, 0, 1)
        Float2,     //> two-component float, expanded to (float, float, 0, 1)
        Float3,     //> three-component float, expanded to (float, float, float, 1)
        Float4,     //> four-component float
        UByte4,     //> four-component unsigned byte
        Short2,     //> two-component signed short, expanded to (value, value, 0, 1)
        Short4,     //> four-component signed short
        UByte4N,    //> four-component normalized unsigned byte (value / 255.0f)
        Short2N,    //> two-component normalized signed short (value / 32767.0f)
        Short4N,    //> four-component normalized signed short (value / 32767.0f)
		ColorBGRA,	//> color type. for D3D9 fix function support
		ColorRGBA,	//> color type. for opengl fix function support

        // PS3-specific
        Float16,
        Float16_2,
        Float16_3,
        Float16_4,
    };
    
    /// access type hint, this is only relevant on the Wii
    enum AccessType
    {
        None,
        Direct,     //> component has direct value (non-indexed)
        Index8,     //> component is indexed with 8-bit indices           
        Index16,    //> component is indexed with 16-bit indices
        Index32,    //> component is indexed with 32-bit indices
    };
	struct Semantic
	{
		SemanticName name;
		ushort index;
		inline Semantic()
			:name(RenderBase::VertexComponent::Invalid)
			,index(0)
		{
		}
		inline Semantic(SemanticName _name, ushort _index)
			:name(_name)
			,index(_index)
		{
		}

		inline const Semantic& operator = (const Semantic& rhs)
		{
			this->name = rhs.name;
			this->index = rhs.index;
			return *this;
		}

		inline bool operator == (const Semantic& rhs) const
		{
			return (this->name == rhs.name) && (this->index == rhs.index);
		}

		inline bool operator != (const Semantic& rhs) const
		{
			return (this->name != rhs.name) || (this->index != rhs.index);
		}

		inline bool operator > (const Semantic& rhs) const
		{
			return ((int(this->name) << 16) + int(this->index)) > ((int(rhs.name) << 16) + int(rhs.index));
		}

		inline bool operator < (const Semantic& rhs) const
		{
			return ((int(this->name) << 16) + int(this->index)) < ((int(rhs.name) << 16) + int(rhs.index));
		}
	};

    /// default constructor
    VertexComponent();
    /// constructor
    VertexComponent(SemanticName semName, IndexT semIndex, Format format, IndexT streamIndex=0);
    /// get semantic name
    SemanticName GetSemanticName() const;

	const Semantic& GetSemantic() const;
    /// get semantic index
    IndexT GetSemanticIndex() const;
    /// get vertex component format
    Format GetFormat() const;
    /// get stream index
    IndexT GetStreamIndex() const;
    /// get the byte size of the vertex component
    SizeT GetByteSize() const;
    /// get a unique signature of the vertex component
    std::string GetSignature() const;
    /// get access type
    AccessType GetAccessType() const;
    /// convert string to semantic name
    static SemanticName StringToSemanticName(const std::string& str);
    /// convert semantic name to string
    static std::string SemanticNameToString(SemanticName n);
    /// convert string to format
    static Format StringToFormat(const std::string& str);
    /// convert format to string
    static std::string FormatToString(Format f);
    /// get the byte offset of this component (only valid when part of a VertexLayout)
    IndexT GetByteOffset() const;
	/// Utility method which returns the count of values in a given type
	static uint GetTypeCount(Format format);
	static size_t GetByteSize(Format format);
	static size_t BuildComponentsOffsetAndSize(std::vector<VertexComponent>& in_out);

protected:
    friend class VertexLayout;

    /// set the vertex byte offset (called from VertexLayoutBase::Setup())
    void SetByteOffset(IndexT offset);

    Semantic semantic;
    Format format;
    AccessType accessType;
    IndexT streamIndex;
    IndexT byteOffset;
};

//------------------------------------------------------------------------------
/**
*/
inline
VertexComponent::VertexComponent() :
    format(Float),
    accessType(Index16),
    streamIndex(0),
    byteOffset(0)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
inline
VertexComponent::VertexComponent(SemanticName semName_, IndexT semIndex_, Format format_, IndexT streamIndex_) :
	semantic(semName_, (ushort)semIndex_),
    format(format_),
    accessType(Index16),
    streamIndex(streamIndex_),
    byteOffset(-1)
{
}

//------------------------------------------------------------------------------
/**
*/
inline VertexComponent::SemanticName
VertexComponent::GetSemanticName() const
{
    return this->semantic.name;
}
//------------------------------------------------------------------------------
/**
*/
inline const VertexComponent::Semantic& 
VertexComponent::GetSemantic() const
{
	return semantic;
}
//------------------------------------------------------------------------------
/**
*/
inline VertexComponent::AccessType
VertexComponent::GetAccessType() const
{
    return this->accessType;     
}

//------------------------------------------------------------------------------
/**
*/
inline IndexT
VertexComponent::GetSemanticIndex() const
{
    return this->semantic.index;
}

//------------------------------------------------------------------------------
/**
*/
inline VertexComponent::Format
VertexComponent::GetFormat() const
{
    return this->format;
}

//------------------------------------------------------------------------------
/**
*/
inline IndexT
VertexComponent::GetStreamIndex() const
{
    return this->streamIndex;
}
//------------------------------------------------------------------------------
/**
*/
inline size_t
	VertexComponent::GetByteSize(Format format)
{
	switch (format)
	{
	case Float:     return 4;
	case Float2:    return 8;
	case Float3:    return 12;
	case Float4:    return 16;
	case UByte4:    return 4;
	case Short2:    return 4;
	case Short4:    return 8;
	case UByte4N:   return 4;
	case Short2N:   return 4;
	case Short4N:   return 8;
	case ColorBGRA: return 4;
	case ColorRGBA: return 4;

		// PS3-specific
	case Float16:   return 2;
	case Float16_2: return 4;
	case Float16_3: return 6;
	case Float16_4: return 8;
	}
	printf("Can't happen");
	assert(0);
	return 0;
}

//------------------------------------------------------------------------------
/**
*/
inline SizeT
VertexComponent::GetByteSize() const
{
   return GetByteSize(this->format);
}

//------------------------------------------------------------------------------
/**
*/
inline std::string
VertexComponent::SemanticNameToString(SemanticName n)
{
    switch (n)
    {
        case Position:      return "Position";
        case Normal:        return "Normal";
        case Tangent:       return "Tangent";
        case Binormal:      return "Binormal";
        case TexCoord:      return "TexCoord";
        case Color:         return "Color";
        case SkinWeights:   return "SkinWeights";
        case SkinJIndices:  return "SkinJIndices";
		case Size:			return "Size";
        default:
			printf("VertexComponent::SemanticNameToString(): invalid SemanticName code!");
			assert(0);
            return "";
    }
}

//------------------------------------------------------------------------------
/**
*/
inline VertexComponent::SemanticName
VertexComponent::StringToSemanticName(const std::string& str)
{
    if (str == "Position") return Position;
    else if (str == "Normal") return Normal;
    else if (str == "Tangent") return Tangent;
    else if (str == "Binormal") return Binormal;
    else if (str == "TexCoord") return TexCoord;
    else if (str == "Color") return Color;
    else if (str == "SkinWeights") return SkinWeights;
    else if (str == "SkinJIndices") return SkinJIndices;
	else if (str == "Size")	return Size;
    else
    {
        printf("VertexComponent::StringToSemanticName(): invalid string '%s'!", str.c_str());
		assert(0);
        return Invalid;
    }
}

//------------------------------------------------------------------------------
/**
*/
inline std::string
VertexComponent::FormatToString(Format f)
{
    switch (f)
    {
        case Float:     return "Float";
        case Float2:    return "Float2";
        case Float3:    return "Float3";
        case Float4:    return "Float4";
        case UByte4:    return "UByte4";
        case Short2:    return "Short2";
        case Short4:    return "Short4";
        case UByte4N:   return "UByte4N";
        case Short2N:   return "Short2N";
        case Short4N:   return "Short4N";
		case ColorBGRA: return "ColorBGRA";
		case ColorRGBA: return "ColorRGBA";

        // PS3-specific
        case Float16:   return "Float16";
        case Float16_2: return "Float16_2";
        case Float16_3: return "Float16_3";
        case Float16_4: return "Float16_4";

        default:
            printf("VertexComponent::FormatToString(): invalid Format code!");
			assert(0);
            return "";
    }
}

//------------------------------------------------------------------------------
/**
*/
inline VertexComponent::Format
VertexComponent::StringToFormat(const std::string& str)
{
    if (str == "Float") return Float;
    else if (str == "Float2") return Float2;
    else if (str == "Float3") return Float3;
    else if (str == "Float4") return Float4;
    else if (str == "UByte4") return UByte4;
    else if (str == "Short2") return Short2;
    else if (str == "Short4") return Short4;
    else if (str == "UByte4N") return UByte4N;
    else if (str == "Short2N") return Short2N;
    else if (str == "Short4N") return Short4N;
	else if (str == "ColorBGRA") return ColorBGRA;
	else if (str == "ColorRGBA") return ColorRGBA;
    else if (str == "Float16") return Float16;
    else if (str == "Float16_2") return Float16_2;
    else if (str == "Float16_3") return Float16_3;
    else if (str == "Float16_4") return Float16_4;
    else
    {
        printf("VertexComponent::StringToFormat(): invalid string '%s'!\n", str.c_str());
		assert(0);
        return Float;
    }
}

//------------------------------------------------------------------------------
/**
*/
inline std::string
VertexComponent::GetSignature() const
{
    std::string str;
    switch (this->semantic.name)
    {
        case Position:      str = "pos"; break;
        case Normal:        str = "nrm"; break;
        case Tangent:       str = "tan"; break;
        case Binormal:      str = "bin"; break;
        case TexCoord:      str = "tex"; break;
        case Color:         str = "clr"; break;
        case SkinWeights:   str = "skw"; break;
        case SkinJIndices:  str = "sji"; break;
		case Size:			str = "sze"; break;
        default:
            printf("can't happen!");
			assert(0);
            break;
    }

	std::string string_temp = std::to_string(GetSemanticIndex());

    str.append(string_temp);
    str.append(FormatToString(this->format));
    return str;
}

//------------------------------------------------------------------------------
/**
*/
inline void
VertexComponent::SetByteOffset(IndexT offset)
{
    this->byteOffset = offset;
}

//------------------------------------------------------------------------------
/**
*/
inline IndexT
VertexComponent::GetByteOffset() const
{
    return this->byteOffset;
}

//------------------------------------------------------------------------------
/**
*/
inline uint
	VertexComponent::GetTypeCount(Format format)
{
	switch (format)
	{
	case Float:	
		return 1;
	case Float2:
		return 2;
	case Float3:
		return 3;
	case Float4:
		return 4;
	case UByte4:
		return 4;
	case Short2:
		return 2;
	case Short4:
		return 4;
	case UByte4N:
		return 4;
	case Short2N:
		return 2;
	case Short4N:
		return 4;
	case ColorBGRA:
	case ColorRGBA:
		return 4;
	}

	printf("Invalid type!");
	assert(0);
	return 0;
}

} // namespace RenderBase
//------------------------------------------------------------------------------

#endif