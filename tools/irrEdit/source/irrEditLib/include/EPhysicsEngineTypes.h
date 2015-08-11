// Copyright (C) 2002-2007 Nikolaus Gebhardt
// This file is part of the irrEdit editor
// For conditions of distribution and use, see copyright notice in irredit.h

#ifndef __E_PHYSICS_ENGINE_TYPES_H_INCLUDED__
#define __E_PHYSICS_ENGINE_TYPES_H_INCLUDED__

namespace irr
{
namespace physics  
{
	//! An enumeration for all types of supported physics engines
	enum EPHYSICS_ENGINE_TYPES
	{
		//! Open dynamics physics engine
		EPET_ODE = 0,

		//! Amount of built-in scene node animators
		EPET_COUNT,

		//! This enum is never used, it only forces the compiler to 
		//! compile these enumeration values to 32 bit.
		EPET_FORCE_32_BIT = 0x7fffffff
	};

} // end namespace scene
} // end namespace irr


#endif

