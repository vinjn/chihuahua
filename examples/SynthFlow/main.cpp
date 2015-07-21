#include <irrlicht.h>
#include "AssimpWrapper.h"
#include <unordered_map>
#include <vector>

using namespace std;
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
#pragma comment(linker, "/subsystem:console /ENTRY:mainCRTStartup")

#define WINDOW_W 800
#define WINDOW_H 800

#define NODE_COUNT_SHIFT 6
#define WORLD_SIZE 100
#define NODE_COUNT 2<<NODE_COUNT_SHIFT

IVideoDriver* driver;
ISceneManager* smgr;
ISceneCollisionManager* coll;
IAnimatedMeshSceneNode* gNodes[NODE_COUNT];

IrrlichtDevice *device;
f32 random(float min, float max)
{
    static IRandomizer* randomizer = device->getRandomizer();
    f32 v = randomizer->frand();
    return min + v * (max - min);
}

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

struct Feature
{
    // Key?
    s32 nodeId;
    s32 triangleId;

    s32 getKey() const
    {
        return getKey(nodeId, triangleId);
    }

    static s32 getKey(s32 node, s32 triangle)
    {
        return node + (triangle << NODE_COUNT_SHIFT);
    }

    // interm
    vector2df centroidUv;

    // Value?
    triangle3df triangleModel;
    triangle3df triangleWorld;
};

struct Frame
{
    matrix4 world_matrix;
    matrix4 view_matrix;
    matrix4 projection_matrix;

    unordered_map<s32, Feature> features;

    void update()
    {
        features.clear();

#if 0
        int x = eventRecv.x;
        int y = eventRecv.y;
        {
            {
#else
#define SPACING 100
        for (int x = 0; x < WINDOW_W - SPACING; x += SPACING)
        {
            for (int y = 0; y < WINDOW_H - SPACING; y += SPACING)
            {
#endif
                auto ray = coll->getRayFromScreenCoordinates(position2di(x, y));
                vector3df hitPt;
                triangle3df hitTri;
                s32 hitTriId;
                ISceneNode* hitNode = coll->getSceneNodeAndCollisionPointFromRay(ray,
                    hitPt, hitTri, 0, 0, false, &hitTriId);
                
                if (hitNode)
                {
                    Feature feature =
                    { 
                        hitNode->getID(),
                        hitTriId,
                        vector2df(),
                        triangle3df(),
                        hitTri
                    };
                    features.emplace(feature.getKey(), feature);
                }
            }
        }
    }

    void debugDraw()
    {
        video::SMaterial material;
        material.ZBuffer = ECFN_DISABLED;
        material.Wireframe = true;
        driver->setTransform(video::ETS_WORLD, core::matrix4());
        driver->setMaterial(material);

        for (auto& kv : features)
        {
            auto triangle = kv.second.triangleWorld;
            driver->draw3DTriangle(triangle, video::SColor(0, 255, 0, 0));
        }
    }
};

vector<vector2df> trackFrameMotion(const Frame& frame_prev, const Frame& frame_curr)
{
    vector<vector2df> motion;

    return motion;
}

void drawFrameDiff(Frame& prevFrame, Frame& currFrame)
{
    video::SMaterial material;
    material.ZBuffer = ECFN_DISABLED;
    material.Wireframe = false;
    driver->setTransform(video::ETS_WORLD, core::matrix4());
    driver->setMaterial(material);

    for (auto& kv : prevFrame.features)
    {
        s32 key = kv.first;
        if (currFrame.features.count(key))
        {
            const auto& prevValue = prevFrame.features.at(key);
            const auto& currValue = currFrame.features.at(key);
            driver->draw3DLine(prevValue.triangleWorld.pointA, currValue.triangleWorld.pointA,
                SColor(0, 255, 0, 0));
        }
    }
}

int main()
{
    device = createDevice(video::EDT_DIRECT3D9, dimension2d<u32>(WINDOW_W, WINDOW_H), 16,
			false, false, false, 0);

	if (!device)
		return 1;

	device->setWindowCaption(L"SynthFlow");

	driver = device->getVideoDriver();
	smgr = device->getSceneManager();
    coll = smgr->getSceneCollisionManager();
    device->setEventReceiver(&eventRecv);

    scene::ISceneNode* skydome = smgr->addSkyDomeSceneNode(driver->getTexture("../../media/skydome.jpg"), 16, 8, 0.95f, 2.0f);

    c8* meshFiles[] =
    {
        //"../../media/duck.fbx",
        "../../media/Cockatoo/Cockatoo.FBX",
    };

    c8* texFiles[] =
    {
        "../../media/Shanghai5.jpg",
        "../../media/Belgium_block_pxr128.jpg",
        "../../media/Slag_stone_pxr128.jpg",
        "../../media/dwarf.jpg",
        "../../media/Cockatoo/Cockatoo_D.png",
    };

    const float kCamDistZ = 40;
    int idx = 0;
    int sNodeId = 0;
    for (auto node : gNodes)
    {
        auto emptyNode = smgr->addEmptySceneNode();
        emptyNode->setPosition({
            random(-WORLD_SIZE, WORLD_SIZE),
            random(-WORLD_SIZE, WORLD_SIZE),
            random(-WORLD_SIZE, WORLD_SIZE)
        });

        IAnimatedMesh* mesh = getMeshFromAssimp(smgr, meshFiles[rand() % _countof(meshFiles)]);
        node = smgr->addAnimatedMeshSceneNode(mesh, emptyNode);
        node->setID(sNodeId++);
        node->setFrameLoop(0, 0);
        auto selector = smgr->createTriangleSelector(node);
        node->setTriangleSelector(selector);
        selector->drop();

        aabbox3df bbox = node->getBoundingBox();
        float newScale = kCamDistZ * 0.6f / bbox.getRadius();
        node->setScale({ newScale, newScale, newScale });
        node->setMaterialFlag(video::EMF_LIGHTING, false);
        node->setMaterialTexture(0, driver->getTexture(texFiles[rand() % _countof(texFiles)]));

        // animator
        float kRotation = 0.05f;
        auto rotAnimator = smgr->createRotationAnimator({
            random(0, kRotation),
            random(0, kRotation),
            random(0, kRotation),
        });
        node->addAnimator(rotAnimator);
        rotAnimator->drop();

        auto flyAnimator = smgr->createFlyCircleAnimator({}, random(0, 20));
        //node->addAnimator(flyAnimator);
        flyAnimator->drop();
    }

#if 1
    smgr->addCameraSceneNode(0, vector3df(0, 0, -kCamDistZ * 4), vector3df(0, 0, 0));
#else
	auto camera = smgr->addCameraSceneNodeFPS(0);
    camera->setPosition({ 0.0f, 0.0f, -kCamDistZ * 3 });
#endif

    int prevFrameIdx = 0;
    int currFrameIdx = 1;
    Frame frames[2];

	while(device->run())
	{
		driver->beginScene(true, true, SColor(255,100,101,140));

        auto& prevFrame = frames[prevFrameIdx];
        auto& currFrame = frames[currFrameIdx];

		smgr->drawAll();
        currFrame.update();
        currFrame.debugDraw();

        drawFrameDiff(prevFrame, currFrame);

		driver->endScene();

        swap(currFrameIdx, prevFrameIdx);
	}

	device->drop();

	return 0;
}

/*
That's it. Compile and run.
**/
