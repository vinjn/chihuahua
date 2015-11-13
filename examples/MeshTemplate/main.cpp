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

class MyEventReceiver : public IEventReceiver
{
public:
    s32 x, y;
    bool LeftButtonDown;

    virtual bool OnEvent(const SEvent& event)
    {
        if (event.EventType == irr::EET_MOUSE_INPUT_EVENT)
        {
            switch (event.MouseInput.Event)
            {
            case EMIE_LMOUSE_PRESSED_DOWN:
                LeftButtonDown = true;
                break;

            case EMIE_LMOUSE_LEFT_UP:
                LeftButtonDown = false;
                break;

            case EMIE_MOUSE_MOVED:
                x = event.MouseInput.X;
                y = event.MouseInput.Y;
                break;

            default:
                // We won't use the wheel
                break;
            }
        }

        if (event.EventType == irr::EET_KEY_INPUT_EVENT)
        {
            if (event.KeyInput.Key == KEY_ESCAPE) device->closeDevice();
        }
        return false;
    }
};
MyEventReceiver eventRecv;

int main(int argc, char const* const* argv)
{
    bx::CommandLine cmdLine(argc, argv);

    device = createDevice(video::EDT_OGLES2, dimension2d<u32>(600, 800), 16,
                          false, false, false, 0);

    if (!device)
        return 1;

    device->setWindowCaption(L"Mesh");
    device->setEventReceiver(&eventRecv);

    Scene_initializeFromDevice((long)device);

    IVideoDriver* driver = device->getVideoDriver();
    ISceneManager* smgr = device->getSceneManager();

    device->getFileSystem()->addFileArchive("../../media/");

    Scene_runScript("test.js");

    //auto shadowDimen = 512;
    //effect->addShadowLight(SShadowLight(shadowDimen, vector3df(0, 0, 0), vector3df(5, 0, 5),
    //    video::SColor(0, 255, 0, 0), 20.0f, 60.0f, 30.0f * DEGTORAD));
    //effect->getShadowLight(0).setPosition({ 100, 100, 100 });

    const float kCamDistZ = 40;

    long nodePtr = Scene_addMeshNode("minion.fbx");
    MeshNode_setAnimationByIndex(nodePtr, 0);
    f32 k = 10;
    Node_setScale(nodePtr, k, k, k);

    //IrrIMGUI::CIMGUIEventReceiver EventReceiver;

#if 1
    //Node_setTexture(nodePtr, Scene_addTexture("polySurface60VRayCompleteMap.jpg"));
#else    
    Node_setTextureAt(nodePtr, 0, Scene_addTexture("polySurface60VRayCompleteMap.jpg"));
    Node_setTextureAt(nodePtr, 1, Scene_addTexture("polySurface60VRayCompleteMap.jpg"));
#endif
    Node_setRotation(nodePtr, 0, 0, 0);

    MeshNode_setShadowMode(nodePtr, Shadow_Both);

    long metaioPtr = Scene_addMeshNode("metaioman.md2");
    MeshNode_setAnimationByIndex(metaioPtr, 0);
    k = 2;
    Node_setPosition(metaioPtr, 100, 100, 100);
    Node_setScale(metaioPtr, k, k, k);
    Node_setTexture(metaioPtr, Scene_addTexture("metaioman.png"));

    MeshNode_setShadowMode(metaioPtr, Shadow_Both);

#if 0
    nodePtr = Scene_addMeshNode("LOGO_new.DAE");
    MeshNode_setAnimationByRange(nodePtr, 450, 500);
    Node_setModelMatrix(nodePtr, m);
    Node_setScale(nodePtr, k, k, k);
#endif

    long mBigPlane = Scene_addPlaneNode(400, 400);
    Node_setTexture(mBigPlane,
                    Scene_addTexture("seymour.jpg"));

    long mSmallPlane = Scene_addPlaneNode(400, 400);
    Node_setTexture(mSmallPlane,
                    Scene_addTexture("seymour.jpg"));

    Node_setPosition(mBigPlane, 0, 0, -500);
    Node_setPosition(mSmallPlane, 100, 20, -500);
    
    Node_setRotation(mBigPlane, 10, 0, 0);
    Node_setRotation(mSmallPlane, 10, 0, 0);

    MeshNode_setShadowMode(mBigPlane, Shadow_Both);
    MeshNode_setShadowMode(mSmallPlane, Shadow_Both);

#if 0
    auto camera = smgr->addCameraSceneNode(0, vector3df(0, 0, 0), vector3df(0, 0, 100));
    smgr->setActiveCamera(camera);
#else
    //auto camera = smgr->addCameraSceneNodeFPS(0);
    //camera->setPosition({ 0.0f, 0.0f, 0.0f });
    //smgr->setActiveCamera(camera);
#endif

    while (device->run())
    {
        //
        // Update matrices
        //
#if 1
        // from Metaio SDK
        float modelMatrix[] =
        {
            0.99942285, 0.020722449, -0.026918545, 0.0,
            -0.02240616, 0.9977092, -0.06383123, 0.0,
            0.025534146, 0.06439753, 0.9975976, 0.0,
            -38.078552, -193.14294, -1045.8368, 1.0
        };
        f32 proj[] =
        {
            3.4011114, 0.0, 0.0, 0.0,
            0.0, 1.9131252, 0.0, 0.0,
            -9.2589855E-4, 5.208254E-4, -1.0033389, -1.0,
            0.0, 0.0, -100.16695, 0.0
        };
#else
        // from HSAR SDK
        float modelMatrix[] =
        {
            0, -1, 0, 0,
            -1, 0, 0, 0,
            0, 0, -1, 0,
            0, 0, 1000, 1
        };
        f32 proj[] =
        {
            0, -1.9131252, 0.0, 0.0,
            -3.4011114, 0, 0.0, 0.0,
            -9.2589855E-4, 5.208254E-4, 1.0033389, 1.0,
            0.0, 0.0, -100.16695, 0.0
        };
#endif

        Node_setModelMatrix(nodePtr, modelMatrix);
        Node_setModelMatrix(metaioPtr, modelMatrix);
        Node_setModelMatrix(mBigPlane, modelMatrix);
        Node_setModelMatrix(mSmallPlane, modelMatrix);

        Camera_setProjectionMatrix(proj);
        //
        // Render
        //
        Scene_clear(122, 122, 122, 122);

        if (eventRecv.LeftButtonDown)
        {
            auto node = (scene::IAnimatedMeshSceneNode*)(Scene_pickNodeFromScreen(eventRecv.x, eventRecv.y));
            if (node)
            {
            }
        }

        Scene_render();
    }

    device->drop();

    return 0;
}
