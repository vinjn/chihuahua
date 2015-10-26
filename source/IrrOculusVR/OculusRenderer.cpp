#include "OculusRenderer.h"


OculusRenderer::OculusRenderer(void *window, irr::video::IVideoDriver *driver, 
	irr::scene::ISceneManager *smgr, float worldScale) : driver_(driver), 
	smgr_(smgr), linkedHead_(0), worldScale_(worldScale)
{
	// Initialize the rift
	ovr_Initialize();

	hmd_ = ovrHmd_Create(0);
	if (!hmd_)
	{
		printf("Oculus Rift not detected - creating debug HMD\n");
		// Create DK1 debug device if no HMD is detected
		hmd_ = ovrHmd_CreateDebug(ovrHmd_DK1);

	}
	if (hmd_->ProductName[0] == '\0') 
	{
		printf("Oculus Rift detected - display not turned on\n");
	}

	// Attach to window for Direct HMD-mode. Haven't actually been able to test this - crashes my computer
	ovrHmd_AttachToWindow(hmd_, window, NULL, NULL);

	// Get default eye fovs from the Rift
	ovrFovPort eyeFov[2] = { hmd_->DefaultEyeFov[0], hmd_->DefaultEyeFov[1] } ;

	// Get the recommended render target size from the Rift
	Sizei recommenedTex0Size = ovrHmd_GetFovTextureSize(hmd_, ovrEye_Left,  hmd_->DefaultEyeFov[0], 1.0f);
	Sizei recommenedTex1Size = ovrHmd_GetFovTextureSize(hmd_, ovrEye_Right, hmd_->DefaultEyeFov[1], 1.0f);

	Sizei RenderTargetSize;
	RenderTargetSize.w = recommenedTex0Size.w + recommenedTex1Size.w;
	RenderTargetSize.h = irr::core::s32_max ( recommenedTex0Size.h, recommenedTex1Size.h );

	// Create RTT
	renderTexture_ = driver_->addRenderTargetTexture(
		irr::core::dimension2d<irr::u32>(RenderTargetSize.w, RenderTargetSize.h));

	// Set viewport sizes
	eyeRenderViewport_[0].Pos  = Vector2i(0,0);
	eyeRenderViewport_[0].Size = Sizei(RenderTargetSize.w / 2, RenderTargetSize.h);
	eyeRenderViewport_[1].Pos  = Vector2i((RenderTargetSize.w + 1) / 2, 0);
	eyeRenderViewport_[1].Size = eyeRenderViewport_[0].Size;

	// Generate distortion meshes for both eyes
	for ( int eyeNum = 0; eyeNum < 2; eyeNum++ )
	{
        ovrEyeType eye = hmd_->EyeRenderOrder[eyeNum]; // Can this order really change?

		ovrDistortionMesh meshData;
		// Create eye mesh with all the nice effects
		ovrHmd_CreateDistortionMesh(hmd_, (ovrEyeType) eye, eyeFov[eye],
			ovrDistortionCap_TimeWarp | 
			ovrDistortionCap_Vignette, &meshData);

		ovrDistortionVertex *vert = meshData.pVertexData;

		// Convert to irrlicht's format
		for(unsigned int vertNum = 0; vertNum < meshData.VertexCount;  vertNum++)
		{
			irr::video::S3DVertex2TCoords v;
			v.Pos.X = vert->ScreenPosNDC.x;
			v.Pos.Y = vert->ScreenPosNDC.y;

			// Map timewarp and vignette to color
			v.Color = irr::video::SColor((int)(vert->TimeWarpFactor*255.00f),
				(int)(vert->VignetteFactor *255.00f), 0, 0);

			// Map the stuff used by chromatic aberration(?)
			v.TCoords.X = vert->TanEyeAnglesR.x;
			v.TCoords.Y = vert->TanEyeAnglesR.y;
			v.TCoords2.X = vert->TanEyeAnglesB.x;
			v.TCoords2.Y = vert->TanEyeAnglesB.y;
			v.Normal.X = vert->TanEyeAnglesG.x;
			v.Normal.Y = vert->TanEyeAnglesG.y;

			EyeVB[eye].push_back(v); // Store the vertex
			vert++;
		}

		// Create indices
		for(unsigned int iNum = 0; iNum < meshData.IndexCount;  iNum++)
		{
			EyeIB[eye].push_back(meshData.pIndexData[iNum]); // Store the index
		}

		// We no longer need the mesh
		ovrHmd_DestroyDistortionMesh( &meshData );

		// Create eye render description for later use
		eyeRenderDesc_[eye] = ovrHmd_GetRenderDesc(hmd_, (ovrEyeType) eye,  eyeFov[eye]);
			
		// Get Scale and Offset values from the SDK
		ovrHmd_GetRenderScaleAndOffset(eyeFov[eye], RenderTargetSize, eyeRenderViewport_[eye], 
			uvScaleOffset_[eye]);

		// Generate eye projections
		Matrix4f proj = ovrMatrix4f_Projection(eyeRenderDesc_[eye].Fov, 0.1f, 10000.0f, false);

		// Convert to Irrlicht's projection matrix
		eyeProjection_[eye][0] = proj.M[0][0];
		eyeProjection_[eye][5] = proj.M[1][1];
		eyeProjection_[eye][10] = proj.M[2][2];
		eyeProjection_[eye][14] = proj.M[2][3];
		eyeProjection_[eye][11] = proj.M[3][2];
		eyeProjection_[eye][8] = proj.M[0][2];
		eyeProjection_[eye][9] = proj.M[1][2];

		// IPD
		// The IPD should also be affected by the scale of the world
		// Should y and z be multiplied with negative worldScale? SDK likely returns 0 for both currently
		eyeDist_[eye] = irr::core::vector3df(eyeRenderDesc_[eye].HmdToEyeViewOffset.x * -worldScale,
			eyeRenderDesc_[eye].HmdToEyeViewOffset.y * worldScale, eyeRenderDesc_[eye].HmdToEyeViewOffset.z * worldScale);
	}

	// Init low persistence & prediction
	ovrHmd_SetEnabledCaps(hmd_, ovrHmdCap_LowPersistence | ovrHmdCap_DynamicPrediction);

	// Init tracking
	ovrHmd_ConfigureTracking(hmd_, ovrTrackingCap_Orientation |
		ovrTrackingCap_MagYawCorrection |
		ovrTrackingCap_Position, 0);

	// Generate material for distortion meshes
	renderMaterial_.Wireframe = false;
	renderMaterial_.Lighting = false;
	renderMaterial_.TextureLayer[0].TrilinearFilter = true;
	//renderMaterial_.TextureLayer[0].AnisotropicFilter = 16;

	irr::video::IGPUProgrammingServices* gpu = driver_->getGPUProgrammingServices();

	// TODO: include the shaders inside source rather than as external files
	if(driver_->getDriverType() == irr::video::EDT_DIRECT3D9)
	{
		renderMaterial_.MaterialType=
			(irr::video::E_MATERIAL_TYPE)gpu->addHighLevelShaderMaterialFromFiles(
				"media/oculus.hlsl","vs_main", 
				irr::video::EVST_VS_2_0,
				"media/oculus.hlsl","main", irr::video::EPST_PS_2_0, 
				&distortionCB_, irr::video::EMT_SOLID, 0);
	}
	else if(driver_->getDriverType() == irr::video::EDT_OPENGL)
	{
		renderMaterial_.MaterialType=
			(irr::video::E_MATERIAL_TYPE)gpu->addHighLevelShaderMaterialFromFiles(
				"media/oculus_vs.glsl","main", 
				irr::video::EVST_VS_2_0,
				"media/oculus_fs.glsl","main", irr::video::EPST_PS_2_a, 
				&distortionCB_, irr::video::EMT_SOLID, 0);
	}

	// Generate nodes for the head rotations - feel free to replace with math
	bodyRotationY_ = smgr_->addEmptySceneNode(0);
	headRotationY_ = smgr_->addEmptySceneNode(bodyRotationY_);
	headRotationX_ = smgr_->addEmptySceneNode(headRotationY_);
	headRotationZ_ = smgr_->addEmptySceneNode(headRotationX_);


	// Place eyes
	for(int i=0;i<2;i++)
	{
		ovrEyeType eye = hmd_->EyeRenderOrder[i]; 
		eye_[eye] = smgr_->addEmptySceneNode(headRotationZ_);
		eye_[eye]->setPosition(eyeDist_[eye]); // apply IPD
	}

	// Finally add a camera node
	camera_ = smgr_->addCameraSceneNode(0, irr::core::vector3df(0,0,0), 
		irr::core::vector3df(0,0,100), -1, false);
}


OculusRenderer::~OculusRenderer(void)
{
	// Remove nodes since they're no longer needed
	camera_->remove();
	bodyRotationY_->remove();

	// Destroy and free the rift
	ovrHmd_Destroy(hmd_);
	ovr_Shutdown();	
}

void OculusRenderer::drawAll(irr::core::vector3df playerPosition, float playerYRotation, irr::video::SColor bgColor)
{
	// Start frame timing - used by timewarp
	ovrFrameTiming frameTiming = ovrHmd_BeginFrameTiming(hmd_, 0); 

	// Get head rotation
	irr::core::vector3df headRotation;
	ovrTrackingState ss = ovrHmd_GetTrackingState(hmd_, 0);

	// Convert orientation from quaternion to euler angles
	OVR::Quatf qua = ss.HeadPose.ThePose.Orientation;
	qua.GetEulerAngles<OVR::Axis_Y, OVR::Axis_X, OVR::Axis_Z>(&headRotation.Y, 
		&headRotation.X, &headRotation.Z);
	headRotation.X = -RadToDegree(headRotation.X);
	headRotation.Y = -RadToDegree(headRotation.Y);
	headRotation.Z = RadToDegree(headRotation.Z);

	// Store eye positions to be used with timewarping
	for(int i=0;i < 2; i++)
	{
		ovrEyeType eye = hmd_->EyeRenderOrder[i]; 
		eyePose_[eye] = ovrHmd_GetHmdPosePerEye(hmd_, eye);
	}

	// Get head position
	// optimal solution would be to use eye positions instead of head position
	irr::core::vector3df headPosition;
	headPosition.X = ss.HeadPose.ThePose.Position.x*worldScale_;
	headPosition.Y = ss.HeadPose.ThePose.Position.y*worldScale_;
	headPosition.Z = ss.HeadPose.ThePose.Position.z*-worldScale_;


	// Set rotations
	irr::core::vector3df bodyroty = irr::core::vector3df( 0.0f, playerYRotation, 0.0f); // Player's y-rotation
	irr::core::vector3df roty = irr::core::vector3df( 0.0f, headRotation.Y, 0.0f);
	irr::core::vector3df rotx = irr::core::vector3df(headRotation.X, 0.0f, 0.0f);
	irr::core::vector3df rotz = irr::core::vector3df(0.0f, 0.0f, headRotation.Z);

	// Apply rotations to nodes
	if(!this->getHeadNode())
	{
		// Only apply these if not linked to a node
		bodyRotationY_->setPosition(playerPosition);
		bodyRotationY_->setRotation(bodyroty);
	}

	headRotationY_->setRotation(roty);
	headRotationY_->setPosition(headPosition);
	headRotationX_->setRotation(rotx);
	headRotationZ_->setRotation(rotz);
	headRotationZ_->updateAbsolutePosition();


	// Finally get absolute rotation from the last head node. This replaces the need
	// to use any difficult math and let the engine handle it instead :)
	irr::core::matrix4 m;
	m.setRotationDegrees(headRotationZ_->getAbsoluteTransformation().getRotationDegrees());

	// transform forward vector of camera
	irr::core::vector3df frv = irr::core::vector3df (0.0f, 0.0f, 1.0f);
	m.transformVect(frv);
   
	// transform upvector of camera
	irr::core::vector3df upv = irr::core::vector3df (0.0f, 1.0f, 0.0f);
	m.transformVect(upv);


	// Set render target for rendering
	driver_->setRenderTarget(renderTexture_, true, true, bgColor);

	// Store current camera and switch to our camera
	irr::scene::ICameraSceneNode *tmpCamera = smgr_->getActiveCamera();
	smgr_->setActiveCamera(camera_);

	// Store current viewport
	irr::core::rect<irr::s32> oldView = driver_->getViewPort();

	// Render eyes
	for(int i=0;i < 2; i++)
	{
		ovrEyeType eye = hmd_->EyeRenderOrder[i]; 

		// Set viewport which was stored earlier
		driver_->setViewPort(irr::core::rect<irr::s32>(
			eyeRenderViewport_[eye].Pos.x, 
			eyeRenderViewport_[eye].Pos.y, 
			eyeRenderViewport_[eye].Pos.x  + eyeRenderViewport_[eye].Size.w, 
			eyeRenderViewport_[eye].Pos.y  + eyeRenderViewport_[eye].Size.h));

		// Make sure the eye's position is up to date
		eye_[eye]->updateAbsolutePosition();

		// Set camera for the current eye
		irr::core::vector3df eyepos = eye_[eye]->getAbsolutePosition();
		camera_->setPosition(eyepos);
		camera_->setProjectionMatrix(eyeProjection_[eye]);
		camera_->updateAbsolutePosition(); // probably unnecessary
		camera_->setTarget(eyepos + frv);
		camera_->setUpVector(upv);

		// Draw all
		smgr_->drawAll();
	}

	// Activate old camera
	smgr_->setActiveCamera(tmpCamera);

	
	// Reset to old view
	driver_->setViewPort(oldView);

	// Post processing could be applied for example here
	
	// Prepare to draw distortion
	renderMaterial_.TextureLayer[0].Texture = renderTexture_;
	driver_->setRenderTarget(0, true, true, irr::video::SColor(0,0,0,0));
	driver_->setMaterial(renderMaterial_);

	// add wait-timer here to make timewarp work correctly
	ovr_WaitTillTime(frameTiming.TimewarpPointSeconds);

	for(int i=0;i<2;i++)
	{
		ovrEyeType eye = hmd_->EyeRenderOrder[i];

		// Get matrixes required by timewarp
		ovrHmd_GetEyeTimewarpMatrices(hmd_, (ovrEyeType)eye, eyePose_[eye], distortionCB_.timeWarpMatrices);

		// Set the scale and offset values
		distortionCB_.EyeToSourceUVScale[0] = uvScaleOffset_[eye][0].x;
		distortionCB_.EyeToSourceUVScale[1] = uvScaleOffset_[eye][0].y;
		distortionCB_.EyeToSourceUVOffset[0] = uvScaleOffset_[eye][1].x;
		distortionCB_.EyeToSourceUVOffset[1] = uvScaleOffset_[eye][1].y;

		// Draw eye
		driver_->drawIndexedTriangleList(&EyeVB[(int)eye][0], EyeVB[(int)eye].size(), 
			&EyeIB[(int)eye][0], EyeIB[(int)eye].size()/3);
	}

	// And we're done
	ovrHmd_EndFrameTiming(hmd_);
}

void OculusRenderer::linkHeadNode(irr::scene::ISceneNode *headNode)
{
	linkedHead_ = headNode;

	if(linkedHead_)
		bodyRotationY_->setParent(linkedHead_);
	else
		bodyRotationY_->setParent(smgr_->getRootSceneNode());
}

irr::scene::ISceneNode *OculusRenderer::getHeadNode() const
{
	return linkedHead_;
}
