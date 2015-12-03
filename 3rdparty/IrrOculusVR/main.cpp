#pragma comment(lib, "Irrlicht.lib")
#include <irrlicht.h>

#include "OculusRenderer.h"

using namespace irr;

int main()
{
	int width = 1280;
	int height = 720;
	irr::video::E_DRIVER_TYPE driverType = video::EDT_DIRECT3D9;

    	// Initialize Irrlicht
	const core::dimension2du videoDim(width,height);

	IrrlichtDevice *device = createDevice(driverType, videoDim, 32, false, false, true );

	video::IVideoDriver* driver = device->getVideoDriver();
	scene::ISceneManager* smgr = device->getSceneManager();


	// Get the window handle for Oculus Rift SDK
	void *window = 0;
	
	if(driverType == irr::video::EDT_DIRECT3D9)
		window = driver->getExposedVideoData().D3D9.HWnd;
	else if(driverType == irr::video::EDT_OPENGL) // OpenGL under windows - no idea how it's done in Linux
		window = driver->getExposedVideoData().OpenGLWin32.HWnd;


	// Initialize Oculus Rift Renderer - world scale set to normal
	OculusRenderer oculusRenderer(window, driver, smgr, 1.0f);


	// FPS camera with no vertical movement.
    	scene::ICameraSceneNode* camera = smgr->addCameraSceneNodeFPS(0, 100.0f, 0.5f, -1,
		0, 0, true,
		0.f, false,
		true);

	camera->setPosition(irr::core::vector3df(10, 10.0f, 0));
	camera->setTarget(irr::core::vector3df(10, 10.0f, 100.0f));


    	// add stuff
	scene::ITerrainSceneNode* terrain = smgr->addTerrainSceneNode(
		"media/terrain.png",
		0,					// parent node
		-1,					// node id
		core::vector3df(0.f, -20.f, 0.f),		// position
		core::vector3df(0.f, 0.f, 0.f),		// rotation
		core::vector3df(1.f, 0.5f, 1.f),	// scale
		video::SColor ( 255, 255, 255, 255 ),	// vertexColor
		5,					// maxLOD
		scene::ETPS_17,				// patchSize
		4					// smoothFactor
		);

	terrain->setMaterialFlag(video::EMF_LIGHTING, false);
	terrain->setMaterialTexture(0,
			driver->getTexture("media/terraintex.png"));


	/* Enable this to try linking head to a rotating animator. Could be used to link player to plane's cockpit etc

	irr::scene::ISceneNode *rotatingNode = smgr->addEmptySceneNode();
	rotatingNode->setPosition(irr::core::vector3df(10, 50, 10));
	irr::scene::ISceneNode *rotatingChild = smgr->addEmptySceneNode(rotatingNode);
	rotatingChild->setPosition(irr::core::vector3df(0.0f,4.0f, 0.0f));
	irr::scene::ISceneNodeAnimator *anim = smgr->createRotationAnimator(irr::core::vector3df(1.0f,0,0.0f));
	rotatingNode->addAnimator(anim);
	anim->drop();

	oculusRenderer.linkHeadNode(rotatingChild);
	*/

    	int frames = 0;

    	while(device->run())
    	{
        	driver->beginScene(true, true, irr::video::SColor(255, 150, 140, 255));

		camera->OnAnimate(device->getTimer()->getTime());
		camera->updateAbsolutePosition();

		oculusRenderer.drawAll(camera->getAbsolutePosition(), camera->getRotation().Y, 
			irr::video::SColor(255, 150, 140, 255));

        	driver->endScene();

		if(++frames == 100)
		{
			irr::core::stringw title = L"IrrOculusVR [FPS: ";
			title += driver->getFPS();
			title += "]";
			device->setWindowCaption(title.c_str());

			frames = 0;
		}
    	}

    	return 0;
}
