// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#ifndef __C_BGFX_TEXTURE_H_INCLUDED__
#define __C_BGFX_TEXTURE_H_INCLUDED__

#include "ITexture.h"
#include "CImage.h"
#include "bgfx.h"

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
	CBgfxTexture(IImage* surface, const io::path& name,
			bool renderTarget=false, void* mipmapData=0);

	//! destructor
	virtual ~CBgfxTexture();

	//! lock function
	virtual void* lock(E_TEXTURE_LOCK_MODE mode=ETLM_READ_WRITE, u32 mipmapLevel=0) _IRR_OVERRIDE_;

	//! unlock function
	virtual void unlock() _IRR_OVERRIDE_;

	//! Returns original size of the texture.
	virtual const core::dimension2d<u32>& getOriginalSize() const _IRR_OVERRIDE_;

	//! Returns (=size) of the texture.
	virtual const core::dimension2d<u32>& getSize() const _IRR_OVERRIDE_;

	//! returns driver type of texture (=the driver, who created the texture)
	virtual E_DRIVER_TYPE getDriverType() const _IRR_OVERRIDE_;

	//! returns color format of texture
	virtual ECOLOR_FORMAT getColorFormat() const _IRR_OVERRIDE_;

	//! returns pitch of texture (in bytes)
	virtual u32 getPitch() const _IRR_OVERRIDE_;

	//! Regenerates the mip map levels of the texture. Useful after locking and
	//! modifying the texture
	virtual void regenerateMipMapLevels(void* mipmapData=0) _IRR_OVERRIDE_;

	//! is it a render target?
	virtual bool isRenderTarget() const _IRR_OVERRIDE_;

private:
	core::dimension2d<u32> OrigSize;
	bool IsRenderTarget;

    IImage* LockImage;
    bgfx::TextureHandle Texture;
    bgfx::TextureInfo   Info;
};


} // end namespace video
} // end namespace irr

#endif

