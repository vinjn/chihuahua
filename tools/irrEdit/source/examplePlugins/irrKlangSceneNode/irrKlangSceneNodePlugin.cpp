#include <irrlicht.h>
#include <irredit.h>
#include "irrKlangSceneNode.h"

// entry point for the plugin, will be called by the editor. Return a
// plugin interface if your plugin should run as plugin inside the editor, otherwise
// just modify the engine (add factories etc) and return 0.
__declspec(dllexport) irr::irredit::IIrrEditPlugin* __stdcall createPlugin( irr::irredit::IrrEditServices* services )
{
	// we simple register a new scene node factory here, which is only able to
	// create our irrklang scene node
	
	scene::ISceneManager* mgr = services->getDevice()->getSceneManager();

	CIrrKlangSceneNodeFactory* factory = new CIrrKlangSceneNodeFactory(services->getSoundEngine(), mgr);
	mgr->registerSceneNodeFactory(factory);
	factory->drop();

	return 0;
}
