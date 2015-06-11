#include "AssimpWrapper.h"
#include "IrrAssimpImport.h"
#include "../Irrlicht/os.h"

#include <iostream>

using namespace irr;

irr::scene::IAnimatedMesh* getMesh(irr::scene::ISceneManager* smgr, IrrAssimpImport& importer, const io::path& path)
{
    scene::IAnimatedMesh* msh = smgr->getMeshCache()->getMeshByName(path);
    if (msh)
        return msh;

    os::Printer::log("Loading mesh from Assimp", path, ELL_INFORMATION);
    io::IReadFile* file = smgr->getFileSystem()->createAndOpenFile(path);
    if (!file)
    {
        os::Printer::log("Could not load mesh, because file could not be opened", path, ELL_ERROR);
        return 0;
    }

    msh = importer.loadMesh(path);

    if (msh)
    {
        s32 animCount = msh->getAnimationCount();
        for (s32 i = 0; i < animCount; i++)
        {
            os::Printer::log("----animation", msh->getAnimationName(i), ELL_INFORMATION);
        }
        smgr->getMeshCache()->addMesh(path, msh);
        msh->drop();
    }
    else
    {
        os::Printer::log("Could not load mesh", path, ELL_ERROR);
    }

    file->drop();

    /*
    if (!msh)
    os::Printer::log("Could not load mesh, file format seems to be unsupported", filename, ELL_ERROR);
    else
    os::Printer::log("Loaded mesh", filename, ELL_INFORMATION);
    */

    return msh;
}

bool isLoadable(irr::core::stringc path)
{
    Assimp::Importer importer;

    irr::core::stringc extension;
    irr::core::getFileNameExtension(extension, path);
    return importer.IsExtensionSupported(extension.c_str());
}

#ifdef __ANDROID__
IRRLICHT_API void IRRCALLCONV setAssimpJNIAssetPath(const irr::io::path& path, struct AAssetManager* assetManager)
{
    IrrAssimpImport::sInternalDataPath = path;
    IrrAssimpImport::sAssetManager = assetManager;
}
#endif

IRRLICHT_API irr::scene::IAnimatedMesh* IRRCALLCONV getMeshFromAssimp(irr::scene::ISceneManager* smgr, const irr::io::path& path)
{
    static IrrAssimpImport importer(smgr);
    if (isLoadable(path))
    {
        return getMesh(smgr, importer, path);
    }
    else
    {
        return smgr->getMesh(path);
    }
}
