
#include "stdneb.h"
#include "Texture.h"

namespace RenderBase
{

	//------------------------------------------------------------------------------
	/**
	*/
	Texture::Texture() :
	type(InvalidType),
		width(0),
		height(0),
		depth(0),
		numMipLevels(0),
		skippedMips(0),
		memorySize(0),
		m_unitIndex(0),
		m_bSRGBRead(false),
		pixelFormat(PixelFormat::InvalidPixelFormat)
	{
		// empty
	}

	//------------------------------------------------------------------------------
	/**
	*/
	Texture::~Texture()
	{
		// empty
	}

	//------------------------------------------------------------------------------
	/**
	*/
	bool
		Texture::Map(IndexT mipLevel, Access accessMode, MapInfo& outMapInfo)
	{
		printf("Base Texture::Map() called!");
		assert(0);
		return false;
	}

	//------------------------------------------------------------------------------
	/**
	*/
	void
		Texture::Unmap(IndexT mipLevel)
	{
		printf("Base Texture::Unmap() called!");
		assert(0);
	}

	//------------------------------------------------------------------------------
	/**
	*/
	bool
		Texture::MapCubeFace(CubeFace face, IndexT mipLevel, Access accessMode, MapInfo& outMapInfo)
	{
		printf("Base Texture::MapCubeFace() called!");
		assert(0);
		return false;
	}

	//------------------------------------------------------------------------------
	/**
	*/
	void
		Texture::UnmapCubeFace(CubeFace cubeFace, IndexT mipLevel)
	{
		printf("Base Texture::UnmapCubeFace() called!");
		assert(0);
	}

	//------------------------------------------------------------------------------
	/**
	*/
	void Texture::Setup()
	{
		SetRenderCommandType(RenderCommandType::SetTexture);
	}

	//------------------------------------------------------------------------------
	/**
	*/
	void Texture::OnDeviceLost()
	{

	}

	//------------------------------------------------------------------------------
	/**
	*/
	void Texture::OnDeviceReset()
	{

	}

} // namespace RenderBase
