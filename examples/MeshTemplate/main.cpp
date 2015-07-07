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

#ifdef _IRR_WINDOWS_
#pragma comment(lib, "Irrlicht.lib")
#pragma comment(linker, "/subsystem:console /ENTRY:mainCRTStartup")
#endif

IrrlichtDevice *device;

int main(int argc, char const* const* argv)
{
    bx::CommandLine cmdLine(argc, argv);

	device = createDevice(video::EDT_BGFX, dimension2d<u32>(800, 600), 16,
			false, false, false, 0);

	if (!device)
		return 1;

	device->setWindowCaption(L"Mesh");

	IVideoDriver* driver = device->getVideoDriver();
	ISceneManager* smgr = device->getSceneManager();

    const float kCamDistZ = 40;

    auto mesh = getMeshFromAssimp(smgr, "c:/Users/vincentz/Downloads/1409111100_37555.obj/file-28.obj");
    auto node = smgr->addMeshSceneNode(mesh);
    node->setRotation({ -90, 0, 0 });
    //node->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false);
    //node->setMaterialFlag(video::EMF_FRONT_FACE_CULLING, true);
    node->setMaterialTexture(0, driver->getTexture("c:/Users/vincentz/Downloads/1409111100_37555.obj/28.jpg"));
    mesh->drop();

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
