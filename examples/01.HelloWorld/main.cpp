#include <irrlicht.h>

/*
In the Irrlicht Engine, everything can be found in the namespace 'irr'. So if
you want to use a class of the engine, you have to write irr:: before the name
of the class. For example to use the IrrlichtDevice write: irr::IrrlichtDevice.
To get rid of the irr:: in front of the name of every class, we tell the
compiler that we use that namespace from now on, and we will not have to write
irr:: anymore.
*/
using namespace irr;

/*
There are 5 sub namespaces in the Irrlicht Engine. Take a look at them, you can
read a detailed description of them in the documentation by clicking on the top
menu item 'Namespace List' or by using this link:
http://irrlicht.sourceforge.net/docu/namespaces.html
Like the irr namespace, we do not want these 5 sub namespaces now, to keep this
example simple. Hence, we tell the compiler again that we do not want always to
write their names.
*/
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

/*
To be able to use the Irrlicht.DLL file, we need to link with the Irrlicht.lib.
We could set this option in the project settings, but to make it easy, we use a
pragma comment lib for VisualStudio. On Windows platforms, we have to get rid
of the console window, which pops up when starting a program with main(). This
is done by the second pragma. We could also use the WinMain method, though
losing platform independence then.
*/
#ifdef _IRR_WINDOWS_
#pragma comment(linker, "/subsystem:console /ENTRY:mainCRTStartup")
#endif


/*
This is the main method. We can now use main() on every platform.
*/
int main()
{
	/*
	The most important function of the engine is the createDevice()
	function. The IrrlichtDevice is created by it, which is the root
	object for doing anything with the engine. createDevice() has 7
	parameters:

	- deviceType: Type of the device. This can currently be the Null-device,
	   one of the two software renderers, D3D8, D3D9, or OpenGL. In this
	   example we use EDT_SOFTWARE, but to try out, you might want to
	   change it to EDT_BURNINGSVIDEO, EDT_NULL, EDT_DIRECT3D8,
	   EDT_DIRECT3D9, or EDT_OPENGL.

	- windowSize: Size of the Window or screen in FullScreenMode to be
	   created. In this example we use 640x480.

	- bits: Amount of color bits per pixel. This should be 16 or 32. The
	   parameter is often ignored when running in windowed mode.

	- fullscreen: Specifies if we want the device to run in fullscreen mode
	   or not.

	- stencilbuffer: Specifies if we want to use the stencil buffer (for
	   drawing shadows).

	- vsync: Specifies if we want to have vsync enabled, this is only useful
	   in fullscreen mode.

	- eventReceiver: An object to receive events. We do not want to use this
	   parameter here, and set it to 0.

	Always check the return value to cope with unsupported drivers,
	dimensions, etc.
	*/
	IrrlichtDevice *device =
        createDevice(video::EDT_DIRECT3D9, dimension2d<u32>(640, 480), 16,
			false, false, false, 0);

	if (!device)
		return 1;

	/*
	Set the caption of the window to some nice text. Note that there is an
	'L' in front of the string. The Irrlicht Engine uses wide character
	strings when displaying text.
	*/
	device->setWindowCaption(L"Hello World! - Irrlicht Engine Demo");

	/*
	Get a pointer to the VideoDriver, the SceneManager and the graphical
	user interface environment, so that we do not always have to write
	device->getVideoDriver(), device->getSceneManager(), or
	device->getGUIEnvironment().
	*/
	IVideoDriver* driver = device->getVideoDriver();
	ISceneManager* smgr = device->getSceneManager();
	IGUIEnvironment* guienv = device->getGUIEnvironment();

	/*
	We add a hello world label to the window, using the GUI environment.
	The text is placed at the position (10,10) as top left corner and
	(260,22) as lower right corner.
	*/
	//guienv->addStaticText(L"Hello World! This is the Irrlicht Engine!", rect<s32>(10,10,260,22), true);

	/*
	To show something interesting, we load a Quake 2 model and display it.
	We only have to get the Mesh from the Scene Manager with getMesh() and add
	a SceneNode to display the mesh with addAnimatedMeshSceneNode(). We
	check the return value of getMesh() to become aware of loading problems
	and other errors.

	Instead of writing the filename sydney.md2, it would also be possible
	to load a Maya object file (.obj), a complete Quake3 map (.bsp) or any
	other supported file format. By the way, that cool Quake 2 model
	called sydney was modelled by Brian Collins.
	*/

    c8* files[] =
    {
        "../../media/metaioman.md2",
    };
    c8* textures[] =
    {
        "../../media/seymour.jpg",
    };
    IAnimatedMeshSceneNode* head = 0;
    const float kCamDistZ = 40;
    int idx = 0;
    for (auto file : files)
    {
        IAnimatedMesh* mesh = smgr->getMesh(file);
        auto node = smgr->addAnimatedMeshSceneNode(mesh);
        node->setAnimation(0);

        if (idx == 0){
            head = node;
        }
/*
0 - 24 cockatoo_takeoff
24 - 40 cockatoo_flying
40 - 62 cockatoo_landing
64 - 82 cockatoo_walking
85 - 130 cockatoo_idle
131 - 190 cockatoo_eating
191 - 211 cockatoo_death
213 - 230 cockatoo_jumping
*/
        //node->setFrameLoop(400, 500);
        node->setLoopMode(true);
        node->setAnimationSpeed(30);

        //node->setPosition({ idx * 20.0f, idx * 5.0f, 0.0f });
        core::aabbox3df bbox = node->getBoundingBox();
        f32 rad = bbox.getRadius();
        float newScale = kCamDistZ * 0.5f / rad;
        //node->setScale(core::vector3df(newScale));
        f32 k = 10000;
        //node->setScale({ k, k, k });

        node->setMaterialFlag(video::EMF_LIGHTING, true);
        //node->setMaterialFlag(video::EMF_BLEND_OPERATION, true);
        //node->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL);
        node->setDebugDataVisible(scene::EDS_SKELETON);
        node->setMaterialTexture(0, driver->getTexture(textures[0]));

        idx++;
    }
    //video::ITexture* texture = driver->getTexture("../../media/duck.png");
    //texture->lock();
    //texture->unlock();

	
	/*
	To look at the mesh, we place a camera into 3d space at the position
	(0, 30, -40). The camera looks from there to (0,5,0), which is
	approximately the place where our md2 model is.
	*/
    {
        scene::ILightSceneNode* light1 = smgr->addLightSceneNode(0, core::vector3df(100, 100, 100));
        light1->setLightType(video::ELT_POINT);
        light1->setRadius(200.f);
        light1->getLightData().DiffuseColor.set(1,1,1);
        auto flyAnimator = smgr->createFlyCircleAnimator({}, 1000);
        light1->addAnimator(flyAnimator);
        flyAnimator->drop();
    }
    //smgr->addCubeSceneNode();
#if 0
    smgr->addCameraSceneNode(0, vector3df(0, kCamDistZ, -kCamDistZ), vector3df(0, 0, 0));
#else
	auto camera = smgr->addCameraSceneNodeFPS(0);
    camera->setPosition({ 0.0f, kCamDistZ, -kCamDistZ });
#endif
	/*
	Ok, now we have set up the scene, lets draw everything: We run the
	device in a while() loop, until the device does not want to run any
	more. This would be when the user closes the window or presses ALT+F4
	(or whatever keycode closes a window).
	*/
	while(device->run())
	{
		/*
		Anything can be drawn between a beginScene() and an endScene()
		call. The beginScene() call clears the screen with a color and
		the depth buffer, if desired. Then we let the Scene Manager and
		the GUI Environment draw their content. With the endScene()
		call everything is presented on the screen.
		*/
		driver->beginScene(true, true, SColor(255,100,101,140));

		smgr->drawAll();
		//guienv->drawAll();

        static int frame = head->getFrameNr();
        if (frame != (int)head->getFrameNr()) {
            printf("%d\n", frame);
            frame = head->getFrameNr();
        }

		driver->endScene();
	}

	/*
	After we are done with the render loop, we have to delete the Irrlicht
	Device created before with createDevice(). In the Irrlicht Engine, you
	have to delete all objects you created with a method or function which
	starts with 'create'. The object is simply deleted by calling ->drop().
	See the documentation at irr::IReferenceCounted::drop() for more
	information.
	*/
	device->drop();

	return 0;
}

/*
That's it. Compile and run.
**/
