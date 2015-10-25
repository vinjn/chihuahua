/** Example 015 Loading Scenes from .irr Files

Since version 1.1, Irrlicht is able to save and load
the full scene graph into an .irr file, an xml based
format. There is an editor available to edit
those files, named irrEdit (http://www.ambiera.com/irredit)
which can also be used as world and particle editor.
This tutorial shows how to use .irr files.

Lets start: Create an Irrlicht device and setup the window.
*/

#include <irrlicht.h>
#include "driverChoice.h"
#include "../../source/HMDIrrlicht/HMDStereoRender.h"

using namespace irr;

bool isStereoRendering = false;
bool isGameRunning = true;

class MyEventReceiver : public IEventReceiver
{
public:
    virtual bool OnEvent(const SEvent& event)
    {
        if (event.EventType == EET_KEY_INPUT_EVENT &&
            !event.KeyInput.PressedDown)
        {
            auto key = event.KeyInput.Key;
            if (key == KEY_RETURN)
            {
                isStereoRendering = !isStereoRendering;
                return true;
            }
            if (key == KEY_ESCAPE)
            {
                isGameRunning = false;
                return true;
            }
        }
        return false;
    }
}myEventReceiver;

int main(int argc, char** argv)
{
    // ask user for driver
    video::E_DRIVER_TYPE driverType = video::EDT_OGLES2;

    HMDDescriptor HMD;
    // Parameters from the Oculus Rift DK1
    HMD.hResolution = 1280;
    HMD.vResolution = 800;
    HMD.hScreenSize = 0.14976;
    HMD.vScreenSize = 0.0936;
    HMD.interpupillaryDistance = 0.064;
    HMD.lensSeparationDistance = 0.064;
    HMD.eyeToScreenDistance = 0.041;
    HMD.distortionK[0] = 1.0;
    HMD.distortionK[1] = 0.22;
    HMD.distortionK[2] = 0.24;
    HMD.distortionK[3] = 0.0;

    // create device and exit if creation failed

    IrrlichtDevice* device =
        createDevice(driverType, core::dimension2d<u32>(HMD.hResolution, HMD.vResolution));

    if (device == 0)
        return 1; // could not create selected driver.

    device->setEventReceiver(&myEventReceiver);
    device->setWindowCaption(L"uViewer - Ubiquitous Viewer");

    video::IVideoDriver* driver = device->getVideoDriver();
    scene::ISceneManager* smgr = device->getSceneManager();
    auto timer = device->getTimer();

    HMDStereoRender stereoRenderer(smgr, driver, timer, HMD, 10);

    // load the scene
    if (argc > 1)
    {

        auto fs = device->getFileSystem();
        const io::path oldCWD = fs->getWorkingDirectory();
        auto absPath = fs->getAbsolutePath(argv[1]);
        auto dirPath = fs->getFileDir(absPath);
        fs->changeWorkingDirectoryTo(dirPath);

        smgr->loadScene(argv[1]);

        fs->changeWorkingDirectoryTo(oldCWD);
    }
    else
        smgr->loadScene("../../media/example.irr");


    /*
    Now we'll create a camera, and give it a collision response animator
    that's built from the mesh nodes in the scene we just loaded.
    */
    SKeyMap keyMap[] =
    {
        { EKA_MOVE_FORWARD, KEY_KEY_W },
        { EKA_MOVE_BACKWARD, KEY_KEY_S },
        { EKA_STRAFE_LEFT, KEY_KEY_A },
        { EKA_STRAFE_RIGHT, KEY_KEY_D },
        { EKA_JUMP_UP, KEY_SPACE },
        { EKA_CROUCH, KEY_LCONTROL },
    };

    scene::ICameraSceneNode * camera = smgr->addCameraSceneNodeFPS(0, 50.f, 0.1f, -1,
        keyMap, _countof(keyMap),
        false, 1);

    // Create a meta triangle selector to hold several triangle selectors.
    scene::IMetaTriangleSelector * meta = smgr->createMetaTriangleSelector();

    /*
    Now we will find all the nodes in the scene and create triangle
    selectors for all suitable nodes.  Typically, you would want to make a
    more informed decision about which nodes to performs collision checks
    on; you could capture that information in the node name or Id.
    */
    core::array<scene::ISceneNode *> nodes;
    smgr->getSceneNodesFromType(scene::ESNT_ANY, nodes); // Find all nodes

    for (u32 i = 0; i < nodes.size(); ++i)
    {
        scene::ISceneNode * node = nodes[i];
        scene::ITriangleSelector * selector = 0;

        switch (node->getType())
        {
        case scene::ESNT_CUBE:
        case scene::ESNT_ANIMATED_MESH:
            // Because the selector won't animate with the mesh,
            // and is only being used for camera collision, we'll just use an approximate
            // bounding box instead of ((scene::IAnimatedMeshSceneNode*)node)->getMesh(0)
            selector = smgr->createTriangleSelectorFromBoundingBox(node);
            break;

        case scene::ESNT_MESH:
        case scene::ESNT_SPHERE: // Derived from IMeshSceneNode
            selector = smgr->createTriangleSelector(((scene::IMeshSceneNode*)node)->getMesh(), node);
            break;

        case scene::ESNT_TERRAIN:
            selector = smgr->createTerrainTriangleSelector((scene::ITerrainSceneNode*)node);
            break;

        case scene::ESNT_OCTREE:
            selector = smgr->createOctreeTriangleSelector(((scene::IMeshSceneNode*)node)->getMesh(), node);
            break;

        default:
            // Don't create a selector for this node type
            break;
        }

        if (selector)
        {
            // Add it to the meta selector, which will take a reference to it
            meta->addTriangleSelector(selector);
            // And drop my reference to it, so that the meta selector owns it.
            selector->drop();
        }
    }

    /*
    Now that the mesh scene nodes have had triangle selectors created and added
    to the meta selector, create a collision response animator from that meta selector.
    */
    scene::ISceneNodeAnimator* anim = smgr->createCollisionResponseAnimator(
        meta, camera,
        core::vector3df(5, 5, 5),
        core::vector3df(0, -3, 0),
        core::vector3df(0, 15, 0)
        );
    meta->drop(); // I'm done with the meta selector now

    camera->addAnimator(anim);
    anim->drop(); // I'm done with the animator now

    // And set the camera position so that it doesn't start off stuck in the geometry
    camera->setPosition(core::vector3df(0.f, 20.f, 0.f));

    // Point the camera at the cube node, by finding the first node of type ESNT_CUBE
    scene::ISceneNode * cube = smgr->getSceneNodeFromType(scene::ESNT_CUBE);
    if (cube)
        camera->setTarget(cube->getAbsolutePosition());

    /*
    That's it. Draw everything and finish as usual.
    */

    int lastFPS = -1;

    while (device->run() && isGameRunning)
    {
        if (!device->isWindowActive()) continue;

        driver->beginScene(true, true, video::SColor(0, 200, 200, 200));

        if (isStereoRendering)
        {
            stereoRenderer.drawAll();
        }
        else
        {
            smgr->drawAll();
        }
        driver->endScene();

        int fps = driver->getFPS();

        if (lastFPS != fps)
        {
            core::stringw str = L"uViewer - Ubiquitous Viewer FPS: ";
            str += fps;

            device->setWindowCaption(str.c_str());
            lastFPS = fps;
        }
    }
    device->drop();

    return 0;
}
