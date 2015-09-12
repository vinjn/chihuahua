// Copyright (C) 2002-2007 Nikolaus Gebhardt
// This file is part of the irrEdit editor
// For conditions of distribution and use, see copyright notice in irredit.h

#ifndef __IRR_IRREDIT_PLUGIN_H_INCLUDED__
#define __IRR_IRREDIT_PLUGIN_H_INCLUDED__

#include "IReferenceCounted.h"
#include "rect.h"
#include "vector3d.h"

namespace irr
{
	class IrrlichtDevice;

namespace scene
{
	class ISceneManager;
}
namespace irredit
{
	class IrrEditServices;

//! structure holding start up parameters and options 
struct SIrrEditPluginStartupOptions
{
	SIrrEditPluginStartupOptions()
		: StartedByEditor(false) {}

	// DesiredStartPosition is the same as DesiredStartLookAtPosition if not used
	core::vector3df DesiredStartPosition;

	// DesiredStartPosition is the same as DesiredStartLookAtPosition if not used
	core::vector3df DesiredStartLookAtPosition;

	//! Will be true when the plugin has been started by the editor
	bool StartedByEditor;
};

//! Interface to let the engine access the plugin.
/** To make it possible for the editor for example to run a test game directly inside the editor
you need to implement this interface. Create a .dll which implements the function 
\code
__declspec(dllexport) irr::irredit::IIrrEditPlugin* __stdcall 
	createPlugin( irr::irredit::IrrEditServices* services)
\endcode 
and returns a pointer to a newly created plugin. A simple example
can be found in the project exampleGamePlugin. This function will be called once to create and
store an IIrrEditPlugin object, representing you game. When starting a game for example, start() and then
run() will be called in a loop, until it returns a non null value. stop() will be called
to abort the running game.
*/
class IIrrEditPlugin : public IReferenceCounted
{
public:

	//! will be called to init the plugin status. 
	/** Afterwards run() will be called in a loop until it returns a value != 0.
	 \param sceneManger: The scene manger to be used for this plugin instance.  
	 \param param options: Structure holding startup parameters. */
	virtual void start(scene::ISceneManager* sceneManager, SIrrEditPluginStartupOptions& options) = 0;

	//! will be called to stop the currently running simulation. 
	/** Implement this to clean up used memory etc. */
	virtual void stop() = 0;

	//! runs the plugin in an editor window. run() will be called in a loop until it returns a value != 0.
	/** \param windowId: Id to be used for the IVideoDriver::endScene() call, when displaying
	the game into a foreign window
	\param rct: Rect to be used when calling endScene(). This also describes the size of the
	window identified by windowId */
	virtual s32 run( s32 windowId=0, core::rect<irr::s32>* rct=0) = 0;
};
	
} // end namespace
} // end namespace

//! typedef for the function type to create a plugin interface inside a .dll
/** will be called by the editor. Return a plugin interface if your plugin should 
 run as plugin inside the editor, otherwise
 just modify the engine (add factories etc) and return 0. */
typedef irr::irredit::IIrrEditPlugin * (__stdcall *CREATEPLUGIN_TYPE)(irr::irredit::IrrEditServices* services);

#endif