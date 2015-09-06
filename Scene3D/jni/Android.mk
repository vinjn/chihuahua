LOCAL_PATH := $(call my-dir)/../../source/Irrlicht
SCENE3D_PATH := ../../Scene3D/src/Android
ASSIMP_PATH := ../assimp/

include $(CLEAR_VARS)

LOCAL_MODULE := Irrlicht

LOCAL_CFLAGS := -D_IRR_ANDROID_PLATFORM_ -DASSIMP_BUILD_NO_OWN_ZLIB -Wall -pipe -fno-exceptions -fno-rtti -fstrict-aliasing

#define NDEBUG

ifndef NDEBUG
LOCAL_CFLAGS += -g -D_DEBUG
else
LOCAL_CFLAGS += -fexpensive-optimizations -O3
endif

LOCAL_C_INCLUDES := ../include \
					../source/assimp/include \
					../source/assimp/code/BoostWorkaround

LOCAL_SRC_FILES := \
					$(SCENE3D_PATH)/Scene3DAndroid.cpp \
					$(SCENE3D_PATH)/CAndroidAssetFileArchive.cpp \
					$(SCENE3D_PATH)/CAndroidAssetReader.cpp \
    				$(ASSIMP_PATH)/AssimpWrapper.cpp \
    				$(ASSIMP_PATH)/code/Assimp.cpp \
    				$(ASSIMP_PATH)/code/ColladaLoader.cpp \
    				$(ASSIMP_PATH)/code/ColladaParser.cpp \
    				$(ASSIMP_PATH)/code/BaseImporter.cpp \
    				$(ASSIMP_PATH)/code/BaseProcess.cpp \
					$(ASSIMP_PATH)/code/FBXAnimation.cpp \
					$(ASSIMP_PATH)/code/FBXBinaryTokenizer.cpp \
					$(ASSIMP_PATH)/code/FBXConverter.cpp \
					$(ASSIMP_PATH)/code/FBXDeformer.cpp \
					$(ASSIMP_PATH)/code/FBXDocument.cpp \
					$(ASSIMP_PATH)/code/FBXDocumentUtil.cpp \
					$(ASSIMP_PATH)/code/FBXImporter.cpp \
					$(ASSIMP_PATH)/code/FBXMaterial.cpp \
					$(ASSIMP_PATH)/code/FBXMeshGeometry.cpp \
					$(ASSIMP_PATH)/code/FBXModel.cpp \
					$(ASSIMP_PATH)/code/FBXNodeAttribute.cpp \
					$(ASSIMP_PATH)/code/FBXParser.cpp \
					$(ASSIMP_PATH)/code/FBXProperties.cpp \
					$(ASSIMP_PATH)/code/FBXTokenizer.cpp \
					$(ASSIMP_PATH)/code/FBXUtil.cpp \
    				$(ASSIMP_PATH)/code/ConvertToLHProcess.cpp \
    				$(ASSIMP_PATH)/code/DefaultIOStream.cpp \
    				$(ASSIMP_PATH)/code/DefaultIOSystem.cpp \
    				$(ASSIMP_PATH)/code/DefaultLogger.cpp \
    				$(ASSIMP_PATH)/code/GenVertexNormalsProcess.cpp \
    				$(ASSIMP_PATH)/code/Importer.cpp \
    				$(ASSIMP_PATH)/code/ImporterRegistry.cpp \
    				$(ASSIMP_PATH)/code/MaterialSystem.cpp \
    				$(ASSIMP_PATH)/code/PostStepRegistry.cpp \
    				$(ASSIMP_PATH)/code/ProcessHelper.cpp \
    				$(ASSIMP_PATH)/code/ScenePreprocessor.cpp \
    				$(ASSIMP_PATH)/code/SGSpatialSort.cpp \
    				$(ASSIMP_PATH)/code/SkeletonMeshBuilder.cpp \
    				$(ASSIMP_PATH)/code/SpatialSort.cpp \
    				$(ASSIMP_PATH)/code/TriangulateProcess.cpp \
    				$(ASSIMP_PATH)/code/ValidateDataStructure.cpp \
    				$(ASSIMP_PATH)/code/Version.cpp \
    				$(ASSIMP_PATH)/code/VertexTriangleAdjacency.cpp \
    				$(ASSIMP_PATH)/contrib/ConvertUTF/ConvertUTF.c \
    				$(ASSIMP_PATH)/IrrAssimpImport.cpp \
					ISceneNode.cpp \
					CAnimatedMeshMD2.cpp \
					CAnimatedMeshSceneNode.cpp \
					CAttributes.cpp \
					CBillboardSceneNode.cpp \
					CBoneSceneNode.cpp \
					CCameraSceneNode.cpp \
					CColorConverter.cpp \
					CCubeSceneNode.cpp \
					CDefaultSceneNodeAnimatorFactory.cpp \
					CDefaultSceneNodeFactory.cpp \
					CDummyTransformationSceneNode.cpp \
					CEmptySceneNode.cpp \
					CFileList.cpp \
					CFileSystem.cpp \
					CFPSCounter.cpp \
					leakHunter.cpp \
					CGeometryCreator.cpp \
					CImage.cpp \
					CImageWriterBMP.cpp \
					CIrrDeviceStub.cpp \
					CLightSceneNode.cpp \
					CLimitReadFile.cpp \
					CLogger.cpp \
					CMD2MeshFileLoader.cpp \
					CMemoryFile.cpp \
					CMeshCache.cpp \
					CMeshManipulator.cpp \
					CMeshSceneNode.cpp \
					CMeshTextureLoader.cpp \
					CMetaTriangleSelector.cpp \
					CMountPointReader.cpp \
					CNullDriver.cpp \
					COBJMeshFileLoader.cpp \
					COBJMeshWriter.cpp \
					COCTLoader.cpp \
					COctreeSceneNode.cpp \
					COctreeTriangleSelector.cpp \
					CEGLManager.cpp \
					COGLES2Driver.cpp \
					COGLES2ExtensionHandler.cpp \
					COGLES2MaterialRenderer.cpp \
					COGLES2FixedPipelineRenderer.cpp \
					COGLES2NormalMapRenderer.cpp \
					COGLES2ParallaxMapRenderer.cpp \
					COGLES2Renderer2D.cpp \
					COGLES2Texture.cpp \
					COSOperator.cpp \
					CParticleAnimatedMeshSceneNodeEmitter.cpp \
					CParticleAttractionAffector.cpp \
					CParticleBoxEmitter.cpp \
					CParticleCylinderEmitter.cpp \
					CParticleFadeOutAffector.cpp \
					CParticleGravityAffector.cpp \
					CParticleMeshEmitter.cpp \
					CParticlePointEmitter.cpp \
					CParticleRingEmitter.cpp \
					CParticleRotationAffector.cpp \
					CParticleScaleAffector.cpp \
					CParticleSphereEmitter.cpp \
					CParticleSystemSceneNode.cpp \
					CProfiler.cpp \
					CReadFile.cpp \
					CSceneCollisionManager.cpp \
					CSceneLoaderIrr.cpp \
					CSceneManager.cpp \
					CSceneNodeAnimatorCameraFPS.cpp \
					CSceneNodeAnimatorCameraMaya.cpp \
					CSceneNodeAnimatorCollisionResponse.cpp \
					CSceneNodeAnimatorDelete.cpp \
					CSceneNodeAnimatorFlyCircle.cpp \
					CSceneNodeAnimatorFlyStraight.cpp \
					CSceneNodeAnimatorFollowSpline.cpp \
					CSceneNodeAnimatorRotation.cpp \
					CSceneNodeAnimatorTexture.cpp \
					CShadowVolumeSceneNode.cpp \
					CSkinnedMesh.cpp \
					CSkyBoxSceneNode.cpp \
					CSkyDomeSceneNode.cpp \
					CSphereSceneNode.cpp \
					CTerrainSceneNode.cpp \
					CTerrainTriangleSelector.cpp \
					CTextSceneNode.cpp \
					CTriangleBBSelector.cpp \
					CTriangleSelector.cpp \
					CVideoModeList.cpp \
					CVolumeLightSceneNode.cpp \
					CWaterSurfaceSceneNode.cpp \
					CWriteFile.cpp \
					CXMeshFileLoader.cpp \
					CXMLReader.cpp \
					CXMLWriter.cpp \
					Irrlicht.cpp \
					irrXML.cpp \
					os.cpp

LOCAL_LDLIBS     := -llog -lGLESv2 -lEGL -landroid -lz

include $(BUILD_SHARED_LIBRARY)