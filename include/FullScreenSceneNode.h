#ifndef FullScreenSceneNode_H
#define FullScreenSceneNode_H

#include <irrlicht.h>

namespace irr {

struct FullScreenSceneNode : public scene::ISceneNode
{
    core::aabbox3d<f32> Box;
    video::S3DVertex Vertices[4];
    video::SMaterial Material;

    FullScreenSceneNode(scene::ISceneNode* parent, scene::ISceneManager* mgr, s32 id, int rotation)
        : scene::ISceneNode(parent, mgr, id)
    {
            // Material.ZBuffer = video::ECFN_DISABLED;
            Material.ZWriteEnable = false;
            Material.Wireframe = false;
            Material.Lighting = true;
            Material.Thickness = 0.f;

            f32 k = 1;
            core::vector3df positions[] =
            {
                core::vector3df(-k, k, 1),
                core::vector3df(k, k, 1),
                core::vector3df(k, -k, 1),
                core::vector3df(-k, -k, 1)
            };

            core::vector2df texCoords[] =
            {
                core::vector2df(0, 0),
                core::vector2df(1, 0),
                core::vector2df(1, 1),
                core::vector2df(0, 1)
            };
            //if (rotation < 0)
            //{
            //    core::swap(texCoords[0], texCoords[1]);
            //    core::swap(texCoords[2], texCoords[3]);
            //}

            for (int i = 0; i < 4; i++)
            {
                Vertices[i].Pos = positions[i];
                Vertices[i].TCoords = texCoords[(i + rotation) % 4];
                Vertices[i].Color = video::SColor(255, 255, 255, 255);
                Vertices[i].Normal = core::vector3df(0, 0, 1);
            }
            /*
            The Irrlicht Engine needs to know the bounding box of a scene node.
            It will use it for automatic culling and other things. Hence, we
            need to create a bounding box from the 4 vertices we use.
            If you do not want the engine to use the box for automatic culling,
            and/or don't want to create the box, you could also call
            irr::scene::ISceneNode::setAutomaticCulling() with irr::scene::EAC_OFF.
            */
            Box.reset(Vertices[0].Pos);
            for (s32 i = 1; i < 4; ++i)
                Box.addInternalPoint(Vertices[i].Pos);
        }

    /*
    Before it is drawn, the irr::scene::ISceneNode::OnRegisterSceneNode()
    method of every scene node in the scene is called by the scene manager.
    If the scene node wishes to draw itself, it may register itself in the
    scene manager to be drawn. This is necessary to tell the scene manager
    when it should call irr::scene::ISceneNode::render(). For
    example, normal scene nodes render their content one after another,
    while stencil buffer shadows would like to be drawn after all other
    scene nodes. And camera or light scene nodes need to be rendered before
    all other scene nodes (if at all). So here we simply register the
    scene node to render normally. If we would like to let it be rendered
    like cameras or light, we would have to call
    SceneManager->registerNodeForRendering(this, SNRT_LIGHT_AND_CAMERA);
    After this, we call the actual
    irr::scene::ISceneNode::OnRegisterSceneNode() method of the base class,
    which simply lets also all the child scene nodes of this node register
    themselves.
    */
    virtual void OnRegisterSceneNode()
    {
        if (IsVisible)
            SceneManager->registerNodeForRendering(this, scene::ESNRP_SKY_BOX);

        ISceneNode::OnRegisterSceneNode();
    }

    /*
    In the render() method most of the interesting stuff happens: The
    Scene node renders itself. We override this method and draw the
    tetraeder.
    */
    virtual void render()
    {
        /* Indices into the 'Vertices' array. A triangle needs 3 vertices
        so you have to pass the 3 corresponding indices for each triangle to
        tell which of the vertices should be used for it.	*/
        u16 indices[] = { 0, 1, 2, 0, 2, 3 };
        video::IVideoDriver* driver = SceneManager->getVideoDriver();

        core::matrix4 prevView = driver->getTransform(video::ETS_VIEW);
        core::matrix4 prevProj = driver->getTransform(video::ETS_PROJECTION);
#if 1
        driver->setTransform(video::ETS_VIEW, core::matrix4::EM4CONST_IDENTITY);
        driver->setTransform(video::ETS_PROJECTION, core::matrix4::EM4CONST_IDENTITY);
        driver->setTransform(video::ETS_WORLD, core::matrix4::EM4CONST_IDENTITY);
#endif
        {
            driver->setMaterial(Material);
            driver->drawVertexPrimitiveList(&Vertices[0], 4, &indices[0], 2, video::EVT_STANDARD, scene::EPT_TRIANGLES, video::EIT_16BIT);
        }
        driver->setTransform(video::ETS_VIEW, prevView);
        driver->setTransform(video::ETS_PROJECTION, prevProj);
    }

    /*
    And finally we create three small additional methods.
    irr::scene::ISceneNode::getBoundingBox() returns the bounding box of
    this scene node, irr::scene::ISceneNode::getMaterialCount() returns the
    amount of materials in this scene node (our tetraeder only has one
    material), and irr::scene::ISceneNode::getMaterial() returns the
    material at an index. Because we have only one material here, we can
    return the only one material, assuming that no one ever calls
    getMaterial() with an index greater than 0.
    */
    virtual const core::aabbox3d<f32>& getBoundingBox() const
    {
        return Box;
    }

    virtual u32 getMaterialCount() const
    {
        return 1;
    }

    virtual video::SMaterial& getMaterial(u32 i)
    {
        return Material;
    }
};

}

#endif // FullScreenSceneNode_H