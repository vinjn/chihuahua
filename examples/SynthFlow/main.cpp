#include <irrlicht.h>
#include "AssimpWrapper.h"
#include <unordered_map>
#include <vector>
#include <functional>
#include "../source/Irrlicht/CD3D9HLSLMaterialRenderer.h"
#include <assert.h>

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

#define WINDOW_W 400
#define WINDOW_H 400

#define WORLD_SIZE 60
#define NODE_SIZE_X 1
#define NODE_SIZE_Y 1
#define NODE_COUNT NODE_SIZE_X * NODE_SIZE_Y

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

struct FlowImage
{
    vector<vector2df> data;
    int width, height;

    void setup(int w, int h)
    {
        width = w;
        height = h;
        data.resize(w * h);
        //std::fill_n(data.begin(), data.size(), vector2df(0.1f, 0.1f));
    }

    vector2df& at(int x, int y)
    {
        return data[y * width+ x];
    }
};

/*
Please refer data format and flow data/image naming of MPI Sintel, summarized as the below.
Data is binary file (referred in flow_code/C/flowIO.cpp),
Bytes   contents
0-3         tag that indicates storage type, i.e. little endian or big endian, a sample float value
4-7         width as an integer
8-11       height as an integer
12-end  data (width * height* 2* 4 bytes total)

Data naming,
Data flow files have a root directory as ‘flow’
Each scene has a directory containing this scene sequence images, i.e. scene_a
Data file naming as prefix_04d.flo, prefix is ‘frame’, 04d is frame idx
Image naming,
Image files have a root directory as ‘images’
Each scene has also a directory as the same as Data naming
Image file naming as prefix_04d.png, prefix is ‘frame’, 04d is frame idx
*/

// write a 2-band image into flow file 
void WriteFlowFile(FlowImage& img, const char* filename)
{
#define TAG_FLOAT 202021.25  // check for this when READING the file
#define TAG_STRING "PIEH"    // use this when WRITING the file 

    if (filename == NULL)
        printf("WriteFlowFile: empty filename");

    const char *dot = strrchr(filename, '.');
    if (dot == NULL)
        printf("WriteFlowFile: extension required in filename '%s'", filename);

    if (strcmp(dot, ".flo") != 0)
        printf("WriteFlowFile: filename '%s' should have extension '.flo'", filename);

    int width = img.width, height = img.height, nBands = 2;

    if (nBands != 2)
        printf("WriteFlowFile(%s): image must have 2 bands", filename);

    FILE *stream = fopen(filename, "wb");
    if (stream == 0)
        printf("WriteFlowFile: could not open %s", filename);

    // write the header
    fprintf(stream, TAG_STRING);
    if ((int)fwrite(&width, sizeof(int), 1, stream) != 1 ||
        (int)fwrite(&height, sizeof(int), 1, stream) != 1)
        printf("WriteFlowFile(%s): problem writing header", filename);

    // write the rows
    int n = nBands * width;
    for (int y = 0; y < height; y++) {
        float* ptr = &img.data[y * width].X;
        if ((int)fwrite(ptr, sizeof(float), n, stream) != n)
            printf("WriteFlowFile(%s): problem writing data", filename);
    }

    fclose(stream);
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
        return node + (triangle * NODE_COUNT);
    }

    // interm
    vector2df centroidUv;

    // Value?
    vector2df triangleScreen[3];
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
#define SPACING 1
        //#pragma omp parallel for
        for (int x = 0; x < WINDOW_W - SPACING + 1; x += SPACING)
        {
            for (int y = 0; y < WINDOW_H - SPACING + 1; y += SPACING)
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
                    auto scrHit = coll->getScreenCoordinatesFrom3DPositionF32(hitPt);

                    Feature feature =
                    {
                        hitNode->getID(),
                        hitTriId,
                        vector2df(),
                        {
                            coll->getScreenCoordinatesFrom3DPositionF32(hitTri.pointA),
                            coll->getScreenCoordinatesFrom3DPositionF32(hitTri.pointB),
                            coll->getScreenCoordinatesFrom3DPositionF32(hitTri.pointC)
                        },
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
        material.Lighting = false;
        driver->setTransform(video::ETS_WORLD, core::matrix4());
        driver->setMaterial(material);

        for (auto& kv : features)
        {
            auto triangle = kv.second.triangleWorld;
            driver->draw3DTriangle(triangle, video::SColor(100, 255, 0, 0));
        }
    }

    static void writeFlowImage(Frame& prevFrame, Frame& currFrame, const c8* filename)
    {
        FlowImage flowImage;
        flowImage.setup(WINDOW_W, WINDOW_H);

        printf("\n");
        int counter = 0;
        for (auto& kv : prevFrame.features)
        {
            s32 key = kv.first;
            if (currFrame.features.count(key))
            {
                const auto& prevValue = prevFrame.features.at(key);
                const auto& currValue = currFrame.features.at(key);

                for (int i = 0; i < 3; i++)
                {
                    auto& prev = prevValue.triangleScreen[i];
                    auto& curr = currValue.triangleScreen[i];
                    flowImage.at(prev.X, prev.Y) = curr - prev;

                    //driver->draw2DLine(vector2di(prev.X, prev.Y), vector2di(curr.X, curr.Y), SColor(255, 0, 255, 255));
                }
            }
        }
        WriteFlowFile(flowImage, filename);

        char info[256];
        sprintf(info, "%s.txt", filename);
        FILE* fp = fopen(info, "w");
        for (int y = 0; y < WINDOW_H; y++)
            for (int x = 0; x < WINDOW_W; x++)
        {
            auto& flow = flowImage.at(x, y);
            if (flow.X != 0 && flow.Y != 0)
            {
                fprintf(fp, "(%d,%d) -> %.1f, %.1f\n", x, y,flow.X, flow.Y);
            }
        }

        fclose(fp);
    }

    static void drawDiff(Frame& prevFrame, Frame& currFrame)
    {
        video::SMaterial material;
        material.ZBuffer = ECFN_DISABLED;
        material.Wireframe = false;
        material.Lighting = false;
        driver->setTransform(video::ETS_WORLD, core::matrix4());
        driver->setMaterial(material);

#if 0
        for (auto& kv : prevFrame.features)
        {
            s32 key = kv.first;
            if (currFrame.features.count(key))
            {
                const auto& prevValue = prevFrame.features.at(key);
                const auto& currValue = currFrame.features.at(key);
                assert(prevValue.nodeId == currValue.nodeId);
                assert(prevValue.triangleId == currValue.triangleId);

                driver->draw3DLine(prevValue.triangleWorld.pointA, currValue.triangleWorld.pointA,
                    SColor(0, 255, 255, 0));
                driver->draw3DLine(prevValue.triangleWorld.pointB, currValue.triangleWorld.pointB,
                    SColor(0, 255, 255, 0));
                driver->draw3DLine(prevValue.triangleWorld.pointC, currValue.triangleWorld.pointC,
                    SColor(0, 255, 255, 0));
            }
        }
#endif

        for (auto& kv : prevFrame.features)
        {
            s32 key = kv.first;
            if (currFrame.features.count(key))
            {
                const auto& prevValue = prevFrame.features.at(key);
                const auto& currValue = currFrame.features.at(key);
                assert(prevValue.nodeId == currValue.nodeId);
                assert(prevValue.triangleId == currValue.triangleId);

                //driver->draw2DLine(prevValue.triangleScreen[0], currValue.triangleScreen[0],
                //    SColor(255, 0, 255, 255));
                //driver->draw2DLine(prevValue.triangleScreen[1], currValue.triangleScreen[1],
                //    SColor(255, 0, 255, 255));
                //driver->draw2DLine(prevValue.triangleScreen[2], currValue.triangleScreen[2],
                //    SColor(255, 0, 255, 255));
            }
        }
    }
};

vector<vector2df> trackFrameMotion(const Frame& frame_prev, const Frame& frame_curr)
{
    vector<vector2df> motion;

    return motion;
}


class MyShaderCallBack : public video::IShaderConstantSetCallBack
{
public:
    MyShaderCallBack() : WorldViewProjID(-1), TransWorldID(-1), InvWorldID(-1), PositionID(-1),
        ColorID(-1), TextureID(-1), FirstUpdate(true)
    {
    }

    virtual void OnSetConstants(video::IMaterialRendererServices* services,
        s32 userData)
    {
        video::IVideoDriver* driver = services->getVideoDriver();

        // get shader constants id.

        if (FirstUpdate)
        {
            WorldViewProjID = services->getVertexShaderConstantID("mWorldViewProj");
            TransWorldID = services->getVertexShaderConstantID("mTransWorld");
            InvWorldID = services->getVertexShaderConstantID("mInvWorld");
            PositionID = services->getVertexShaderConstantID("mLightPos");
            ColorID = services->getVertexShaderConstantID("mLightColor");

            // Textures ID are important only for OpenGL interface.

            if (driver->getDriverType() == video::EDT_OPENGL)
                TextureID = services->getVertexShaderConstantID("myTexture");

            FirstUpdate = false;
        }

        // set inverted world matrix
        // if we are using highlevel shaders (the user can select this when
        // starting the program), we must set the constants by name.

        core::matrix4 invWorld = driver->getTransform(video::ETS_WORLD);
        invWorld.makeInverse();

        services->setVertexShaderConstant(InvWorldID, invWorld.pointer(), 16);

        // set clip matrix

        core::matrix4 worldViewProj;
        worldViewProj = driver->getTransform(video::ETS_PROJECTION);
        worldViewProj *= driver->getTransform(video::ETS_VIEW);
        worldViewProj *= driver->getTransform(video::ETS_WORLD);

        services->setVertexShaderConstant(WorldViewProjID, worldViewProj.pointer(), 16);

        // set camera position

        core::vector3df pos = device->getSceneManager()->
            getActiveCamera()->getAbsolutePosition();

        services->setVertexShaderConstant(PositionID, reinterpret_cast<f32*>(&pos), 3);

        // set light color

        video::SColorf col(0.0f, 1.0f, 1.0f, 0.0f);

        services->setVertexShaderConstant(ColorID,
            reinterpret_cast<f32*>(&col), 4);

        // set transposed world matrix

        core::matrix4 world = driver->getTransform(video::ETS_WORLD);
        world = world.getTransposed();

        services->setVertexShaderConstant(TransWorldID, world.pointer(), 16);

        // set texture, for textures you can use both an int and a float setPixelShaderConstant interfaces (You need it only for an OpenGL driver).
        s32 TextureLayerID = 0;
        services->setPixelShaderConstant(TextureID, &TextureLayerID, 1);
    }

private:
    s32 WorldViewProjID;
    s32 TransWorldID;
    s32 InvWorldID;
    s32 PositionID;
    s32 ColorID;
    s32 TextureID;

    bool FirstUpdate;
};

struct ActionAnimator : public ISceneNodeAnimator
{
    typedef std::function<void(ISceneNode*, u32)> Action;

    static void addActionToNode(ISceneNode* node, Action action)
    {
        ActionAnimator* actionAnimator = new ActionAnimator(action);
        node->addAnimator(actionAnimator);
        actionAnimator->drop();
    }

    ActionAnimator(Action action)
    {
        this->action = action;
    }

    virtual void animateNode(ISceneNode* node, u32 timeMs)
    {
        action(node, timeMs);
    }

    virtual ISceneNodeAnimator* createClone(ISceneNode* node,
        ISceneManager* newManager)
    {
        ActionAnimator * newAnimator = new ActionAnimator(action);
        newAnimator->cloneMembers(this);
        return newAnimator;
    }

    Action action;
};

int main()
{
    device = createDevice(video::EDT_DIRECT3D9, dimension2d<u32>(WINDOW_W, WINDOW_H), 16,
        false, false, false, 0);

    if (!device)
        return 1;

    system("mkdir flow");
    system("mkdir images");
    system("mkdir flow\\dummy");
    system("mkdir images\\dummy");

    device->setWindowCaption(L"SynthFlow");

    driver = device->getVideoDriver();
    smgr = device->getSceneManager();
    coll = smgr->getSceneCollisionManager();
    device->setEventReceiver(&eventRecv);

    scene::ISceneNode* skydome = smgr->addSkyDomeSceneNode(driver->getTexture("../../media/skydome.jpg"), 16, 8, 0.95f, 2.0f);

    c8* meshFiles[] =
    {
        "c:\\Users\\vincentz\\Documents\\untitled.obj",
        //"../../media/plane.obj",
        //"../../media/duck.fbx",
        //"../../media/Cockatoo/Cockatoo.FBX",
    };

    c8* texFiles[] =
    {
        "../../media/Shanghai5.jpg",
        "../../media/Belgium_block_pxr128.jpg",
        "../../media/Slag_stone_pxr128.jpg",
        "../../media/dwarf.jpg",
        //"../../media/Cockatoo/Cockatoo_D.png",
    };

    auto mani = driver->getMeshManipulator();

    const float kCamDistZ = 40;
    int idx = 0;
    int sNodeId = 0;

    for (int x = 0; x < NODE_SIZE_X; x++)
        for (int y = 0; y < NODE_SIZE_Y; y++)
    {
        auto& node = gNodes[y * NODE_SIZE_X + x];
        auto emptyNode = smgr->addDummyTransformationSceneNode();

        emptyNode->getRelativeTransformationMatrix().setTranslation({
            core::lerp<f32>(-WORLD_SIZE, WORLD_SIZE, (float)x / (core::max_<int>(1, NODE_SIZE_X - 1))),
            core::lerp<f32>(-WORLD_SIZE, WORLD_SIZE, (float)y / (core::max_<int>(1, NODE_SIZE_Y - 1))),
            0
        });

#if 0
        IAnimatedMesh* mesh = getMeshFromAssimp(smgr, meshFiles[rand() % _countof(meshFiles)]);
        auto staticMesh = mesh->getMesh(0);
        auto meshTwoTex = mani->createMeshWith2TCoords(staticMesh);
        auto aniMesh = smgr->getMeshManipulator()->createAnimatedMesh(meshTwoTex);
        node = smgr->addAnimatedMeshSceneNode(aniMesh, emptyNode);
        meshTwoTex->drop();
        aniMesh->drop();
        node->setFrameLoop(0, 0);
#else
        auto mesh = smgr->getGeometryCreator()->createHillPlaneMesh({ 1.0f, 1.0f }, { 10, 10 }, NULL, 1.0f, { 1.0f, 1.0f }, { 1, 1 });
        auto aniMesh = smgr->getMeshManipulator()->createAnimatedMesh(mesh);
        node = smgr->addAnimatedMeshSceneNode(aniMesh, emptyNode);
        node->setMaterialFlag(EMF_BACK_FACE_CULLING, false);
        node->setRotation({ 90.0f, 20.0f, 20.0f });
        mesh->drop();
        aniMesh->drop();
#endif
        node->setID(sNodeId++);
        auto selector = smgr->createTriangleSelector(node);
        node->setTriangleSelector(selector);
        selector->drop();

        aabbox3df bbox = node->getBoundingBox();
        node->setScale({ 2.0f, 2.0f, 2.0f });
        node->setMaterialFlag(video::EMF_LIGHTING, false);
        node->setMaterialTexture(0, driver->getTexture(texFiles[rand() % _countof(texFiles)]));

        // animator
        vector3df kRotation = { 0.0f, 0.0f, 0.1f };
#if 0
        auto rotAnimator = smgr->createRotationAnimator({
            random(0.0f, kRotation.X),
            random(0.0f, kRotation.Y),
            random(0.0f, kRotation.Z),
        });
        node->addAnimator(rotAnimator);
        rotAnimator->drop();
        auto flyAnimator = smgr->createFlyCircleAnimator({}, 1);
        node->addAnimator(flyAnimator);
        flyAnimator->drop();
#else
        u32 StartTime = device->getTimer()->getTime();

        ActionAnimator::addActionToNode(node, [&StartTime, &kRotation](ISceneNode* node, u32 timeMs){
            printf("%d\n", node->getID());
            const u32 diffTime = timeMs - StartTime;

            if (diffTime != 0)
            {
                // clip the rotation to small values, to avoid
                // precision problems with huge floats.
                core::vector3df rot = node->getRotation() + kRotation*(diffTime*0.1f);
                if (rot.X>360.f)
                    rot.X = fmodf(rot.X, 360.f);
                if (rot.Y>360.f)
                    rot.Y = fmodf(rot.Y, 360.f);
                if (rot.Z>360.f)
                    rot.Z = fmodf(rot.Z, 360.f);
                node->setRotation(rot);
                StartTime = timeMs;
            }
        });
#endif
    }

#if 1
    smgr->addCameraSceneNode(0, vector3df(0, 0, -kCamDistZ * 5), vector3df(0, 0, 0));
#else
    auto camera = smgr->addCameraSceneNodeFPS(0);
    camera->setPosition({ 0.0f, 0.0f, -kCamDistZ * 3 });
#endif

    int prevFrameIdx = 0;
    int currFrameIdx = 1;
    Frame frames[2];

    auto rtId = driver->addRenderTargetTexture(dimension2du(WINDOW_W, WINDOW_H), "rt", ECF_A8R8G8B8);

    scene::ISceneNode* test = smgr->addCubeSceneNode(60);
    test->setMaterialFlag(video::EMF_LIGHTING, false); // disable dynamic lighting
    test->setMaterialTexture(0, rtId); // set material of cube to render target

    s32 mtrl;
    video::IGPUProgrammingServices* gpu = driver->getGPUProgrammingServices();
#if 1
    {
        MyShaderCallBack* mc = new MyShaderCallBack();

        mtrl = gpu->addHighLevelShaderMaterial(
            R"(
float4x4 WorldViewProj;
struct VS_IN 
{
    float4 Position  : POSITION;
    float2 TexCoord1 : TEXCOORD1;
};

struct VS_OUT 
{
    float4 Position  : POSITION;
    float2 TexCoord1 : TEXCOORD1;
};

VS_OUT main(VS_IN IN)
{
    VS_OUT OUT;
    OUT.Position = mul(IN.Position, WorldViewProj);
    OUT.TexCoord1 = IN.TexCoord1;
    return OUT;
}

)",
"main",
EVST_VS_3_0,
R"(
struct VS_OUT 
{
    float4 Position  : POSITION;
    float2 TexCoord1 : TEXCOORD1;
};

float4 main(VS_OUT IN) : COLOR
{
    return float4(IN.TexCoord1.xy, 1, 0);
}
)",
"main",
EPST_PS_3_0);

        CD3D9HLSLMaterialRenderer* renderer = (CD3D9HLSLMaterialRenderer*)driver->getMaterialRenderer(mtrl);

        mc->drop();
    }
#endif

    int frameCount = 0;
    while (frameCount < 1000 && device->run())
    {
        driver->beginScene(true, true, SColor(255, 100, 101, 140));

        auto& prevFrame = frames[prevFrameIdx];
        auto& currFrame = frames[currFrameIdx];

#if 0
        driver->setRenderTarget(rtId);
        test->setVisible(false);
        smgr->drawAll();
        driver->setRenderTarget(0);
        test->setVisible(true);
        smgr->drawAll();
#else
        test->setVisible(false);
        smgr->drawAll();
#endif

        currFrame.update();

        if (false)
        {
            currFrame.debugDraw();
            Frame::drawDiff(prevFrame, currFrame);
        }

        c8 filename[256];

        snprintf(filename, 256, "flow\\dummy\\%s_%04d.flo", "frame", frameCount);
        Frame::writeFlowImage(prevFrame, currFrame, filename);

        driver->endScene();

        video::IImage* backBuffer = driver->createScreenShot();
        if (backBuffer)
        {
            snprintf(filename, 256, "images\\dummy\\%s_%04d.bmp", "frame", frameCount);
            driver->writeImageToFile(backBuffer, filename, 85);
            backBuffer->drop();
        }

        swap(currFrameIdx, prevFrameIdx);

        frameCount++;

        //device->sleep(100);
    }

    device->drop();

    return 0;
}

/*
That's it. Compile and run.
**/
