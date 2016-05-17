// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#ifndef __E_DRIVER_TYPES_H_INCLUDED__
#define __E_DRIVER_TYPES_H_INCLUDED__

#include "irrTypes.h"

namespace ue
{
namespace video
{

	//! An enum for all types of drivers the Irrlicht Engine supports.
	enum E_DRIVER_TYPE
	{
		//! Null driver, useful for applications to run the engine without visualisation.
		/** The null device is able to load textures, but does not
		render and display any graphics. */
		EDT_NULL,

        //! bgfx based driver
        EDT_BGFX_OPENGL,
        EDT_BGFX_OPENGL_ES,
        EDT_BGFX_D3D9,
        EDT_BGFX_D3D11,
        EDT_BGFX_D3D12,
        EDT_BGFX_METAL,
        EDT_BGFX_VULKAN,

		//! No driver, just for counting the elements
		EDT_COUNT
	};
	
	const c8* const DRIVER_TYPE_NAMES[] = 
	{
		"NullDriver",
        "OpenGL",
        "OpenGL ES",
        "D3D 9",
        "D3D 11",
        "D3D 12",
        "Metal",
        "Vulkan",
		0 
	};
	

} // end namespace video
} // end namespace ue


#endif
