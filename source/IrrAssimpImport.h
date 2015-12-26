#ifndef IRRASSIMPIMPORT_H
#define IRRASSIMPIMPORT_H


#include "irrlicht.h"

#include "assimp/scene.h"          // Output data structure
#include "assimp/postprocess.h"    // Post processing flags
#include "assimp/Importer.hpp"

class SkinnedVertex
{
public:
    SkinnedVertex()
    {
        Moved = false;
        Position = ue::core::vector3df(0, 0, 0);
        Normal = ue::core::vector3df(0, 0, 0);
    }

    bool Moved;
    ue::core::vector3df Position;
    ue::core::vector3df Normal;
};

class IrrAssimpImport : public ue::scene::IMeshLoader
{
    public:
        IrrAssimpImport(ue::scene::ISceneManager* smgr);
        virtual ~IrrAssimpImport();

        virtual ue::scene::IAnimatedMesh* createMesh(ue::io::IReadFile* file);
        virtual bool isALoadableFileExtension(const ue::io::path& filename) const;

    protected:
    private:
        void createNode(ue::scene::ISkinnedMesh* mesh, aiNode* node, bool isRoot);
        ue::scene::ISkinnedMesh::SJoint* findJoint (ue::scene::ISkinnedMesh* mesh, ue::core::stringc jointName);
        aiNode* findNode (const aiScene* scene, aiString jointName);
        ue::video::ITexture* getTexture(ue::core::stringc path, ue::core::stringc fileDir);

        ue::core::array<ue::video::SMaterial> Mats;

        ue::scene::ISceneManager* Smgr;
        ue::io::IFileSystem* FileSystem;

        ue::core::matrix4 InverseRootNodeWorldTransform;

        ue::core::array<SkinnedVertex> skinnedVertex;

        void skinJoint(ue::scene::ISkinnedMesh* mesh, ue::scene::ISkinnedMesh::SJoint *joint, aiBone* bone);
        void buildSkinnedVertexArray(ue::scene::IMeshBuffer* buffer);
        void applySkinnedVertexArray(ue::scene::IMeshBuffer* buffer);
};

#endif // IRRASSIMPIMPORT_H
