#include <irrlicht.h>
#include "AssimpWrapper.h"

using namespace irr;

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

#ifdef _IRR_WINDOWS_
#pragma comment(lib, "Irrlicht.lib")
#pragma comment(linker, "/subsystem:console /ENTRY:mainCRTStartup")
#endif


int main()
{
	IrrlichtDevice *device =
        createDevice(video::EDT_OGLES2, dimension2d<u32>(800, 600), 16,
			false, false, false, 0);

	if (!device)
		return 1;

	device->setWindowCaption(L"SynthFlow");

	IVideoDriver* driver = device->getVideoDriver();
	ISceneManager* smgr = device->getSceneManager();
	IGUIEnvironment* guienv = device->getGUIEnvironment();

    c8* files[] =
    {
        "../../media/Cockatoo/duck.fbx"
    };

    const float kCamDistZ = 40;
    int idx = 0;
    for (auto file : files)
    {
        IAnimatedMesh* mesh = getMeshFromAssimp(smgr, file);
        for (s32 i = 0; i<mesh->getAnimationCount(); i++)
        {
            s32 begin, end, fps;
            mesh->getFrameLoop(i, begin, end, fps);
            int test = 0;
        }
        auto node = smgr->addAnimatedMeshSceneNode(mesh);

        node->setPosition({ idx++ * 20.0f, idx * 5.0f, 0.0f });
        core::aabbox3df bbox = node->getBoundingBox();
        float newScale = kCamDistZ * 0.5f / bbox.getRadius();
        node->setScale(core::vector3df(newScale));
        node->setMaterialFlag(video::EMF_LIGHTING, false);
        node->setMaterialTexture(0, driver->getTexture("../../media/duck.png"));
    }

#if 1
    smgr->addCameraSceneNode(0, vector3df(0, kCamDistZ, -kCamDistZ), vector3df(0, 0, 0));
#else
	auto camera = smgr->addCameraSceneNodeFPS(0);
    camera->setPosition({ 0.0f, kCamDistZ, -kCamDistZ });
#endif

	while(device->run())
	{
		driver->beginScene(true, true, SColor(255,100,101,140));

		smgr->drawAll();
		//guienv->drawAll();

		driver->endScene();
	}

	device->drop();

	return 0;
}

/*
That's it. Compile and run.
**/
