/*
This script registers a toolbar and menu item which toggles the wireframe mode 
of all scenenodes.
This script is (c) 2006 by Nikolaus Gebhardt.
*/

// add a global variable storing if we should toggle wireframe on or off

WireframeModeOn <- false;


// toggles wirframe mode for one node and its children
function toggleWireframeForNode(node)
{
	local materialCount = irrGetSceneNodeMaterialCount(node);
	
	// set material
	
	for (local i=0; i<materialCount; ++i)
		irrSetSceneNodeMaterialProperty(node, i, "Wireframe", ::WireframeModeOn);
		
	// also do for all children
	
	local childCount = irrGetSceneNodeChildCount(node);
	
	for (local i=0; i<childCount; ++i)
		toggleWireframeForNode(irrGetChildSceneNode(node, i));	
}


// toggles wireframe mode for all nodes
function toggleWireframeForAllSceneNodes()
{
	::WireframeModeOn = !::WireframeModeOn;
		
	toggleWireframeForNode(irrGetRootSceneNode());
	
	editorUpdateAllWindows();
}



// register the new function in the menu and toolbar

editorRegisterMenuEntry("toggleWireframeForAllSceneNodes", 
                        "Toggle wireframe mode");

editorRegisterToolbarEntry("toggleWireframeForAllSceneNodes", 
						   "Toggle wireframe mode",
						   "scripts\\togglewireframe_icon.bmp");
