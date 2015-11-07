// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#ifndef __C_BGFX_TEXTURE_H_INCLUDED__
#define __C_BGFX_TEXTURE_H_INCLUDED__

#include "ITexture.h"
#include "CImage.h"
#include "bgfx/bgfx.h"

namespace irr
{
namespace video
{

/*!
	interface for a Video Driver dependent Texture.
*/
class CBgfxTexture : public ITexture
{
public:

	//! constructor
	CBgfxTexture(IImage* surface, const io::path& name);

	//! destructor
	~CBgfxTexture();

	//! lock function
	void* lock(E_TEXTURE_LOCK_MODE mode=ETLM_READ_WRITE, u32 mipmapLevel=0) _IRR_OVERRIDE_;

	//! unlock function
	void unlock() _IRR_OVERRIDE_;

	//! Returns original size of the texture.
	const core::dimension2d<u32>& getOriginalSize() const _IRR_OVERRIDE_;

	//! Returns (=size) of the texture.
	const core::dimension2d<u32>& getSize() const _IRR_OVERRIDE_;

	//! returns driver type of texture (=the driver, who created the texture)
	E_DRIVER_TYPE getDriverType() const _IRR_OVERRIDE_;

	//! returns color format of texture
	ECOLOR_FORMAT getColorFormat() const _IRR_OVERRIDE_;

	//! returns pitch of texture (in bytes)
	u32 getPitch() const _IRR_OVERRIDE_;

	//! Regenerates the mip map levels of the texture. Useful after locking and
	//! modifying the texture
	void regenerateMipMapLevels(void* mipmapData=0) _IRR_OVERRIDE_;

	//! is it a render target?
	bool isRenderTarget() const _IRR_OVERRIDE_;

    static bgfx::TextureFormat::Enum toBgfx(ECOLOR_FORMAT format);

    bgfx::TextureHandle getTexture() const {return Texture;}

protected:
    CBgfxTexture(const io::path& name);

	core::dimension2d<u32> OriginalSize;
	bool IsRenderTarget;

    IImage* LockImage;
    bgfx::TextureHandle Texture;
    bgfx::TextureInfo   Info;
    ECOLOR_FORMAT Format;
};

class CBgfxFBOTexture : public CBgfxTexture
{
public:
    CBgfxFBOTexture(const core::dimension2du& size,
        const io::path& name, const ECOLOR_FORMAT format = ECF_UNKNOWN);
    ~CBgfxFBOTexture();
    bgfx::FrameBufferHandle getFrameBuffer() const {return FrameBuffer;}

private:
    bgfx::FrameBufferHandle FrameBuffer;
    //bgfx::TextureHandle ColorTex;
    //bgfx::TextureHandle DepthTex;

};

} // end namespace video
} // end namespace irr

#endif

