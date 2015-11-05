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

	device = createDevice(video::EDT_BGFX, dimension2d<u32>(800, 600), 16,
			false, false, false, 0);

	if (!device)
		return 1;

	device->setWindowCaption(L"PlaneTest");

	IVideoDriver* driver = device->getVideoDriver();
	ISceneManager* smgr = device->getSceneManager();
    IGPUProgrammingServices* gpu = driver->getGPUProgrammingServices();

#include "../../media/bgfx-shaders/COGLES2Solid.vsh"
#include "../../media/bgfx-shaders/COGLES2Solid.fsh"

    struct MyShaderCallBack : public video::IShaderConstantSetCallBack
    {
        void OnSetConstants(video::IMaterialRendererServices* services,
            s32 userData)
        {
        }
    };
    s32 mtrlId = gpu->addHighLevelShaderMaterial(COGLES2Solid_vsh, COGLES2Solid_fsh, new MyShaderCallBack());
    const float kCamDistZ = 40;

    scene::IMesh* planeMesh = smgr->getGeometryCreator()->createPlaneMesh({ 10, 10 });
    scene::IMeshSceneNode* node = smgr->addMeshSceneNode(planeMesh);
    node->setRotation({ -90, 0, 0 });
    //node->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false);
    //node->setMaterialFlag(video::EMF_FRONT_FACE_CULLING, true);
    node->setMaterialTexture(0, driver->getTexture("../../media/duck.png"));
    node->setMaterialType((video::E_MATERIAL_TYPE)mtrlId);

    planeMesh->drop();

#if 1
    smgr->addCameraSceneNode(0, vector3df(0, 0, -kCamDistZ * 3), vector3df(0, 0, 0));
#else
	auto camera = smgr->addCameraSceneNodeFPS(0);
    camera->setPosition({ 0.0f, 0.0f, -kCamDistZ * 3 });
#endif

	while (device->run())
	{
		driver->beginScene(true, true, SColor(255,100,101,140));

		smgr->drawAll();
		//guienv->drawAll();

		driver->endScene();
	}

	device->drop();

	return 0;
}
