#ifndef ASSIMP_WRAPPER_H
#define ASSIMP_WRAPPER_H

#include <irrlicht.h>

IRRLICHT_API irr::scene::IAnimatedMesh* IRRCALLCONV getMeshFromAssimp(irr::scene::ISceneManager* smgr, const irr::io::path& path);

#endif // ASSIMP_WRAPPER_H
