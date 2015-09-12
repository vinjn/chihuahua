// Copyright (C) 2002-2007 Nikolaus Gebhardt
// This file is part of the irrEdit editor
// For conditions of distribution and use, see copyright notice in irredit.h

#ifndef __I_IRREDIT_SERVICES_INCLUDED__
#define __I_IRREDIT_SERVICES_INCLUDED__

#include <irrlicht.h>
#include "IScriptEngine.h"
#include "IPhysicsSimulation.h"
#include "ISceneNodeAnimatorPhysicsBody.h"
#include "IBody.h"

namespace irrklang
{
	class ISoundEngine; // irrKlang sound engine forward declaration for people not having irrKlang headers
}
namespace irr
{
namespace irredit
{

	//! Interface providing useful services to make games and irrEdit plugins with irrlicht
	class IrrEditServices : public IReferenceCounted
	{
	public:

		//! destructor
		virtual ~IrrEditServices() {}

		//! returns the device used in this instance of irrEdit services
		virtual IrrlichtDevice* getDevice() = 0;

		//! returns the sound engine used in this instance of irrEdit services
		virtual irrklang::ISoundEngine* getSoundEngine() = 0;

		//! Creates an instance of an integrated physics engine.
		/** Usually, you would want to set the created simulation in the scene manager
		using ISceneManager::setPhysics() afterwards. 
		\return Returns pointer to the created physics engine.
	    If you no longer need the animator, you should call IPhysicsSimulation::drop().
		See IUnknown::drop() for more information. */
		virtual physics::IPhysicsSimulation* createPhysics( 
			physics::EPHYSICS_ENGINE_TYPES type = physics::EPET_ODE ) = 0;

		//! Creates an instance of the scripting engine.
		/** \param withStdLibs: Makes libraries available in the script environment. These are I/O, Blob, Math,
		System and String libraries.
		\param withIrrlichtLibs: Makes irrlicht libraries available in the scripting environment. Provides functions
		to access the irrlicht device, scenemanager, video driver etc 
		\return Returns pointer to the created script engine.
	    If you no longer need the engine, you should call IScriptEngine::drop().
		See IUnknown::drop() for more information. */
		virtual script::IScriptEngine* createScriptEngine(bool withStdLibs=true, bool withIrrlichtLibs=true) = 0;

			//! Creates a physics body animator
		/** This Animator makes the attached object behave physically correct using the  
		physics engine set for this scene manager (using setPhysics() )
		\param sceneNode: SceneNode which should be manipulated.
		\return Returns the animator. Attach it to a scene node with ISceneNode::addAnimator()
		 and the animator will cause it to do physics based movement.
		 If you no longer need the animator, you should call ISceneNodeAnimator::drop(). 
		 See IUnknown::drop() for more information. */
		virtual scene::ISceneNodeAnimatorPhysicsBody* createPhysicsBodyAnimator(scene::ISceneNode* node,
			physics::E_PHYSICS_BODY_TYPE type=physics::EPBT_BOX,
			f32 density=1.0f, core::vector3df overrideSize = core::vector3df(0,0,0)) = 0;

		//! Creates a physics avatar animator for controlling characters and cameras
		/** This Animator makes the attached object behave physically correct using the  
		physics engine set for this scene manager (using setPhysics() ) like the animator
		createable via createPhysicsBodyAnimator but lets the avatar look up always, ignores
		rotation of the object and treats position changes of the object as force inputs
		instead of preventing position changes.
		\param sceneNode: SceneNode which should be manipulated.
		\return Returns the animator. Attach it to a scene node with ISceneNode::addAnimator()
		 and the animator will cause it to do physics based movement.
		 If you no longer need the animator, you should call ISceneNodeAnimator::drop(). 
		 See IUnknown::drop() for more information. */
		virtual scene::ISceneNodeAnimatorPhysicsBody* createPhysicsAvatarAnimator(scene::ISceneNode* node,
			f32 eyeHeight=8.0f, f32 density=1.0f, core::vector3df avatarSize = core::vector3df(0,0,0)) = 0;

		//! Reads a string value from the windows registry, returns true if sucessful.
		/** Path is a value like "Software\\CompanyName\\ApplicationName" */
		virtual bool readRegistryValue(const c8* path, const c8* valueName, c8* outstring, s32 sizeofString) = 0;

		//! Reads an int value from the windows registry, returns true if sucessful.
		/** Path is a value like "Software\\CompanyName\\ApplicationName" */
		virtual bool readRegistryValue(const c8* path, const c8* valueName, s32& outint) = 0;

		//! Writes a string value to the registry, returns true if sucessful.
		/** Path is a value like "Software\\CompanyName\\ApplicationName" */
		virtual bool writeRegistryValue(const c8* path, const c8* valueName, const c8* stringToWrite) = 0;

		//! Writes a int value to the registry, returns true if sucessful.
		/** Path is a value like "Software\\CompanyName\\ApplicationName" */
		virtual bool writeRegistryValue(const c8* path, const c8* valueName, s32 intToWrite) = 0;
	};


	#ifdef _IRR_WINDOWS_
		#ifdef IRREDITLIB_EXPORTS
			#define IRREDITLIB_API __declspec(dllexport)
			#else
			#define IRREDITLIB_API __declspec(dllimport)
		#endif // IRRLICHT_EXPORT

		// Declare the calling convention.
		#if defined(_STDCALL_SUPPORTED)
			#define IRREDITLIBCALLCONV __stdcall
			#else
			#define IRREDITLIBCALLCONV __cdecl
		#endif // STDCALL_SUPPORTED
	#else
		#define IRREDITLIB_API 
		#define IRREDITLIBCALLCONV
	#endif // _IRR_WINDOWS_


	IRREDITLIB_API IrrEditServices* IRREDITLIBCALLCONV createIrrEditServices(IrrlichtDevice* device, irrklang::ISoundEngine* soundEngine);

} // end namespace irredit
} // end namespace irr




#endif

