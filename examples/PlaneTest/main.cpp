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

#define WORLD_SIZE 100
#define NODE_COUNT 1
IAnimatedMeshSceneNode* gNodes[NODE_COUNT];

IrrlichtDevice *device;
f32 random(float min, float max)
{
    static IRandomizer* randomizer = device->getRandomizer();
    f32 v = randomizer->frand();
    return min + v * (max - min);
}

int main()
{
	device = createDevice(video::EDT_OGLES2, dimension2d<u32>(800, 600), 16,
			false, false, false, 0);

	if (!device)
		return 1;

	device->setWindowCaption(L"FbxTest");

	IVideoDriver* driver = device->getVideoDriver();
	ISceneManager* smgr = device->getSceneManager();
	IGUIEnvironment* guienv = device->getGUIEnvironment();

    c8* meshFiles[] =
    {
        //"../../media/Cockatoo/Cockatoo_ASC.fbx",
        "../../media/Cockatoo/Cockatoo_ASC.FBX",
    };

    c8* texFiles[] =
    {
        "../../media/Cockatoo/Cockatoo_D.png",
    };

    const float kCamDistZ = 40;
    int idx = 0;
    for (auto node : gNodes)
    {
        auto emptyNode = smgr->addEmptySceneNode();

        IAnimatedMesh* mesh = getMeshFromAssimp(smgr, meshFiles[rand() % _countof(meshFiles)]);

        node = smgr->addAnimatedMeshSceneNode(mesh, emptyNode);
        node->setLoopMode(true);

        core::aabbox3df bbox = node->getBoundingBox();
        float newScale = kCamDistZ * 1.0f / bbox.getRadius();
        node->setScale({ newScale, newScale, newScale });
        node->setMaterialFlag(video::EMF_LIGHTING, false);
        node->setMaterialTexture(0, driver->getTexture(texFiles[rand() % _countof(texFiles)]));
    }

#if 0
    smgr->addCameraSceneNode(0, vector3df(0, 0, -kCamDistZ * 3), vector3df(0, 0, 0));
#else
	auto camera = smgr->addCameraSceneNodeFPS(0);
    camera->setPosition({ 0.0f, 0.0f, -kCamDistZ * 3 });
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
