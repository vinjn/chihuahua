#include <irrlicht.h>
#include "bx/commandline.h"
#include "bx/float4x4_t.h"
#include "../../Scene3D/src/Scene3D.h"

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

    Scene_initializeFromDevice((long)device);

	IVideoDriver* driver = device->getVideoDriver();
	ISceneManager* smgr = device->getSceneManager();

    const float kCamDistZ = 40;

    long nodePtr = Scene_addMeshNode("../../media/robot-body.DAE");
    MeshNode_setAnimationByIndex(nodePtr, 0);
    f32 k = 10;
    Node_setScale(nodePtr, k, k, k);
    Node_setTextureAt(nodePtr, 0, Scene_addTexture("../../media/polySurface60VRayCompleteMap.jpg"));
    Node_setTextureAt(nodePtr, 1, Scene_addTexture("../../media/polySurface60VRayCompleteMap.jpg"));
    float m[] =
    {
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, -1000, 1
    };
    Node_setModelMatrix(nodePtr, m);
    //node->setRotation({ -90, 0, 0 });
    //node->setMaterialFlag(video::EMF_WIREFRAME, true);
    //((scene::IAnimatedMeshSceneNode*)nodePtr)->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false);
    //((scene::IAnimatedMeshSceneNode*)nodePtr)->setMaterialFlag(video::EMF_FRONT_FACE_CULLING, true);
    //node->getMaterial(0).setTexture(0, driver->getTexture("../../media/polySurface60VRayCompleteMap.jpg"));
    //node->getMaterial(1).setTexture(0, driver->getTexture("../../media/polySurface60VRayCompleteMap.jpg"));
    //node->getMaterial(2).setTexture(0, driver->getTexture("../../media/fire.bmp"));
    //node->getMaterial(3).setTexture(0, driver->getTexture("../../media/fire.bmp"));

    nodePtr = Scene_addMeshNode("../../media/LOGO_new.DAE");
    MeshNode_setAnimationByRange(nodePtr, 450, 500);
    Node_setScale(nodePtr, k, k, k);


    f32 proj[] = {
        3.4011114, 0.0, 0.0, 0.0,
        0.0, 1.9131252, 0.0, 0.0,
        -9.2589855E-4, 5.208254E-4, -1.0033389, -1.0,
        0.0, 0.0, -100.16695, 0.0
    };
    Camera_setProjectionMatrix(proj);

#if 0
    smgr->addCameraSceneNode(0, vector3df(0, 0, -kCamDistZ * 3), vector3df(0, 0, 0));
#else
	//auto camera = smgr->addCameraSceneNodeFPS(0);
    //camera->setPosition({ 0.0f, 0.0f, -kCamDistZ * 3 });
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
