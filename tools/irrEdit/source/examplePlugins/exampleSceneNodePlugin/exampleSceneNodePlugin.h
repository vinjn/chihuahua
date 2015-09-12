#ifndef __EXAMPLE_SCENE_NODE_PLUGIN_H_INCLUDED__
#define __EXAMPLE_SCENE_NODE_PLUGIN_H_INCLUDED__

#include <irrlicht.h>
#include <irredit.h>

using namespace irr;
using namespace scene;


// own factory implementation
class CExampleSceneNodeFactory : public ISceneNodeFactory
{
public:

	CExampleSceneNodeFactory(ISceneManager* mgr);
	~CExampleSceneNodeFactory();

	virtual ISceneNode* addSceneNode(ESCENE_NODE_TYPE type, ISceneNode* parent=0);
	virtual ISceneNode* addSceneNode(const c8* typeName, ISceneNode* parent=0);
	virtual u32 getCreatableSceneNodeTypeCount() const;
	virtual const c8* getCreateableSceneNodeTypeName(u32 idx) const;
	virtual ESCENE_NODE_TYPE getCreateableSceneNodeType(u32 idx) const;
	virtual const c8* getCreateableSceneNodeTypeName(ESCENE_NODE_TYPE type) const;

private:

	ESCENE_NODE_TYPE getTypeFromName(const c8* name);
	ISceneManager* Manager;
};




// own scene node implementation
class CSampleSceneNode : public scene::ISceneNode
{
	core::aabbox3d<f32> Box;
	video::S3DVertex Vertices[4];
	video::SMaterial Material;

public:

	CSampleSceneNode(scene::ISceneNode* parent, scene::ISceneManager* mgr, s32 id);

	virtual void OnRegisterSceneNode();
	virtual void render();
	virtual const core::aabbox3d<f32>& getBoundingBox() const;
	virtual u32 getMaterialCount();
	virtual video::SMaterial& getMaterial(u32 i);
	virtual ESCENE_NODE_TYPE getType() const;
	ISceneNode* clone(ISceneNode* newParent, ISceneManager* newManager);
};


#endif