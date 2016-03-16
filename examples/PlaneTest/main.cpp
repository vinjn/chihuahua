#include <irrlicht.h>
#include "bx/commandline.h"
#include "bx/float4x4_t.h"
#include "bx/readerwriter.h"
#include "bgfx/c99/bgfx.h"

using namespace ue;

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

IrrlichtDevice *device;

static const bgfx_memory* loadMem(bx::FileReaderI* _reader, const char* _filePath)
{
    if (0 == bx::open(_reader, _filePath))
    {
        uint32_t size = (uint32_t)bx::getSize(_reader);
        const bgfx_memory* mem;// = bgfx::alloc(size + 1);
        //bx::read(_reader, mem->data, size);
        //bx::close(_reader);
        //mem->data[mem->size - 1] = '\0';
        return mem;
    }

    return NULL;
}

int main(int argc, char const* const* argv)
{
    bx::CommandLine cmdLine(argc, argv);

    device = createDevice(video::EDT_BGFX_D3D11, dimension2d<u32>(800, 600), 16,
        false, false, false, 0);

    if (!device)
        return 1;

    device->setWindowCaption(L"PlaneTest");

    IVideoDriver* driver = device->getVideoDriver();
    ISceneManager* smgr = device->getSceneManager();
    IGPUProgrammingServices* gpu = driver->getGPUProgrammingServices();
    auto fs = device->getFileSystem();

    //auto s_fileReader = new bx::CrtFileReader;

    //fs->addFileArchive("../media");

    struct MyShaderCallBack : public video::IShaderConstantSetCallBack
    {
        const int m_numLights = 4;

        MyShaderCallBack()
        {
            // Create texture sampler uniforms.
            s_texColor = bgfx_create_uniform("s_texColor", BGFX_UNIFORM_TYPE_INT1, 1);
            s_texNormal = bgfx_create_uniform("s_texNormal", BGFX_UNIFORM_TYPE_INT1, 1);

            u_lightPosRadius = bgfx_create_uniform("u_lightPosRadius", BGFX_UNIFORM_TYPE_VEC4, m_numLights);
            u_lightRgbInnerR = bgfx_create_uniform("u_lightRgbInnerR", BGFX_UNIFORM_TYPE_VEC4, m_numLights);
        }

        void OnSetMaterial(const SMaterial& material)
        {
            bgfx_set_texture(0, s_texColor, { material.getTexture(0)->getNativeHandle() }, 0);
            bgfx_set_texture(1, s_texNormal, { material.getTexture(1)->getNativeHandle() }, 0);

            float lightPosRadius[4][4];
            for (uint32_t ii = 0; ii < m_numLights; ++ii)
            {
                lightPosRadius[ii][0] = sinf((device->getTimer()->getTime() * 0.001f *(0.1f + ii*0.17f) + ii*HALF_PI*1.37f))*100.0f;
                lightPosRadius[ii][1] = cosf((device->getTimer()->getTime()* 0.001f*(0.2f + ii*0.29f) + ii*HALF_PI*1.49f))*100.0f;
                lightPosRadius[ii][2] = -2.5f;
                lightPosRadius[ii][3] = 100.0f;
            }

            bgfx_set_uniform(u_lightPosRadius, lightPosRadius, m_numLights);

            float lightRgbInnerR[4][4] =
            {
                { 1.0f, 0.7f, 0.2f, 0.8f },
                { 0.7f, 0.2f, 1.0f, 0.8f },
                { 0.2f, 1.0f, 0.7f, 0.8f },
                { 1.0f, 0.4f, 0.2f, 0.8f },
            };

            bgfx_set_uniform(u_lightRgbInnerR, lightRgbInnerR, m_numLights);
        }

        bgfx_uniform_handle s_texColor, s_texNormal;
        bgfx_uniform_handle u_lightPosRadius, u_lightRgbInnerR;
    };

    const float kCamDistZ = 40;

    // TODO: implement assets system
    s32 mtrlId = gpu->addHighLevelShaderMaterialFromFiles(
        "../media/bgfx-shaders/dx11/vs_bump.bin", "main", EVST_VS_1_1,
        "../media/bgfx-shaders/dx11/fs_bump.bin", "main");
    scene::IMesh* mesh;
#if 0
    mesh = smgr->getGeometryCreator()->createCubeMesh({ 100, 100, 100 });
    mesh->drop();
#else
    mesh = smgr->getMesh("../media/duck/duck.fbx");
#endif
    scene::IMesh* tangentMesh = smgr->getMeshManipulator()->
        createMeshWithTangents(mesh);

    tangentMesh->setHardwareMappingHint(EHM_STATIC);
    scene::IMeshSceneNode* node = smgr->addMeshSceneNode(tangentMesh);
    tangentMesh->drop();

    node->setRotation({ -90, 0, 0 });
    //node->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false);
    //node->setMaterialFlag(video::EMF_FRONT_FACE_CULLING, true);
    node->setMaterialTexture(0, driver->getTexture("../media/duck.png"));
    node->setMaterialTexture(1, driver->getTexture("../media/fieldstone-n.tga"));
    node->setMaterialType((video::E_MATERIAL_TYPE)mtrlId);
    node->setMaterialShaderCallback(new MyShaderCallBack());
    node->setMaterialFlag(EMF_BACK_FACE_CULLING, true);
    node->setMaterialFlag(EMF_FRONT_FACE_CULLING, false);

#if 0
    smgr->addCameraSceneNode(0, vector3df(0, 0, -kCamDistZ * 3), vector3df(0, 0, 0));
#else
    auto camera = smgr->addCameraSceneNodeFPS(0);
    camera->setPosition({ 0.0f, 0.0f, -kCamDistZ * 3 });
#endif

    while (device->run())
    {
        driver->beginScene(true, true, SColor(255, 100, 101, 140));

        smgr->drawAll();
        //guienv->drawAll();

        driver->endScene();
    }

    device->drop();

    return 0;
}
