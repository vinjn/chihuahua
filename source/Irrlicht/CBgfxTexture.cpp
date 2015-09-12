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
CBgfxTexture::CBgfxTexture(IImage* image, const io::path& name,
		bool renderTarget, void* mipmapData)
: ITexture(name), IsRenderTarget(renderTarget), LockImage(NULL)
{
	#ifdef _DEBUG
	setDebugName("CBgfxTexture");
	#endif

	OrigSize = image->getDimension();
	core::dimension2d<u32> optSize=OrigSize.getOptimalSize();

    uint32_t _flags = BGFX_TEXTURE_NONE;
    Texture = bgfx::createTexture2D(uint16_t(OrigSize.Width), uint16_t(OrigSize.Height), 1
        , bgfx::TextureFormat::RGBA8
        , _flags
        , bgfx::copy(image->lock(), OrigSize.Width*OrigSize.Height * 4)
        );
    image->unlock();

    bgfx::calcTextureSize(Info
        , uint16_t(OrigSize.Width)
        , uint16_t(OrigSize.Height)
        , 0
        , false
        , 1
        , bgfx::TextureFormat::RGBA8
        );
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
	return OrigSize;
}


//! Returns (=size) of the texture.
const core::dimension2d<u32>& CBgfxTexture::getSize() const
{
	return core::dimension2d<u32>(Info.width, Info.height);
}


//! returns driver type of texture (=the driver, who created the texture)
E_DRIVER_TYPE CBgfxTexture::getDriverType() const
{
	return EDT_BGFX;
}



//! returns color format of texture
ECOLOR_FORMAT CBgfxTexture::getColorFormat() const
{
	return ECF_A8R8G8B8;
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


} // end namespace video
} // end namespace irr

#endif // _IRR_COMPILE_WITH_SOFTWARE_

