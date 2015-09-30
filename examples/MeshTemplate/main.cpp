#include <irrlicht.h>
#include "bx/commandline.h"
#include "bx/float4x4_t.h"

using namespace irr;

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

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

    auto mesh = smgr->getMesh("../../media/robot-body.DAE");
    auto node = smgr->addAnimatedMeshSceneNode(mesh);
    node->setAnimation(0);
    f32 k = 10;
    node->setScale({ k, k, k });
    //node->setRotation({ -90, 0, 0 });
    //node->setMaterialFlag(video::EMF_WIREFRAME, true);
    node->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false);
    node->setMaterialFlag(video::EMF_FRONT_FACE_CULLING, true);
    node->getMaterial(0).setTexture(0, driver->getTexture("../../media/polySurface60VRayCompleteMap.jpg"));
    node->getMaterial(1).setTexture(0, driver->getTexture("../../media/polySurface60VRayCompleteMap.jpg"));
    //node->getMaterial(2).setTexture(0, driver->getTexture("../../media/fire.bmp"));
    //node->getMaterial(3).setTexture(0, driver->getTexture("../../media/fire.bmp"));


    mesh = smgr->getMesh("../../media/LOGO_new.dae");
    node = smgr->addAnimatedMeshSceneNode(mesh);
    node->setFrameLoop(450, 500);
    node->setScale({ k, k, k });

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
		driver->endScene();
	}

	device->drop();

	return 0;
}
