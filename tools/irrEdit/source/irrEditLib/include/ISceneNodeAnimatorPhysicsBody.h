// Copyright (C) 2002-2007 Nikolaus Gebhardt
// This file is part of the irrEdit editor
// For conditions of distribution and use, see copyright notice in irredit.h

#ifndef __I_SCENE_NODE_ANIMATOR_PHYSICS_BODY_INCLUDED__
#define __I_SCENE_NODE_ANIMATOR_PHYSICS_BODY_INCLUDED__

#include "ISceneNodeAnimator.h"

namespace irr
{
namespace scene
{

	//! Animator making the attached object behave physically correct using a physics engine.
	class ISceneNodeAnimatorPhysicsBody : public ISceneNodeAnimator
	{
	public:

		//! destructor
		virtual ~ISceneNodeAnimatorPhysicsBody() {}

		// TODO: currently only possible to influence parameters via serialization
	};

} // end namespace scene
} // end namespace irr

#endif

