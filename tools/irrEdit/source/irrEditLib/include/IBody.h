// Copyright (C) 2002-2007 Nikolaus Gebhardt
// This file is part of the irrEdit editor
// For conditions of distribution and use, see copyright notice in irredit.h

#ifndef __IRR_BODY_H_INCLUDED__
#define __IRR_BODY_H_INCLUDED__

#include "IReferenceCounted.h"
#include "matrix4.h"

namespace irr
{
namespace physics
{

//! enumeration for simulated physics object types
enum E_PHYSICS_BODY_TYPE
{
	EPBT_SPHERE = 0,
	EPBT_BOX,
	EPBT_CYLINDER,
	EPBT_CAPSULE,
	EPBT_CHARACTER_AVATAR,

	ERDRBT_COUNT
};

//! A body simulated by the physics engine
class IBody : public IReferenceCounted
{
public:

	//! returns the current simulated position of the body
	virtual core::vector3df getPosition() = 0;

	//! returns the current simulated rotation of the body
	virtual core::vector3df getRotation() = 0;

	//! returns the type of the body
    virtual E_PHYSICS_BODY_TYPE getType() = 0;

	//! adds a force to the body
	virtual void addForce( core::vector3df posFrom, core::vector3df posTo, f32 force) = 0;
};
	
} // end namespace
} // end namespace

#endif