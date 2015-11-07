// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#include "IrrCompileConfig.h"
#ifdef _IRR_COMPILE_WITH_BGFX_

#include "CBgfxTexture.h"
#include "os.h"

namespace irr
{
namespace video
{

//! constructor
CBgfxTexture::CBgfxTexture(IImage* image, const io::path& name)
: ITexture(name), IsRenderTarget(false), LockImage(NULL)
{
	#ifdef _DEBUG
	setDebugName("CBgfxTexture");
	#endif

    Format = image->getColorFormat();
	OriginalSize = image->getDimension();
	core::dimension2d<u32> optSize=OriginalSize.getOptimalSize();

    auto bgfxFormat = toBgfx(Format);

    uint32_t flags = BGFX_TEXTURE_NONE;
    Texture = bgfx::createTexture2D(uint16_t(OriginalSize.Width), uint16_t(OriginalSize.Height), 1
        , bgfxFormat
        , flags
        , bgfx::copy(image->lock(), OriginalSize.Width*OriginalSize.Height * image->getBytesPerPixel())
        );
    image->unlock();

    bgfx::calcTextureSize(Info
        , uint16_t(OriginalSize.Width)
        , uint16_t(OriginalSize.Height)
        , 0
        , false
        , 1
        , bgfxFormat
        );
}

CBgfxTexture::CBgfxTexture(const io::path& name)
    : ITexture(name), IsRenderTarget(false), LockImage(NULL)
{

}


//! destructor
CBgfxTexture::~CBgfxTexture()
{
	if (LockImage)
        LockImage->drop();

    //bgfx::destroyTexture(Texture);
}



//! lock function
void* CBgfxTexture::lock(E_TEXTURE_LOCK_MODE mode, u32 mipmapLevel)
{
	return LockImage->lock();
}



//! unlock function
void CBgfxTexture::unlock()
{
	//if (LockImage != Texture)
	//{
	//	os::Printer::log("Performance warning, slow unlock of non power of 2 texture.", ELL_WARNING);
	//	Image->copyToScaling(Texture);
	//}

    LockImage->unlock();
}


//! Returns original size of the texture.
const core::dimension2d<u32>& CBgfxTexture::getOriginalSize() const
{
	return OriginalSize;
}


//! Returns (=size) of the texture.
const core::dimension2d<u32>& CBgfxTexture::getSize() const
{
    return{ Info.width, Info.height };
}


//! returns driver type of texture (=the driver, who created the texture)
E_DRIVER_TYPE CBgfxTexture::getDriverType() const
{
	return EDT_BGFX;
}



//! returns color format of texture
ECOLOR_FORMAT CBgfxTexture::getColorFormat() const
{
	return Format;
}



//! returns pitch of texture (in bytes)
u32 CBgfxTexture::getPitch() const
{
	return Info.width * Info.bitsPerPixel;
}


//! Regenerates the mip map levels of the texture. Useful after locking and
//! modifying the texture
void CBgfxTexture::regenerateMipMapLevels(void* mipmapData)
{
	// our software textures don't have mip maps
}

bool CBgfxTexture::isRenderTarget() const
{
	return IsRenderTarget;
}

bgfx::TextureFormat::Enum CBgfxTexture::toBgfx(ECOLOR_FORMAT format)
{
    static bgfx::TextureFormat::Enum texFormats[] =
    {
        bgfx::TextureFormat::Enum::RGB5A1,//ECF_A1R5G5B5 = 0,
        bgfx::TextureFormat::Enum::R5G6B5,//ECF_R5G6B5,
        bgfx::TextureFormat::Enum::Unknown,//ECF_R8G8B8,
        bgfx::TextureFormat::Enum::RGBA8,//ECF_A8R8G8B8,

        /** Compressed image formats. **/
        bgfx::TextureFormat::Enum::BC1,//ECF_DXT1,
        bgfx::TextureFormat::Enum::BC1,//ECF_DXT2,
        bgfx::TextureFormat::Enum::BC2,//ECF_DXT3,
        bgfx::TextureFormat::Enum::BC2,//ECF_DXT4,
        bgfx::TextureFormat::Enum::BC3,//ECF_DXT5,
        bgfx::TextureFormat::Enum::PTC12,//ECF_PVRTC_RGB2,
        bgfx::TextureFormat::Enum::PTC12A,//ECF_PVRTC_ARGB2,
        bgfx::TextureFormat::Enum::PTC14,//ECF_PVRTC_RGB4,
        bgfx::TextureFormat::Enum::PTC14A,//ECF_PVRTC_ARGB4,
        bgfx::TextureFormat::Enum::PTC22,//ECF_PVRTC2_ARGB2,
        bgfx::TextureFormat::Enum::PTC24,//ECF_PVRTC2_ARGB4,
        bgfx::TextureFormat::Enum::ETC1,//ECF_ETC1,
        bgfx::TextureFormat::Enum::ETC2,//ECF_ETC2_RGB,
        bgfx::TextureFormat::Enum::ETC2A,//ECF_ETC2_ARGB,

        /** Floating Point formats. The following formats may only be used for render target textures. */
        bgfx::TextureFormat::Enum::R16F,//ECF_R16F,
        bgfx::TextureFormat::Enum::RG16F,//ECF_G16R16F,
        bgfx::TextureFormat::Enum::RGBA16F,//ECF_A16B16G16R16F,
        bgfx::TextureFormat::Enum::R32F,//ECF_R32F,
        bgfx::TextureFormat::Enum::RG32F,//ECF_G32R32F,
        bgfx::TextureFormat::Enum::RGBA32F,//ECF_A32B32G32R32F,
        bgfx::TextureFormat::Enum::Unknown,//ECF_UNKNOWN
    };

    return texFormats[format];
}

CBgfxFBOTexture::CBgfxFBOTexture(const core::dimension2du& size,
    const io::path& name, const ECOLOR_FORMAT format)
    :CBgfxTexture(name)
{
    OriginalSize = size;
    IsRenderTarget = true;
    Format = format;

    FrameBuffer = bgfx::createFrameBuffer(OriginalSize.Width, OriginalSize.Height, toBgfx(format));
}

CBgfxFBOTexture::~CBgfxFBOTexture()
{
    bgfx::destroyFrameBuffer(FrameBuffer);
}

} // end namespace video
} // end namespace irr

#endif // _IRR_COMPILE_WITH_SOFTWARE_

