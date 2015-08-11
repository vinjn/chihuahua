#include "ExampleSceneNodePlugin.h"

// this example shows how to add own scene nodes into irrEdit.
// You only need two things:
// - a scene node factory 
// - your scene node.
// you can add as many scene nodes with a plugin as you wish.


// entry point for the plugin, will be called by the editor. Return a
// plugin interface if your plugin should run as plugin inside the editor, otherwise
// just modify the engine (add factories etc) and return 0.
__declspec(dllexport) irr::irredit::IIrrEditPlugin* __stdcall createPlugin( irr::irredit::IrrEditServices* services )
{
	// we simple register a new scene node factory here, which is only able to
	// create our example scene node
	
	scene::ISceneManager* mgr = services->getDevice()->getSceneManager();

	CExampleSceneNodeFactory* factory = new CExampleSceneNodeFactory(mgr);
	mgr->registerSceneNodeFactory(factory);
	factory->drop();

	return 0;
}



// -------------------------------------------------------------------------------
// example factory implementation
// -------------------------------------------------------------------------------


// id for our scene node, 'xmpl', short for 'example'.
const int EXAMPLE_SCENE_NODE_ID = MAKE_IRR_ID('x','m','p','l');

// type name for our scene node
const char* exampleSceneNodeTypeName = "examplePluginSceneNode";



CExampleSceneNodeFactory::CExampleSceneNodeFactory(ISceneManager* mgr)
: Manager(mgr)
{
	// don't grab the manager here, to avoid cyclic references
}


CExampleSceneNodeFactory::~CExampleSceneNodeFactory()
{
}


//! adds a scene node to the scene graph based on its type id
ISceneNode* CExampleSceneNodeFactory::addSceneNode(ESCENE_NODE_TYPE type, ISceneNode* parent)
{
	if (!parent)
		parent = Manager->getRootSceneNode();

	if (type == EXAMPLE_SCENE_NODE_ID)
	{
		CSampleSceneNode* node = new CSampleSceneNode(parent, Manager, -1);
		node->drop();
		return node; 
	}

	return 0;
}


//! adds a scene node to the scene graph based on its type name
ISceneNode* CExampleSceneNodeFactory::addSceneNode(const c8* typeName, ISceneNode* parent)
{
	return addSceneNode( getTypeFromName(typeName), parent );
}


//! returns amount of scene node types this factory is able to create
u32 CExampleSceneNodeFactory::getCreatableSceneNodeTypeCount() const
{
	return 1;
}


//! returns type of a createable scene node type
ESCENE_NODE_TYPE CExampleSceneNodeFactory::getCreateableSceneNodeType(u32 idx) const
{
	if (idx==0)
		return (ESCENE_NODE_TYPE)EXAMPLE_SCENE_NODE_ID;

	return ESNT_UNKNOWN;
}


//! returns type name of a createable scene node type 
const c8* CExampleSceneNodeFactory::getCreateableSceneNodeTypeName(u32 idx) const
{
	if (idx==0)
		return exampleSceneNodeTypeName;

	return 0;
}


//! returns type name of a createable scene node type 
const c8* CExampleSceneNodeFactory::getCreateableSceneNodeTypeName(ESCENE_NODE_TYPE type) const
{
	if (type == EXAMPLE_SCENE_NODE_ID)
		return exampleSceneNodeTypeName;

	return 0;
}


ESCENE_NODE_TYPE CExampleSceneNodeFactory::getTypeFromName(const c8* name)
{
	if (!strcmp(name, exampleSceneNodeTypeName))
		return (ESCENE_NODE_TYPE)EXAMPLE_SCENE_NODE_ID;

	return ESNT_UNKNOWN;
}



// -------------------------------------------------------------------------------
// example scene node implementation which will be added into the editor
// -------------------------------------------------------------------------------


CSampleSceneNode::CSampleSceneNode(scene::ISceneNode* parent, scene::ISceneManager* mgr, s32 id)
	: scene::ISceneNode(parent, mgr, id)
{
	Material.Wireframe = false;
	Material.Lighting = false;

	Vertices[0] = video::S3DVertex(0,0,10, 1,1,0, video::SColor(255,0,255,255), 0, 1);
	Vertices[1] = video::S3DVertex(10,0,-10, 1,0,0, video::SColor(255,255,0,255), 1, 1);
	Vertices[2] = video::S3DVertex(0,20,0, 0,1,1, video::SColor(255,255,255,0), 1, 0);
	Vertices[3] = video::S3DVertex(-10,0,-10, 0,0,1, video::SColor(255,0,255,0), 0, 0);

	Box.reset(Vertices[0].Pos);
	for (s32 i=1; i<4; ++i)
		Box.addInternalPoint(Vertices[i].Pos);
}


void CSampleSceneNode::OnRegisterSceneNode()
{
	if (IsVisible)
		SceneManager->registerNodeForRendering(this);

	ISceneNode::OnRegisterSceneNode();
}


void CSampleSceneNode::render()
{
	u16 indices[] = {	0,2,3, 2,1,3, 1,0,3, 2,0,1	};
	video::IVideoDriver* driver = SceneManager->getVideoDriver();

	driver->setMaterial(Material);
	driver->setTransform(video::ETS_WORLD, AbsoluteTransformation);
	driver->drawIndexedTriangleList(&Vertices[0], 4, &indices[0], 4);
}


const core::aabbox3d<f32>& CSampleSceneNode::getBoundingBox() const
{
	return Box;
}


u32 CSampleSceneNode::getMaterialCount()
{
	return 1;
}


video::SMaterial& CSampleSceneNode::getMaterial(u32 i)
{
	return Material;
}	



//! Creates a clone of this scene node and its children.
ISceneNode* CSampleSceneNode::clone(ISceneNode* newParent, ISceneManager* newManager)
{
	// this method is only implemented to let irrEdit be able to copy the
	// scene node via CTRL+C, it's not necessary

	if (!newParent) newParent = Parent;
	if (!newManager) newManager = SceneManager;

	CSampleSceneNode* nb = new CSampleSceneNode(newParent, 
		newManager, ID);

	nb->cloneMembers(this, newManager);

	nb->drop();
	return nb;
}


//! Returns type of the scene node
ESCENE_NODE_TYPE CSampleSceneNode::getType() const
{
	return (ESCENE_NODE_TYPE)EXAMPLE_SCENE_NODE_ID;
}
