#include "ExampleGamePlugin.h"

// entry point for the plugin, will be called by the editor. Return a
// plugin interface if your plugin should run as plugin inside the editor, otherwise
// just modify the engine (add factories etc) and return 0.
__declspec(dllexport) irr::irredit::IIrrEditPlugin* __stdcall createPlugin( irr::irredit::IrrEditServices* services )
{
	return new ExampleGamePlugin(services);
}

// -----------------------------------------------

ExampleGamePlugin::ExampleGamePlugin(irredit::IrrEditServices* services)
: Services(services), CurrentSceneManager(0)
{
	if (Services)
		Services->grab();
}

ExampleGamePlugin::~ExampleGamePlugin()
{
	stop();

	if (Services)
		Services->drop();
}

//! will be called to init the plugin status. 
void ExampleGamePlugin::start(ISceneManager* sceneManager, irredit::SIrrEditPluginStartupOptions& options)
{
	CurrentSceneManager = sceneManager;

	if (CurrentSceneManager)
	{
		CurrentSceneManager->grab();

		// create player camera 

		scene::ICameraSceneNode* cam = CurrentSceneManager->addCameraSceneNodeFPS(
			0, 100.0f, 0.5f, -1);

		// set camera position if the editor supplied one

		if (options.DesiredStartPosition != options.DesiredStartLookAtPosition)
		{
			cam->setPosition(options.DesiredStartPosition);
			cam->setTarget(options.DesiredStartLookAtPosition);
		}		
	}
}


//! will be called to stop the currently running plugin. 
void ExampleGamePlugin::stop()
{
	if (CurrentSceneManager)
		CurrentSceneManager->drop();

	CurrentSceneManager = 0;
}

//! runs the plugin in an editor window. run() will be called in a loop until it returns a value != 0.
s32 ExampleGamePlugin::run( s32 windowId, core::rect<irr::s32>* rct)
{
	if (!Services->getDevice() || !CurrentSceneManager || !rct)
		return 1;

	irr::video::IVideoDriver* driver = Services->getDevice()->getVideoDriver();

	// set view port 

	irr::core::rect<irr::s32> rctWindow(*rct);
	driver->setViewPort(rctWindow);

	// draw

	driver->beginScene(true, true, irr::video::SColor(0,255,0,0), reinterpret_cast<void*>(windowId), rct);

	CurrentSceneManager->drawAll();

	Services->getDevice()->getGUIEnvironment()->getBuiltInFont()->draw(L"Press esacpe to end simulation",
		irr::core::rect<irr::s32>(0,0,200,50), video::SColor(100,255,255,255));

	driver->endScene();
	
	return 0;
}