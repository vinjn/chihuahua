/** Example 003 Custom SceneNode

This Tutorial is more advanced than the previous ones.
If you are currently just playing around with the Irrlicht
engine, you may want to look at other examples first.
This tutorials shows how to create a custom scene node and
how to use it in the engine. A custom scene node is needed
if you want to implement a render technique the Irrlicht
Engine currently does not support. For example, you can write
an indoor portal based renderer or an advanced terrain scene
node with it. By creating custom scene nodes, you can
easily extend the Irrlicht Engine and adapt it to your own
needs.

I will keep the tutorial simple: Keep everything very
short, everything in one .cpp file, and I'll use the engine
here as in all other tutorials.

To start, I include the header files, use the irr namespace,
and tell the linker to link with the .lib file.
*/
#include <irrlicht.h>
#include "driverChoice.h"

using namespace ue;

/*
Here comes the more sophisticated part of this tutorial:
The class of our very own custom scene node. To keep it simple,
our scene node will not be an indoor portal renderer nor a terrain
scene node, but a simple tetraeder, a 3d object consisting of 4
connected vertices, which only draws itself and does nothing more.
Note that this scenario does not require a custom scene node in Irrlicht.
Instead one would create a mesh from the geometry and pass it to a
ue::scene::IMeshSceneNode. This example just illustrates creation of a custom
scene node in a very simple setting.

To let our scene node be able to be inserted into the Irrlicht
Engine scene, the class we create needs to be derived from the
ue::scene::ISceneNode class and has to override some methods.
*/

#include "FullScreenSceneNode.h"

/*
That's it. The Scene node is done. Now we simply have to start
the engine, create the scene node and a camera, and look at the result.
*/
int main()
{
    // ask user for driver
    video::E_DRIVER_TYPE driverType = video::EDT_OGLES2; //driverChoiceConsole();
    if (driverType == video::EDT_COUNT)
        return 1;

    // create device

    IrrlichtDevice *device = createDevice(driverType,
        core::dimension2d<u32>(640, 480), 16, false);

    if (device == 0)
        return 1; // could not create selected driver.

    // create engine and camera

    device->setWindowCaption(L"Custom Scene Node - Irrlicht Engine Demo");

    video::IVideoDriver* driver = device->getVideoDriver();
    scene::ISceneManager* smgr = device->getSceneManager();

    smgr->addCameraSceneNodeFPS(0/*, core::vector3df(0, -40, 0), core::vector3df(0, 0, 0)*/);

    /*
    Create our scene node. I don't check the result of calling new, as it
    should throw an exception rather than returning 0 on failure. Because
    the new node will create itself with a reference count of 1, and then
    will have another reference added by its parent scene node when it is
    added to the scene, I need to drop my reference to it. Best practice is
    to drop it only *after* I have finished using it, regardless of what
    the reference count of the object is after creation.
    */
    scene::IAnimatedMeshSceneNode* md2 = smgr->addAnimatedMeshSceneNode(smgr->getMesh("../../media/sydney.md2"));
    md2->setMaterialTexture(0, driver->getTexture("../../media/sydney.bmp"));
    //md2->setVisible(false);

    FullScreenSceneNode *myNode =
        new FullScreenSceneNode(smgr->getRootSceneNode(), smgr, 666, 3);
    myNode->Material.setTexture(0, driver->getTexture("../../media/Shanghai5.jpg"));

    /*
    To animate something in this boring scene consisting only of one
    tetraeder, and to show that you now can use your scene node like any
    other scene node in the engine, we add an animator to the scene node,
    which rotates the node a little bit.
    ue::scene::ISceneManager::createRotationAnimator() could return 0, so
    should be checked.
    */
    scene::ISceneNodeAnimator* anim =
        smgr->createRotationAnimator(core::vector3df(0.8f, 0, 0.8f));

    if (anim)
    {
        //myNode->addAnimator(anim);

        /*
        I'm done referring to anim, so must
        ue::IReferenceCounted::drop() this reference now because it
        was produced by a createFoo() function. As I shouldn't refer to
        it again, ensure that I can't by setting to 0.
        */
        anim->drop();
        anim = 0;
    }

    /*
    I'm done with my CSampleSceneNode object, and so must drop my reference.
    This won't delete the object, yet, because it is still attached to the
    scene graph, which prevents the deletion until the graph is deleted or the
    custom scene node is removed from it.
    */
    myNode->drop();
    myNode = 0; // As I shouldn't refer to it again, ensure that I can't

    /*
    Now draw everything and finish.
    */
    u32 frames = 0;
    while (device->run())
    {
        driver->beginScene(true, true, video::SColor(0, 100, 100, 100));

        smgr->drawAll();

        driver->endScene();
        if (++frames == 100)
        {
            core::stringw str = L"Irrlicht Engine [";
            str += driver->getName();
            str += L"] FPS: ";
            str += (s32)driver->getFPS();

            device->setWindowCaption(str.c_str());
            frames = 0;
        }
    }

    device->drop();

    return 0;
}

/*
That's it. Compile and play around with the program.
**/
