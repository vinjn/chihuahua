LOCAL_PATH := $(call my-dir)/../../
SOURCE_PATH := source
WRAPPER_PATH := wrapper/src
THIRDPARTY_PATH := 3rdparty
ASSIMP_PATH := $(THIRDPARTY_PATH)/assimp
BGFX_PATH := $(THIRDPARTY_PATH)/bgfx
JPEG_PATH := $(THIRDPARTY_PATH)/jpeglib
PNG_PATH := $(THIRDPARTY_PATH)/libpng

include $(CLEAR_VARS)

LOCAL_MODULE := Scene3D

LOCAL_CFLAGS := -Wall -pipe -fno-exceptions -fno-rtti -fstrict-aliasing
LOCAL_CFLAGS += -D_IRR_ANDROID_PLATFORM_
LOCAL_CFLAGS += -DRAPIDJSON_HAS_CXX11_RVALUE_REFS=0
LOCAL_CFLAGS += -D__STDINT_LIMITS
LOCAL_CFLAGS += -DASSIMP_BUILD_NO_OWN_ZLIB
LOCAL_CFLAGS += -DASSIMP_BUILD_NO_X_IMPORTER
LOCAL_CFLAGS += -DASSIMP_BUILD_NO_3DS_IMPORTER
LOCAL_CFLAGS += -DASSIMP_BUILD_NO_MD3_IMPORTER
LOCAL_CFLAGS += -DASSIMP_BUILD_NO_MDL_IMPORTER
LOCAL_CFLAGS += -DASSIMP_BUILD_NO_MD2_IMPORTER
LOCAL_CFLAGS += -DASSIMP_BUILD_NO_ASE_IMPORTER
LOCAL_CFLAGS += -DASSIMP_BUILD_NO_HMP_IMPORTER
LOCAL_CFLAGS += -DASSIMP_BUILD_NO_SMD_IMPORTER
LOCAL_CFLAGS += -DASSIMP_BUILD_NO_MDC_IMPORTER
LOCAL_CFLAGS += -DASSIMP_BUILD_NO_MD5_IMPORTER
LOCAL_CFLAGS += -DASSIMP_BUILD_NO_STL_IMPORTER
LOCAL_CFLAGS += -DASSIMP_BUILD_NO_LWO_IMPORTER
LOCAL_CFLAGS += -DASSIMP_BUILD_NO_DXF_IMPORTER
LOCAL_CFLAGS += -DASSIMP_BUILD_NO_NFF_IMPORTER
LOCAL_CFLAGS += -DASSIMP_BUILD_NO_RAW_IMPORTER
LOCAL_CFLAGS += -DASSIMP_BUILD_NO_OFF_IMPORTER
LOCAL_CFLAGS += -DASSIMP_BUILD_NO_AC_IMPORTER
LOCAL_CFLAGS += -DASSIMP_BUILD_NO_BVH_IMPORTER
LOCAL_CFLAGS += -DASSIMP_BUILD_NO_IRRMESH_IMPORTER
LOCAL_CFLAGS += -DASSIMP_BUILD_NO_IRR_IMPORTER
LOCAL_CFLAGS += -DASSIMP_BUILD_NO_Q3D_IMPORTER
LOCAL_CFLAGS += -DASSIMP_BUILD_NO_B3D_IMPORTER
LOCAL_CFLAGS += -DASSIMP_BUILD_NO_TERRAGEN_IMPORTER
LOCAL_CFLAGS += -DASSIMP_BUILD_NO_CSM_IMPORTER
LOCAL_CFLAGS += -DASSIMP_BUILD_NO_3D_IMPORTER
LOCAL_CFLAGS += -DASSIMP_BUILD_NO_LWS_IMPORTER
LOCAL_CFLAGS += -DASSIMP_BUILD_NO_OGRE_IMPORTER
LOCAL_CFLAGS += -DASSIMP_BUILD_NO_OPENGEX_IMPORTER
LOCAL_CFLAGS += -DASSIMP_BUILD_NO_MS3D_IMPORTER
LOCAL_CFLAGS += -DASSIMP_BUILD_NO_COB_IMPORTER
LOCAL_CFLAGS += -DASSIMP_BUILD_NO_BLEND_IMPORTER
LOCAL_CFLAGS += -DASSIMP_BUILD_NO_Q3BSP_IMPORTER
LOCAL_CFLAGS += -DASSIMP_BUILD_NO_NDO_IMPORTER
LOCAL_CFLAGS += -DASSIMP_BUILD_NO_IFC_IMPORTER
LOCAL_CFLAGS += -DASSIMP_BUILD_NO_XGL_IMPORTER
LOCAL_CFLAGS += -DASSIMP_BUILD_NO_ASSBIN_IMPORTER
LOCAL_CFLAGS += -DASSIMP_BUILD_NO_C4D_IMPORTER
LOCAL_CFLAGS += -DASSIMP_BUILD_NO_STEP_EXPORTER
LOCAL_CFLAGS += -DASSIMP_BUILD_NO_SIB_IMPORTER

LOCAL_CFLAGS += -DASSIMP_BUILD_NO_CALCTANGENTS_PROCESS
LOCAL_CFLAGS += -DASSIMP_BUILD_NO_JOINVERTICES_PROCESS
LOCAL_CFLAGS += -DASSIMP_BUILD_NO_GENFACENORMALS_PROCESS
LOCAL_CFLAGS += -DASSIMP_BUILD_NO_REMOVEVC_PROCESS
LOCAL_CFLAGS += -DASSIMP_BUILD_NO_SPLITLARGEMESHES_PROCESS
LOCAL_CFLAGS += -DASSIMP_BUILD_NO_PRETRANSFORMVERTICES_PROCESS
LOCAL_CFLAGS += -DASSIMP_BUILD_NO_LIMITBONEWEIGHTS_PROCESS
LOCAL_CFLAGS += -DASSIMP_BUILD_NO_IMPROVECACHELOCALITY_PROCESS
LOCAL_CFLAGS += -DASSIMP_BUILD_NO_FIXINFACINGNORMALS_PROCESS
LOCAL_CFLAGS += -DASSIMP_BUILD_NO_REMOVE_REDUNDANTMATERIALS_PROCESS
LOCAL_CFLAGS += -DASSIMP_BUILD_NO_FINDINVALIDDATA_PROCESS
LOCAL_CFLAGS += -DASSIMP_BUILD_NO_FINDDEGENERATES_PROCESS
LOCAL_CFLAGS += -DASSIMP_BUILD_NO_SORTBYPTYPE_PROCESS
LOCAL_CFLAGS += -DASSIMP_BUILD_NO_GENUVCOORDS_PROCESS
LOCAL_CFLAGS += -DASSIMP_BUILD_NO_TRANSFORMTEXCOORDS_PROCESS
LOCAL_CFLAGS += -DASSIMP_BUILD_NO_FINDINSTANCES_PROCESS
LOCAL_CFLAGS += -DASSIMP_BUILD_NO_OPTIMIZEMESHES_PROCESS
LOCAL_CFLAGS += -DASSIMP_BUILD_NO_OPTIMIZEGRAPH_PROCESS
LOCAL_CFLAGS += -DASSIMP_BUILD_NO_SPLITBYBONECOUNT_PROCESS
LOCAL_CFLAGS += -DASSIMP_BUILD_NO_DEBONE_PROCESS

# LOCAL_CFLAGS += -g -D_DEBUG
LOCAL_CFLAGS += -fexpensive-optimizations -O3

LOCAL_C_INCLUDES := ../include \
                    ../source \
                    ../3rdparty \
                    ../3rdparty/bgfx/include \
                    ../3rdparty/assimp/include \
                    ../3rdparty/assimp/code/BoostWorkaround \
                    ../3rdparty/assimp/contrib/rapidjson/include

LOCAL_SRC_FILES := \
                    $(WRAPPER_PATH)/Scene3D.cpp \
                    $(WRAPPER_PATH)/Javascript/Scene3D_v7.cpp \
                    $(WRAPPER_PATH)/Android/Scene3DAndroid.cpp \
                    $(WRAPPER_PATH)/Android/CAndroidAssetFileArchive.cpp \
                    $(WRAPPER_PATH)/Android/CAndroidAssetReader.cpp \
                    $(ASSIMP_PATH)/contrib/irrXML/irrXML.cpp \
                    $(ASSIMP_PATH)/code/Assimp.cpp \
                    $(ASSIMP_PATH)/code/BaseImporter.cpp \
                    $(ASSIMP_PATH)/code/ColladaLoader.cpp \
                    $(ASSIMP_PATH)/code/ColladaParser.cpp \
                    $(ASSIMP_PATH)/code/DXFLoader.cpp \
                    $(ASSIMP_PATH)/code/PlyParser.cpp \
                    $(ASSIMP_PATH)/code/PlyLoader.cpp \
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
                    $(ASSIMP_PATH)/code/ObjFileImporter.cpp \
                    $(ASSIMP_PATH)/code/ObjFileMtlImporter.cpp \
                    $(ASSIMP_PATH)/code/ObjFileParser.cpp \
                    $(ASSIMP_PATH)/code/glTFImporter.cpp \
                    $(ASSIMP_PATH)/contrib/ConvertUTF/ConvertUTF.c \
                    $(SOURCE_PATH)/IrrAssimpImport.cpp \
                    $(SOURCE_PATH)/C3DSMeshFileLoader.cpp \
                    $(SOURCE_PATH)/CAnimatedMeshHalfLife.cpp \
                    $(SOURCE_PATH)/CAnimatedMeshMD2.cpp \
                    $(SOURCE_PATH)/CAnimatedMeshMD3.cpp \
                    $(SOURCE_PATH)/CAnimatedMeshSceneNode.cpp \
                    $(SOURCE_PATH)/CAttributes.cpp \
                    $(SOURCE_PATH)/CB3DMeshFileLoader.cpp \
                    $(SOURCE_PATH)/CBillboardSceneNode.cpp \
                    $(SOURCE_PATH)/CBoneSceneNode.cpp \
                    $(SOURCE_PATH)/CBSPMeshFileLoader.cpp \
                    $(SOURCE_PATH)/CCameraSceneNode.cpp \
                    $(SOURCE_PATH)/CColladaFileLoader.cpp \
                    $(SOURCE_PATH)/CColladaMeshWriter.cpp \
                    $(SOURCE_PATH)/CColorConverter.cpp \
                    $(SOURCE_PATH)/CCSMLoader.cpp \
                    $(SOURCE_PATH)/CCubeSceneNode.cpp \
                    $(SOURCE_PATH)/CDefaultSceneNodeAnimatorFactory.cpp \
                    $(SOURCE_PATH)/CDefaultSceneNodeFactory.cpp \
                    $(SOURCE_PATH)/CDMFLoader.cpp \
                    $(SOURCE_PATH)/CDummyTransformationSceneNode.cpp \
                    $(SOURCE_PATH)/CEmptySceneNode.cpp \
                    $(SOURCE_PATH)/CFileList.cpp \
                    $(SOURCE_PATH)/CFileSystem.cpp \
                    $(SOURCE_PATH)/CFPSCounter.cpp \
                    $(SOURCE_PATH)/leakHunter.cpp \
                    $(SOURCE_PATH)/CGeometryCreator.cpp \
                    $(SOURCE_PATH)/CImage.cpp \
                    $(SOURCE_PATH)/CImageLoaderBMP.cpp \
                    $(SOURCE_PATH)/CImageLoaderDDS.cpp \
                    $(SOURCE_PATH)/CImageLoaderJPG.cpp \
                    $(SOURCE_PATH)/CImageLoaderPCX.cpp \
                    $(SOURCE_PATH)/CImageLoaderPNG.cpp \
                    $(SOURCE_PATH)/CImageLoaderPPM.cpp \
                    $(SOURCE_PATH)/CImageLoaderPSD.cpp \
                    $(SOURCE_PATH)/CImageLoaderRGB.cpp \
                    $(SOURCE_PATH)/CImageLoaderTGA.cpp \
                    $(SOURCE_PATH)/CImageLoaderWAL.cpp \
                    $(SOURCE_PATH)/CImageWriterBMP.cpp \
                    $(SOURCE_PATH)/CImageWriterJPG.cpp \
                    $(SOURCE_PATH)/CImageWriterPCX.cpp \
                    $(SOURCE_PATH)/CImageWriterPNG.cpp \
                    $(SOURCE_PATH)/CImageWriterPPM.cpp \
                    $(SOURCE_PATH)/CImageWriterPSD.cpp \
                    $(SOURCE_PATH)/CImageWriterTGA.cpp \
                    $(SOURCE_PATH)/CImageLoaderPVR.cpp \
                    $(SOURCE_PATH)/CIrrDeviceConsole.cpp \
                    $(SOURCE_PATH)/CIrrDeviceFB.cpp \
                    $(SOURCE_PATH)/CIrrDeviceLinux.cpp \
                    $(SOURCE_PATH)/CIrrDeviceSDL.cpp \
                    $(SOURCE_PATH)/CIrrDeviceStub.cpp \
                    $(SOURCE_PATH)/CIrrDeviceWin32.cpp \
                    $(SOURCE_PATH)/CIrrDeviceWinCE.cpp \
                    $(SOURCE_PATH)/CLightSceneNode.cpp \
                    $(SOURCE_PATH)/CLimitReadFile.cpp \
                    $(SOURCE_PATH)/CLMTSMeshFileLoader.cpp \
                    $(SOURCE_PATH)/CLogger.cpp \
                    $(SOURCE_PATH)/CLWOMeshFileLoader.cpp \
                    $(SOURCE_PATH)/CMD2MeshFileLoader.cpp \
                    $(SOURCE_PATH)/CMD3MeshFileLoader.cpp \
                    $(SOURCE_PATH)/CMemoryFile.cpp \
                    $(SOURCE_PATH)/CMeshCache.cpp \
                    $(SOURCE_PATH)/CMeshManipulator.cpp \
                    $(SOURCE_PATH)/CMeshSceneNode.cpp \
                    $(SOURCE_PATH)/CMeshTextureLoader.cpp \
                    $(SOURCE_PATH)/CMetaTriangleSelector.cpp \
                    $(SOURCE_PATH)/CMountPointReader.cpp \
                    $(SOURCE_PATH)/CNullDriver.cpp \
                    $(SOURCE_PATH)/COctreeSceneNode.cpp \
                    $(SOURCE_PATH)/COctreeTriangleSelector.cpp \
                    $(SOURCE_PATH)/CEGLManager.cpp \
                    $(SOURCE_PATH)/COGLES2Driver.cpp \
                    $(SOURCE_PATH)/COGLES2ExtensionHandler.cpp \
                    $(SOURCE_PATH)/COGLES2MaterialRenderer.cpp \
                    $(SOURCE_PATH)/COGLES2FixedPipelineRenderer.cpp \
                    $(SOURCE_PATH)/COGLES2NormalMapRenderer.cpp \
                    $(SOURCE_PATH)/COGLES2ParallaxMapRenderer.cpp \
                    $(SOURCE_PATH)/COGLES2Renderer2D.cpp \
                    $(SOURCE_PATH)/COGLES2Texture.cpp \
                    $(SOURCE_PATH)/COSOperator.cpp \
                    $(SOURCE_PATH)/CParticleAnimatedMeshSceneNodeEmitter.cpp \
                    $(SOURCE_PATH)/CParticleAttractionAffector.cpp \
                    $(SOURCE_PATH)/CParticleBoxEmitter.cpp \
                    $(SOURCE_PATH)/CParticleCylinderEmitter.cpp \
                    $(SOURCE_PATH)/CParticleFadeOutAffector.cpp \
                    $(SOURCE_PATH)/CParticleGravityAffector.cpp \
                    $(SOURCE_PATH)/CParticleMeshEmitter.cpp \
                    $(SOURCE_PATH)/CParticlePointEmitter.cpp \
                    $(SOURCE_PATH)/CParticleRingEmitter.cpp \
                    $(SOURCE_PATH)/CParticleRotationAffector.cpp \
                    $(SOURCE_PATH)/CParticleScaleAffector.cpp \
                    $(SOURCE_PATH)/CParticleSphereEmitter.cpp \
                    $(SOURCE_PATH)/CParticleSystemSceneNode.cpp \
                    $(SOURCE_PATH)/CPLYMeshFileLoader.cpp \
                    $(SOURCE_PATH)/CPLYMeshWriter.cpp \
                    $(SOURCE_PATH)/CProfiler.cpp \
                    $(SOURCE_PATH)/CReadFile.cpp \
                    $(SOURCE_PATH)/CSceneCollisionManager.cpp \
                    $(SOURCE_PATH)/CSceneLoaderIrr.cpp \
                    $(SOURCE_PATH)/CSceneManager.cpp \
                    $(SOURCE_PATH)/CSceneNodeAnimatorCameraFPS.cpp \
                    $(SOURCE_PATH)/CSceneNodeAnimatorCameraMaya.cpp \
                    $(SOURCE_PATH)/CSceneNodeAnimatorCollisionResponse.cpp \
                    $(SOURCE_PATH)/CSceneNodeAnimatorDelete.cpp \
                    $(SOURCE_PATH)/CSceneNodeAnimatorFlyCircle.cpp \
                    $(SOURCE_PATH)/CSceneNodeAnimatorFlyStraight.cpp \
                    $(SOURCE_PATH)/CSceneNodeAnimatorFollowSpline.cpp \
                    $(SOURCE_PATH)/CSceneNodeAnimatorRotation.cpp \
                    $(SOURCE_PATH)/CSceneNodeAnimatorTexture.cpp \
                    $(SOURCE_PATH)/CShadowVolumeSceneNode.cpp \
                    $(SOURCE_PATH)/CSkinnedMesh.cpp \
                    $(SOURCE_PATH)/CSkyBoxSceneNode.cpp \
                    $(SOURCE_PATH)/CSkyDomeSceneNode.cpp \
                    $(SOURCE_PATH)/CSMFMeshFileLoader.cpp \
                    $(SOURCE_PATH)/CSphereSceneNode.cpp \
                    $(SOURCE_PATH)/CSTLMeshFileLoader.cpp \
                    $(SOURCE_PATH)/CSTLMeshWriter.cpp \
                    $(SOURCE_PATH)/CTarReader.cpp \
                    $(SOURCE_PATH)/CTerrainSceneNode.cpp \
                    $(SOURCE_PATH)/CTerrainTriangleSelector.cpp \
                    $(SOURCE_PATH)/CTextSceneNode.cpp \
                    $(SOURCE_PATH)/CTriangleBBSelector.cpp \
                    $(SOURCE_PATH)/CTriangleSelector.cpp \
                    $(SOURCE_PATH)/CVideoModeList.cpp \
                    $(SOURCE_PATH)/CVolumeLightSceneNode.cpp \
                    $(SOURCE_PATH)/CWADReader.cpp \
                    $(SOURCE_PATH)/CWaterSurfaceSceneNode.cpp \
                    $(SOURCE_PATH)/CWriteFile.cpp \
                    $(SOURCE_PATH)/CXMeshFileLoader.cpp \
                    $(SOURCE_PATH)/CXMLReader.cpp \
                    $(SOURCE_PATH)/CXMLWriter.cpp \
                    $(SOURCE_PATH)/CZBuffer.cpp \
                    $(SOURCE_PATH)/CZipReader.cpp \
                    $(SOURCE_PATH)/Irrlicht.cpp \
                    $(SOURCE_PATH)/irrXML.cpp \
                    $(SOURCE_PATH)/os.cpp   \
                    $(JPEG_PATH)/jdphuff.c \
                    $(JPEG_PATH)/jcphuff.c \
                    $(JPEG_PATH)/jcapimin.c \
                    $(JPEG_PATH)/jidctred.c \
                    $(JPEG_PATH)/jcapistd.c \
                    $(JPEG_PATH)/jccoefct.c \
                    $(JPEG_PATH)/jccolor.c \
                    $(JPEG_PATH)/jcdctmgr.c \
                    $(JPEG_PATH)/jchuff.c \
                    $(JPEG_PATH)/jcinit.c \
                    $(JPEG_PATH)/jcmainct.c \
                    $(JPEG_PATH)/jcmarker.c \
                    $(JPEG_PATH)/jcmaster.c \
                    $(JPEG_PATH)/jcomapi.c \
                    $(JPEG_PATH)/jcparam.c \
                    $(JPEG_PATH)/jcprepct.c \
                    $(JPEG_PATH)/jcsample.c \
                    $(JPEG_PATH)/jctrans.c \
                    $(JPEG_PATH)/jdapimin.c \
                    $(JPEG_PATH)/jdapistd.c \
                    $(JPEG_PATH)/jdatadst.c \
                    $(JPEG_PATH)/jdatasrc.c \
                    $(JPEG_PATH)/jdcoefct.c \
                    $(JPEG_PATH)/jdcolor.c \
                    $(JPEG_PATH)/jddctmgr.c \
                    $(JPEG_PATH)/jdhuff.c \
                    $(JPEG_PATH)/jdinput.c \
                    $(JPEG_PATH)/jdmainct.c \
                    $(JPEG_PATH)/jdmarker.c \
                    $(JPEG_PATH)/jdmaster.c \
                    $(JPEG_PATH)/jdmerge.c \
                    $(JPEG_PATH)/jdpostct.c \
                    $(JPEG_PATH)/jdsample.c \
                    $(JPEG_PATH)/jdtrans.c \
                    $(JPEG_PATH)/jerror.c \
                    $(JPEG_PATH)/jfdctflt.c \
                    $(JPEG_PATH)/jfdctfst.c \
                    $(JPEG_PATH)/jfdctint.c \
                    $(JPEG_PATH)/jidctflt.c \
                    $(JPEG_PATH)/jidctfst.c \
                    $(JPEG_PATH)/jidctint.c \
                    $(JPEG_PATH)/jmemmgr.c \
                    $(JPEG_PATH)/jmemnobs.c \
                    $(JPEG_PATH)/jquant1.c \
                    $(JPEG_PATH)/jquant2.c \
                    $(JPEG_PATH)/jutils.c \
                    $(PNG_PATH)/png.c \
                    $(PNG_PATH)/pngerror.c \
                    $(PNG_PATH)/pngget.c \
                    $(PNG_PATH)/pngmem.c \
                    $(PNG_PATH)/pngpread.c \
                    $(PNG_PATH)/pngread.c \
                    $(PNG_PATH)/pngrio.c \
                    $(PNG_PATH)/pngrtran.c \
                    $(PNG_PATH)/pngrutil.c \
                    $(PNG_PATH)/pngset.c \
                    $(PNG_PATH)/pngtrans.c \
                    $(PNG_PATH)/pngwio.c \
                    $(PNG_PATH)/pngwrite.c \
                    $(PNG_PATH)/pngwtran.c \
                    $(PNG_PATH)/pngwutil.c

LOCAL_LDLIBS     := -llog -lGLESv2 -lEGL -landroid -lz

include $(BUILD_SHARED_LIBRARY)