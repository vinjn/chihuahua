// Copyright (C) 2002-2007 Nikolaus Gebhardt
// This file is part of the irrEdit editor
// For conditions of distribution and use, see copyright notice in irredit.h

#ifndef __IRR_PHYSICS_SIMULATION_H_INCLUDED__
#define __IRR_PHYSICS_SIMULATION_H_INCLUDED__

#include "IReferenceCounted.h"
#include "vector3d.h"
#include "IBody.h"
#include "EPhysicsEngineTypes.h"

namespace irr
{
namespace scene
{
	class ITriangleSelector;
}
namespace physics
{

//! Interface wrapping the integrated physics engine
class IPhysicsSimulation : public IReferenceCounted
{
public:

	//! Sets the triangle selector the physics simulation should use as world geometry
	virtual void setWorld(scene::ITriangleSelector* world) = 0;

	//! returns the triangle selector representing the world, previously set by setWorld()
	virtual scene::ITriangleSelector* getWorld() = 0;

	//! updates the geometry of the static world.
	/* To make the physics simulation faster, the simulation can be updated so that
	it only calculates the simulation around a special point (the position of the camera
	as usual) and in a special radius (the view distance as usual) */
	virtual void updateWorld(core::vector3df pos=core::vector3df(0,0,0), f32 radius=1000000.0f) = 0;

	//! Calculates a simulation step of the simulation.
	/** Call this as often as you can. The simulator
	should try to run the simulation at best speed (set by SetSimulationHz()) 
	independent of how often you call it. However, the more often you call, the better 
	the simulator is able to calculate an accurate simulation. */
	virtual void calculateSimulationStep() = 0;

	//! Adds a physics simulated body to the world
	virtual IBody* addBody(E_PHYSICS_BODY_TYPE type, 
						   core::vector3df pos, 
						   core::vector3df rotation = core::vector3df(0,0,0),
						   core::vector3df size = core::vector3df(10,10,10),
						   f32 density = 1.0f) = 0;

	//! Returns body count available in the simulation
	virtual s32 getBodyCount() = 0;

	//! Removes a body permanently from the simulation
	virtual void removeBody(IBody* toRemove) = 0;

	//! returns a body by index. Index must by smaller than getBodyCount()
	virtual IBody* getBody(s32 index) = 0;

	//! gets the frequency (Hz) the simulation is running with. Default is 60.
	virtual s32 getSimulationHz() = 0;

	//! sets the frequency (Hz) the simulation is running with. Default is 60.
	virtual void setSimulationHz(s32 hz) = 0;

	//! returns type of engine running this simulation
	virtual EPHYSICS_ENGINE_TYPES getType() = 0;

	//! Returns true if the suppliced ray hits the world
	virtual bool collidesRayWithWorld(const core::vector3df& rayStart, 
									  const core::vector3df& rayEnd, 
									  core::vector3df& outhitPos) = 0;
};
	
} // end namespace
} // end namespace

#endif