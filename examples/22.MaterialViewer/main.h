#ifndef EXAMPLE22_MATERIAL_VIEWER_MAIN_H
#define EXAMPLE22_MATERIAL_VIEWER_MAIN_H

#include <irrlicht.h>

// Helper control to allow setting colors
class CColorControl : public ue::gui::IGUIElement
{
public:
	CColorControl(ue::gui::IGUIEnvironment* guiEnv, const ue::core::position2d<ue::s32> & pos, const wchar_t *text, ue::gui::IGUIElement* parent, ue::s32 id=-1);

	// Event receiver
	virtual bool OnEvent(const ue::SEvent &event);

	// Set the color values
	void setColor(const ue::video::SColor& col);

	// Get the color values
	const ue::video::SColor& getColor() const
	{
		return Color;
	}

	// To reset the dirty flag
	void resetDirty()
	{
		DirtyFlag = false;
	}

	// when the color was changed the dirty flag is set
	bool isDirty() const
	{
		return DirtyFlag;
	};

protected:

	// Add a staticbox for a description + an editbox so users can enter numbers
	ue::gui::IGUIEditBox* addEditForNumbers(ue::gui::IGUIEnvironment* guiEnv, const ue::core::position2d<ue::s32> & pos, const wchar_t *text, ue::s32 id, ue::gui::IGUIElement * parent);

	// Get the color value from the editfields
	ue::video::SColor getColorFromEdits() const;

	// Fill the editfields with the value for the given color
	void setEditsFromColor(ue::video::SColor col);

private:

	bool DirtyFlag;
	ue::video::SColor Color;
	ue::s32 ButtonSetId;
	ue::gui::IGUIStaticText * ColorStatic;
	ue::gui::IGUIEditBox * EditAlpha;
	ue::gui::IGUIEditBox * EditRed;
	ue::gui::IGUIEditBox * EditGreen;
	ue::gui::IGUIEditBox * EditBlue;
};

/*
	Custom GUI-control for to edit all colors typically used in materials and lights
*/
class CTypicalColorsControl : public ue::gui::IGUIElement
{
public:
	// Constructor
	CTypicalColorsControl(ue::gui::IGUIEnvironment* guiEnv, const ue::core::position2d<ue::s32> & pos, bool hasEmissive, ue::gui::IGUIElement* parent, ue::s32 id=-1);

	// Destructor
	virtual ~CTypicalColorsControl();

	// Set the color values to those within the material
	void setColorsToMaterialColors(const ue::video::SMaterial & material);

	// Update all changed colors in the material
	void updateMaterialColors(ue::video::SMaterial & material) const;

	// Set the color values to those from the light data
	void setColorsToLightDataColors(const ue::video::SLight & lightData);

	// Update all changed colors in the light data
	void updateLightColors(ue::video::SLight & lightData) const;

	// To reset the dirty flags
	void resetDirty();

private:
	CColorControl*	ControlAmbientColor;
	CColorControl*	ControlDiffuseColor;
	CColorControl*	ControlSpecularColor;
	CColorControl*	ControlEmissiveColor;
};

/*
	GUI-Control to offer a selection of available textures.
*/
class CTextureControl : public ue::gui::IGUIElement
{
public:
	CTextureControl(ue::gui::IGUIEnvironment* guiEnv, ue::video::IVideoDriver * driver, const ue::core::position2d<ue::s32> & pos, ue::gui::IGUIElement* parent, ue::s32 id=-1);

	virtual bool OnEvent(const ue::SEvent &event);

	// Workaround for a problem with comboboxes.
	// We have to get in front when the combobox wants to get in front or combobox-list might be drawn below other elements.
	virtual bool bringToFront(ue::gui::IGUIElement* element);

	// Return selected texturename (if any, otherwise 0)
	const wchar_t * getSelectedTextureName() const;

	// Change active selectionbased on the texture name
	void selectTextureByName(const ue::core::stringw& name);

	// Reset the dirty flag
	void resetDirty()
	{
		DirtyFlag = false;
	}

	// When the texture was changed the dirty flag is set
	bool isDirty() const
	{
		return DirtyFlag;
	};

	// Put the names of all currently loaded textures in a combobox
	void updateTextures(ue::video::IVideoDriver * driver);

private:
	bool DirtyFlag;
	ue::gui::IGUIComboBox * ComboTexture;
};

/*
	Control which allows setting some of the material values for a meshscenenode
*/
struct SMaterialControl
{
	// constructor
	SMaterialControl()
	: Initialized(false), Driver(0)
	, TypicalColorsControl(0), ButtonLighting(0), InfoLighting(0), ComboMaterial(0)
	{
		for (ue::u32 i=0; i<ue::video::MATERIAL_MAX_TEXTURES; ++i)
			TextureControls[i] = 0;
	}

	// Destructor
	~SMaterialControl()
	{
		for (ue::u32 i=0; i<ue::video::MATERIAL_MAX_TEXTURES; ++i)
		{
			if (TextureControls[i] )
				TextureControls[i]->drop();
		}
		if ( TypicalColorsControl )
			TypicalColorsControl->drop();
	}

	void init(ue::scene::IMeshSceneNode* node, ue::IrrlichtDevice * device, const ue::core::position2d<ue::s32> & pos, const wchar_t * description);

	void update(ue::scene::IMeshSceneNode* sceneNode, ue::scene::IMeshSceneNode* sceneNode2T, ue::scene::IMeshSceneNode* sceneNodeTangents);

	void updateTextures();

	void selectTextures(const ue::core::stringw& name);

	bool isLightingEnabled() const;

protected:

	void updateMaterial(ue::video::SMaterial & material);

	bool Initialized;
	ue::video::IVideoDriver * 	Driver;
	CTypicalColorsControl* 		TypicalColorsControl;
	ue::gui::IGUIButton * 		ButtonLighting;
	ue::gui::IGUIStaticText* 	InfoLighting;
	ue::gui::IGUIComboBox * 	ComboMaterial;
	CTextureControl*			TextureControls[ue::video::MATERIAL_MAX_TEXTURES];
};

/*
	Control to allow setting the color values of a lightscenenode.
*/
struct SLightNodeControl
{
	// constructor
	SLightNodeControl() : Initialized(false), TypicalColorsControl(0)
	{}

	~SLightNodeControl()
	{
		if ( TypicalColorsControl )
			TypicalColorsControl->drop();
	}

	void init(ue::scene::ILightSceneNode* node, ue::gui::IGUIEnvironment* guiEnv, const ue::core::position2d<ue::s32> & pos, const wchar_t * description);

	void update(ue::scene::ILightSceneNode* node);

protected:
	bool Initialized;
	CTypicalColorsControl* TypicalColorsControl;
};

/*
	Application configuration
*/
struct SConfig
{
	SConfig()
	: RenderInBackground(true)
	, DriverType(ue::video::EDT_NULL)
	, ScreenSize(640, 480)
	{
	}

	bool RenderInBackground;
	ue::video::E_DRIVER_TYPE DriverType;
	ue::core::dimension2d<ue::u32> ScreenSize;
};

/*
	Main application class
*/
class CApp : public ue::IEventReceiver
{
	friend int main(int argc, char *argv[]);

public:
	// constructor
	CApp()
	: IsRunning(false)
	, RealTimeTick(0)
	, Device(0)
	, MeshManipulator(0)
	, Camera(0)
	, SceneNode(0), SceneNode2T(0), SceneNodeTangents(0), NodeLight(0)
	, CameraRotationAxis(ue::core::vector3df(1,0,0))
	, LightRotationAxis(ue::core::vector3df(1,0,0))
	, ControlVertexColors(0)
	, GlobalAmbient(0)
	, MousePressed(false)
	{
		memset(KeysPressed, 0, sizeof KeysPressed);
	}

	// destructor
	~CApp()
	{
	}

	// Tell it to stop running
	void setRunning(bool appRuns)
	{
		IsRunning = appRuns;
	}

	// Check if it should continue running
	bool isRunning() const
	{
		return IsRunning;
	}

	// Event handler
	virtual bool OnEvent(const ue::SEvent &event);

protected:

	// Application initialization
	// returns true when it was successful initialized, otherwise false.
	bool init(int argc, char *argv[]);

	// Update one frame
	bool update();

	// Close down the application
	void quit();

	// Create some useful textures.
	void createDefaultTextures(ue::video::IVideoDriver * driver);

	// Load a texture and make sure nodes know it when more textures are available.
	void loadTexture(const ue::io::path &name);

	// Rotate a node around the origin (0,0,0)
	void RotateHorizontal(ue::scene::ISceneNode* node, ue::f32 angle);
	void RotateAroundAxis(ue::scene::ISceneNode* node, ue::f32 angle, const ue::core::vector3df& axis);
	void ZoomOut(ue::scene::ISceneNode* node, ue::f32 units);
	void UpdateRotationAxis(ue::scene::ISceneNode* node, ue::core::vector3df& axis);


private:
	SConfig	Config;
	bool	IsRunning;
	ue::u32 RealTimeTick;
	ue::IrrlichtDevice * 			Device;
	ue::scene::IMeshManipulator* 	MeshManipulator;
	ue::scene::ICameraSceneNode *	Camera;
	ue::scene::IMeshSceneNode* 	SceneNode;
	ue::scene::IMeshSceneNode* 	SceneNode2T;
	ue::scene::IMeshSceneNode* 	SceneNodeTangents;
	ue::scene::ILightSceneNode* 	NodeLight;
	ue::core::vector3df CameraRotationAxis;
	ue::core::vector3df LightRotationAxis;
	SMaterialControl	MeshMaterialControl;
	SLightNodeControl	LightControl;
	CColorControl*	ControlVertexColors;
	CColorControl*	GlobalAmbient;
	bool KeysPressed[ue::KEY_KEY_CODES_COUNT];
	bool MousePressed;
	ue::core::position2d<ue::s32> MouseStart;
};

#endif
