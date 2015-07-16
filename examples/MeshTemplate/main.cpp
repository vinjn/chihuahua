#include <irrlicht.h>
#include "AssimpWrapper.h"
#include "bx/commandline.h"
#include "bx/float4x4_t.h"

using namespace irr;

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

#ifdef _DEBUG
#pragma comment(lib, "Irrlicht_d.lib")
#else
#pragma comment(lib, "Irrlicht.lib")
#endif

IrrlichtDevice *device;

int main(int argc, char const* const* argv)
{
    bx::CommandLine cmdLine(argc, argv);

	device = createDevice(video::EDT_OGLES2, dimension2d<u32>(800, 600), 16,
			false, false, false, 0);

	if (!device)
		return 1;

	device->setWindowCaption(L"Mesh");

	IVideoDriver* driver = device->getVideoDriver();
	ISceneManager* smgr = device->getSceneManager();

    const float kCamDistZ = 40;

    auto mesh = getMeshFromAssimp(smgr, "../../media/chair.fbx");
    auto node = smgr->addMeshSceneNode(mesh);
    node->setRotation({ -90, 0, 0 });
    //node->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false);
    //node->setMaterialFlag(video::EMF_FRONT_FACE_CULLING, true);
    //node->setMaterialTexture(0, driver->getTexture("../../media/metaioman.png"));

#if 1
    smgr->addCameraSceneNode(0, vector3df(0, 0, -kCamDistZ * 3), vector3df(0, 0, 0));
#else
	auto camera = smgr->addCameraSceneNodeFPS(0);
    camera->setPosition({ 0.0f, 0.0f, -kCamDistZ * 3 });
#endif

	while(device->run())
	{
		driver->beginScene(true, true, SColor(255,100,101,140));

		smgr->drawAll();
		driver->endScene();
	}

	device->drop();

	return 0;
}
