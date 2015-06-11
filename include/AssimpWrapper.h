#ifndef ASSIMP_WRAPPER_H
#define ASSIMP_WRAPPER_H

#include <irrlicht.h>

IRRLICHT_API irr::scene::IAnimatedMesh* IRRCALLCONV getMeshFromAssimp(irr::scene::ISceneManager* smgr, const irr::io::path& path);

#ifdef __ANDROID__
IRRLICHT_API void IRRCALLCONV setAssimpJNIAssetPath(const irr::io::path& internalDataPath, struct AAssetManager* assetManager);
#endif

#endif // ASSIMP_WRAPPER_H
