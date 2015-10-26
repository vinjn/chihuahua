/*
Class for rendering the scene as SBS image to RTT and then performing Oculus Rift's distortion on the image.

Copyright (c) 2014 Suvi Aalto
http://www.coolhouse.fi

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The origin of this software must not be misrepresented; you must not claim
that you wrote the original software.

If you use this software in a product, an acknowledgment in the product
documentation would be appreciated.

This notice may not be removed or altered from any source distribution.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
#ifndef __OCULUSRENDERER_H_INCLUDED__
#define __OCULUSRENDERER_H_INCLUDED__
#include <irrlicht.h>

#include "OVR.h"
using namespace OVR;

class OculusRenderer
{
public:
	/// Pass the window's handle to the renderer. In Windows the handle is supposedly HWND
	/// worldScale is used with positional tracking to multiply the values from the tracker.
	OculusRenderer(void *window, irr::video::IVideoDriver *driver, irr::scene::ISceneManager *smgr, float worldScale);
	~OculusRenderer(void);

	/// Draw the scene using player's position and rotation around Y-axis. Other rotations are read from Oculus Rift.
	/// headPosition and playerYRotation will be ignored if linkHeadNode() is used.
	/// Use this function instead of SceneManager->drawAll() to get Oculus Rift distortion.
	/// Doesn't draw GUI. If you want to draw GUI then you should for example pre-render it to a billboard.
	void drawAll(irr::core::vector3df headPosition, float playerYRotation, irr::video::SColor bgColor);

	/// Use this if you want to link the head position to some node instead of specifying the position every frame
	/// This will also apply rotations so may work for space flight, rollercoasters etc
	void linkHeadNode(irr::scene::ISceneNode *headNode);
	/// Get linked head node
	irr::scene::ISceneNode *getHeadNode() const;

private:
	ovrHmd hmd_;
	ovrEyeRenderDesc eyeRenderDesc_[2];
	ovrVector2f uvScaleOffset_[2][2];
	ovrRecti eyeRenderViewport_[2];
	ovrPosef eyePose_[2];

	irr::video::IVideoDriver *driver_;
	irr::scene::ISceneManager *smgr_;

	irr::video::ITexture *renderTexture_;

	// Eye distortion meshes
	irr::core::array<irr::video::S3DVertex2TCoords> EyeVB[2];
	irr::core::array<unsigned short> EyeIB[2];
	irr::video::SMaterial renderMaterial_;


	irr::core::matrix4 eyeProjection_[2];
	irr::core::vector3df eyeDist_[2];

	irr::scene::ISceneNode *bodyRotationY_; // The rotation of our body
	irr::scene::ISceneNode *headRotationY_; // Head's rotation around Y-axis from the rift - also contains the tracked head position
	irr::scene::ISceneNode *headRotationX_; // Head's rotation around X-axis from the rift
	irr::scene::ISceneNode *headRotationZ_; // Head's rotation around Z-axis from the rift

	irr::scene::ISceneNode *linkedHead_; // linked head node

	irr::scene::ISceneNode *eye_[2]; // These hold the eye positions

	irr::scene::ICameraSceneNode * camera_;

	float worldScale_;


	/// Shader callback for the Oculus Rift distortion shader
	class OculusDistorsionCallback: public irr::video::IShaderConstantSetCallBack
	{
	public:
		irr::f32 EyeToSourceUVScale[2];
		irr::f32 EyeToSourceUVOffset[2];
		ovrMatrix4f timeWarpMatrices[2];

		virtual void OnSetConstants(irr::video::IMaterialRendererServices* services, irr::s32 userData)
		{
			irr::video::IVideoDriver* driver = services->getVideoDriver();

			services->setVertexShaderConstant("EyeToSourceUVScale", EyeToSourceUVScale, 2);
			services->setVertexShaderConstant("EyeToSourceUVOffset", EyeToSourceUVOffset, 2);

			services->setVertexShaderConstant("EyeRotationStart", (float*)(&timeWarpMatrices[0]), 16);
			services->setVertexShaderConstant("EyeRotationEnd", (float*)(&timeWarpMatrices[1]), 16);


		}
	};
	OculusDistorsionCallback distortionCB_;

};

#endif