#ifndef __EXAMPLE_GAME_PLUGIN_H_INCLUDED__
#define __EXAMPLE_GAME_PLUGIN_H_INCLUDED__

#include <irrlicht.h>
#include <irredit.h>

using namespace irr;
using namespace scene;

class ExampleGamePlugin : public irredit::IIrrEditPlugin
{
public:

	ExampleGamePlugin(irredit::IrrEditServices* services);
	~ExampleGamePlugin();

	//! will be called to init the (game)logic status. 
	/** Afterwards run() will be called in a loop until it returns a value != 0.
	 \param sceneManger: The scene manger to be used for this (game)logic instance.  
	 \param options: Structure holding startup parameters. */
	virtual void start(ISceneManager* sceneManager, irredit::SIrrEditPluginStartupOptions& options);

	//! will be called to stop the currently running simulation. 
	/** Implement this to clean up used memory etc. */
	virtual void stop();

	//! runs the plugin in an editor window. run() will be called in a loop until it returns a value != 0.
	/** \param windowId: Id to be used for the IVideoDriver::endScene() call, when displaying
	the game into a foreign window
	\param rct: Rect to be used when calling endScene(). This also describes the size of the
	window identified by windowId */
	virtual s32 run( s32 windowId=0, core::rect<irr::s32>* rct=0);

protected:

	irredit::IrrEditServices* Services;
	scene::ISceneManager* CurrentSceneManager;
};

#endif