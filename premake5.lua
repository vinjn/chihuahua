-- http://industriousone.com/scripting-reference
-- https://github.com/premake/premake-core/wiki

local action = _ACTION or ""

solution "uEngine"
    location ("_project")
    configurations { "Debug", "Release" }
    platforms {"x64", "x86"}
    language "C++"
    kind "ConsoleApp"

    configuration "vs*"
        defines {
            "_CRT_SECURE_NO_WARNINGS",
            "_CRT_SECURE_NO_DEPRECATE",
        }

        configuration "x86"
            libdirs {
                "x86",
            }
            targetdir ("x86")

        configuration "x64"
            libdirs {
                "x64",
            }
            targetdir ("x64")

    flags {
        "MultiProcessorCompile"
    }

    configuration "Debug"
        defines { "DEBUG" }
        flags { "Symbols"}
        targetsuffix "-d"

    configuration "Release"
        defines { "NDEBUG" }
        flags { "Optimize"}

    project "imgui"
        kind "StaticLib"

        includedirs {
            "3rdparty",
        }

        files {
            "3rdparty/imgui/*.h",
            "3rdparty/imgui/*.cpp",
            "3rdparty/ImWindow/*.h",
            "3rdparty/ImWindow/*.cpp",
        }

    project "glfw"
        kind "StaticLib"

        includedirs { "3rdparty/glfw/include" }
        files { 
            "3rdparty/glfw/include/GLFW/*.h",
            "3rdparty/glfw/src/context.c",
            "3rdparty/glfw/src/init.c",
            "3rdparty/glfw/src/input.c",
            "3rdparty/glfw/src/monitor.c",
            "3rdparty/glfw/src/window.c",
        }

        defines { "_GLFW_USE_OPENGL" }

        configuration "windows"
            defines { "_GLFW_WIN32", "_GLFW_WGL" }
            files {
                "3rdparty/glfw/src/win32*.c",
                "3rdparty/glfw/src/wgl_context.c",
                "3rdparty/glfw/src/winmm_joystick.c",
            }

    project "glew"
        kind "StaticLib"

        includedirs {
            "3rdparty/glew",
        }

        files {
            "3rdparty/glew/glew.c",
        }

        defines {
            "GLEW_STATIC",
        }

    project "zlib"
        kind "StaticLib"

        includedirs {
            "3rdparty/zlib",
        }

        files {
            "3rdparty/zlib/*.c",
        }

    project "bgfx"
        kind "StaticLib"

        includedirs {
            "include",
            "3rdparty",
            "3rdparty/bgfx/include",
            "3rdparty/khronos",
        }

        configuration "vs*"
            includedirs {
                "3rdparty/compat/msvc",
            }

        files {
            "3rdparty/bgfx/src/*.h",
            "3rdparty/bgfx/src/*.cpp",
        }

        removefiles {
            "3rdparty/bgfx/src/amalgamated.cpp",
        }

    project "assimp"
        kind "StaticLib"

        includedirs {
            "3rdparty/zlib",
            "3rdparty/assimp/include",
            "3rdparty/assimp/code/BoostWorkaround",
            "3rdparty/assimp/contrib/rapidjson/include",
        }

        defines {
            "SWIG",
            "ASSIMP_BUILD_NO_OWN_ZLIB",

            "ASSIMP_BUILD_NO_X_IMPORTER",
            "ASSIMP_BUILD_NO_3DS_IMPORTER",
            "ASSIMP_BUILD_NO_MD3_IMPORTER",
            "ASSIMP_BUILD_NO_MDL_IMPORTER",
            "ASSIMP_BUILD_NO_MD2_IMPORTER",
            -- "ASSIMP_BUILD_NO_PLY_IMPORTER",
            "ASSIMP_BUILD_NO_ASE_IMPORTER",
            -- "ASSIMP_BUILD_NO_OBJ_IMPORTER",
            "ASSIMP_BUILD_NO_HMP_IMPORTER",
            "ASSIMP_BUILD_NO_SMD_IMPORTER",
            "ASSIMP_BUILD_NO_MDC_IMPORTER",
            "ASSIMP_BUILD_NO_MD5_IMPORTER",
            "ASSIMP_BUILD_NO_STL_IMPORTER",
            "ASSIMP_BUILD_NO_LWO_IMPORTER",
            "ASSIMP_BUILD_NO_DXF_IMPORTER",
            "ASSIMP_BUILD_NO_NFF_IMPORTER",
            "ASSIMP_BUILD_NO_RAW_IMPORTER",
            "ASSIMP_BUILD_NO_OFF_IMPORTER",
            "ASSIMP_BUILD_NO_AC_IMPORTER",
            "ASSIMP_BUILD_NO_BVH_IMPORTER",
            "ASSIMP_BUILD_NO_IRRMESH_IMPORTER",
            "ASSIMP_BUILD_NO_IRR_IMPORTER",
            "ASSIMP_BUILD_NO_Q3D_IMPORTER",
            "ASSIMP_BUILD_NO_B3D_IMPORTER",
            -- "ASSIMP_BUILD_NO_COLLADA_IMPORTER",
            "ASSIMP_BUILD_NO_TERRAGEN_IMPORTER",
            "ASSIMP_BUILD_NO_CSM_IMPORTER",
            "ASSIMP_BUILD_NO_3D_IMPORTER",
            "ASSIMP_BUILD_NO_LWS_IMPORTER",
            "ASSIMP_BUILD_NO_OGRE_IMPORTER",
            "ASSIMP_BUILD_NO_OPENGEX_IMPORTER",
            "ASSIMP_BUILD_NO_MS3D_IMPORTER",
            "ASSIMP_BUILD_NO_COB_IMPORTER",
            "ASSIMP_BUILD_NO_BLEND_IMPORTER",
            "ASSIMP_BUILD_NO_Q3BSP_IMPORTER",
            "ASSIMP_BUILD_NO_NDO_IMPORTER",
            "ASSIMP_BUILD_NO_IFC_IMPORTER",
            "ASSIMP_BUILD_NO_XGL_IMPORTER",
            -- "ASSIMP_BUILD_NO_FBX_IMPORTER",
            "ASSIMP_BUILD_NO_ASSBIN_IMPORTER",
            -- "ASSIMP_BUILD_NO_GLTF_IMPORTER",
            "ASSIMP_BUILD_NO_C4D_IMPORTER",
            "ASSIMP_BUILD_NO_STEP_EXPORTER",

            -- "ASSIMP_BUILD_NO_MAKELEFTHANDED_PROCESS",
            -- "ASSIMP_BUILD_NO_FLIPUVS_PROCESS",
            -- "ASSIMP_BUILD_NO_FLIPWINDINGORDER_PROCESS",
            "ASSIMP_BUILD_NO_CALCTANGENTS_PROCESS",
            "ASSIMP_BUILD_NO_JOINVERTICES_PROCESS",
            -- "ASSIMP_BUILD_NO_TRIANGULATE_PROCESS",
            "ASSIMP_BUILD_NO_GENFACENORMALS_PROCESS",
            -- "ASSIMP_BUILD_NO_GENVERTEXNORMALS_PROCESS",
            "ASSIMP_BUILD_NO_REMOVEVC_PROCESS",
            "ASSIMP_BUILD_NO_SPLITLARGEMESHES_PROCESS",
            "ASSIMP_BUILD_NO_PRETRANSFORMVERTICES_PROCESS",
            "ASSIMP_BUILD_NO_LIMITBONEWEIGHTS_PROCESS",
            -- "ASSIMP_BUILD_NO_VALIDATEDS_PROCESS",
            "ASSIMP_BUILD_NO_IMPROVECACHELOCALITY_PROCESS",
            "ASSIMP_BUILD_NO_FIXINFACINGNORMALS_PROCESS",
            "ASSIMP_BUILD_NO_REMOVE_REDUNDANTMATERIALS_PROCESS",
            "ASSIMP_BUILD_NO_FINDINVALIDDATA_PROCESS",
            "ASSIMP_BUILD_NO_FINDDEGENERATES_PROCESS",
            "ASSIMP_BUILD_NO_SORTBYPTYPE_PROCESS",
            "ASSIMP_BUILD_NO_GENUVCOORDS_PROCESS",
            "ASSIMP_BUILD_NO_TRANSFORMTEXCOORDS_PROCESS",
            "ASSIMP_BUILD_NO_FINDINSTANCES_PROCESS",
            "ASSIMP_BUILD_NO_OPTIMIZEMESHES_PROCESS",
            "ASSIMP_BUILD_NO_OPTIMIZEGRAPH_PROCESS",
            "ASSIMP_BUILD_NO_SPLITBYBONECOUNT_PROCESS",
            "ASSIMP_BUILD_NO_DEBONE_PROCESS",
        }

        files {
            "3rdparty/assimp/include/**",
            "3rdparty/assimp/code/Assimp.cpp",
            "3rdparty/assimp/code/BaseImporter.cpp",
            "3rdparty/assimp/code/ColladaLoader.cpp",
            "3rdparty/assimp/code/ColladaParser.cpp",
            "3rdparty/assimp/code/PlyParser.cpp",
            "3rdparty/assimp/code/PlyLoader.cpp",
            "3rdparty/assimp/code/BaseProcess.cpp",
            "3rdparty/assimp/code/FBXAnimation.cpp",
            "3rdparty/assimp/code/FBXBinaryTokenizer.cpp",
            "3rdparty/assimp/code/FBXConverter.cpp",
            "3rdparty/assimp/code/FBXDeformer.cpp",
            "3rdparty/assimp/code/FBXDocument.cpp",
            "3rdparty/assimp/code/FBXDocumentUtil.cpp",
            "3rdparty/assimp/code/FBXImporter.cpp",
            "3rdparty/assimp/code/FBXMaterial.cpp",
            "3rdparty/assimp/code/FBXMeshGeometry.cpp",
            "3rdparty/assimp/code/FBXModel.cpp",
            "3rdparty/assimp/code/FBXNodeAttribute.cpp",
            "3rdparty/assimp/code/FBXParser.cpp",
            "3rdparty/assimp/code/FBXProperties.cpp",
            "3rdparty/assimp/code/FBXTokenizer.cpp",
            "3rdparty/assimp/code/FBXUtil.cpp",
            "3rdparty/assimp/code/ConvertToLHProcess.cpp",
            "3rdparty/assimp/code/DefaultIOStream.cpp",
            "3rdparty/assimp/code/DefaultIOSystem.cpp",
            "3rdparty/assimp/code/DefaultLogger.cpp",
            "3rdparty/assimp/code/GenVertexNormalsProcess.cpp",
            "3rdparty/assimp/code/Importer.cpp",
            "3rdparty/assimp/code/ImporterRegistry.cpp",
            "3rdparty/assimp/code/MaterialSystem.cpp",
            "3rdparty/assimp/code/PostStepRegistry.cpp",
            "3rdparty/assimp/code/ProcessHelper.cpp",
            "3rdparty/assimp/code/ScenePreprocessor.cpp",
            "3rdparty/assimp/code/SGSpatialSort.cpp",
            "3rdparty/assimp/code/SkeletonMeshBuilder.cpp",
            "3rdparty/assimp/code/SpatialSort.cpp",
            "3rdparty/assimp/code/TriangulateProcess.cpp",
            "3rdparty/assimp/code/ValidateDataStructure.cpp",
            "3rdparty/assimp/code/Version.cpp",
            "3rdparty/assimp/code/VertexTriangleAdjacency.cpp",
            "3rdparty/assimp/code/ObjFileImporter.cpp",
            "3rdparty/assimp/code/ObjFileMtlImporter.cpp",
            "3rdparty/assimp/code/ObjFileParser.cpp",
            "3rdparty/assimp/code/glTFImporter.cpp",
            "3rdparty/assimp/contrib/ConvertUTF/ConvertUTF.c",
            "3rdparty/assimp/contrib/irrXML/*",
        }
-- 
    project "uEngine"
        kind "SharedLib"

        includedirs {
            "include",
            "3rdparty",
            "3rdparty/bgfx/include",
            "3rdparty/glew/include",
            "3rdparty/khronos",
            "3rdparty/assimp/include",
        }

        defines {
            "UENGINE_EXPORTS",
            "IRRLICHT_EXPORTS",
            "GLEW_STATIC",
            "SWIG",
        }

        files {
            "include/*",
            "wrapper/src/Scene3D.h",
            "wrapper/src/Scene3D.cpp",
            -- "wrapper/src/Javascript/Scene3D_v7.cpp",
            "source/*.h",
            "source/CAnimatedMeshHalfLife.cpp",
            "source/CAnimatedMeshMD2.cpp",
            "source/CAnimatedMeshMD3.cpp",
            "source/CAnimatedMeshSceneNode.cpp",
            "source/CAttributes.cpp",
            "source/CBillboardSceneNode.cpp",
            "source/CBoneSceneNode.cpp",
            "source/CCameraSceneNode.cpp",
            "source/CColorConverter.cpp",
            "source/CCubeSceneNode.cpp",
            "source/CDefaultSceneNodeAnimatorFactory.cpp",
            "source/CDefaultSceneNodeFactory.cpp",
            "source/CDMFLoader.cpp",
            "source/CDummyTransformationSceneNode.cpp",
            "source/CEmptySceneNode.cpp",
            "source/CFileList.cpp",
            "source/CFileSystem.cpp",
            "source/CFPSCounter.cpp",
            "source/leakHunter.cpp",
            "source/CGeometryCreator.cpp",
            "source/CImage.cpp",
            "source/CImageLoaderBMP.cpp",
            "source/CImageLoaderDDS.cpp",
            "source/CImageLoaderJPG.cpp",
            "source/CImageLoaderPCX.cpp",
            "source/CImageLoaderPNG.cpp",
            "source/CImageLoaderPPM.cpp",
            "source/CImageLoaderPSD.cpp",
            "source/CImageLoaderRGB.cpp",
            "source/CImageLoaderTGA.cpp",
            "source/CImageLoaderWAL.cpp",
            "source/CImageWriterBMP.cpp",
            "source/CImageWriterJPG.cpp",
            "source/CImageWriterPCX.cpp",
            "source/CImageWriterPNG.cpp",
            "source/CImageWriterPPM.cpp",
            "source/CImageWriterPSD.cpp",
            "source/CImageWriterTGA.cpp",
            "source/CImageLoaderPVR.cpp",
            "source/CIrrDeviceConsole.cpp",
            "source/CIrrDeviceFB.cpp",
            "source/CIrrDeviceLinux.cpp",
            "source/CIrrDeviceSDL.cpp",
            "source/CIrrDeviceStub.cpp",
            "source/CIrrDeviceWin32.cpp",
            "source/CIrrDeviceWinCE.cpp",
            "source/CLightSceneNode.cpp",
            "source/CLimitReadFile.cpp",
            "source/CLogger.cpp",
            "source/CMD2MeshFileLoader.cpp",
            "source/CMemoryFile.cpp",
            "source/CMeshCache.cpp",
            "source/CMeshManipulator.cpp",
            "source/CMeshSceneNode.cpp",
            "source/CMeshTextureLoader.cpp",
            "source/CMetaTriangleSelector.cpp",
            "source/CMountPointReader.cpp",
            "source/CNullDriver.cpp",
            "source/COctreeSceneNode.cpp",
            "source/COctreeTriangleSelector.cpp",
            "source/CEGLManager.cpp",
            "source/COGLES2Driver.cpp",
            "source/COGLES2ExtensionHandler.cpp",
            "source/COGLES2MaterialRenderer.cpp",
            "source/COGLES2FixedPipelineRenderer.cpp",
            "source/COGLES2NormalMapRenderer.cpp",
            "source/COGLES2ParallaxMapRenderer.cpp",
            "source/COGLES2Renderer2D.cpp",
            "source/COGLES2Texture.cpp",
            "source/COSOperator.cpp",
            "source/CParticleAnimatedMeshSceneNodeEmitter.cpp",
            "source/CParticleAttractionAffector.cpp",
            "source/CParticleBoxEmitter.cpp",
            "source/CParticleCylinderEmitter.cpp",
            "source/CParticleFadeOutAffector.cpp",
            "source/CParticleGravityAffector.cpp",
            "source/CParticleMeshEmitter.cpp",
            "source/CParticlePointEmitter.cpp",
            "source/CParticleRingEmitter.cpp",
            "source/CParticleRotationAffector.cpp",
            "source/CParticleScaleAffector.cpp",
            "source/CParticleSphereEmitter.cpp",
            "source/CParticleSystemSceneNode.cpp",
            "source/CProfiler.cpp",
            "source/CReadFile.cpp",
            "source/CSceneCollisionManager.cpp",
            "source/CSceneLoaderIrr.cpp",
            "source/CSceneManager.cpp",
            "source/CSceneNodeAnimatorCameraFPS.cpp",
            "source/CSceneNodeAnimatorCameraMaya.cpp",
            "source/CSceneNodeAnimatorCollisionResponse.cpp",
            "source/CSceneNodeAnimatorDelete.cpp",
            "source/CSceneNodeAnimatorFlyCircle.cpp",
            "source/CSceneNodeAnimatorFlyStraight.cpp",
            "source/CSceneNodeAnimatorFollowSpline.cpp",
            "source/CSceneNodeAnimatorRotation.cpp",
            "source/CSceneNodeAnimatorTexture.cpp",
            "source/CShadowVolumeSceneNode.cpp",
            "source/CSkinnedMesh.cpp",
            "source/CSkyBoxSceneNode.cpp",
            "source/CSkyDomeSceneNode.cpp",
            "source/CSphereSceneNode.cpp",
            "source/CTarReader.cpp",
            "source/CTerrainSceneNode.cpp",
            "source/CTerrainTriangleSelector.cpp",
            "source/CTextSceneNode.cpp",
            "source/CTriangleBBSelector.cpp",
            "source/CTriangleSelector.cpp",
            "source/CVideoModeList.cpp",
            "source/CVolumeLightSceneNode.cpp",
            "source/CWADReader.cpp",
            "source/CWaterSurfaceSceneNode.cpp",
            "source/CWriteFile.cpp",
            "source/CXMLReader.cpp",
            "source/CXMLWriter.cpp",
            "source/CZBuffer.cpp",
            "source/CZipReader.cpp",
            "source/Irrlicht.cpp",
            "source/irrXML.cpp",
            "source/os.cpp",
            "source/CWGLManager.cpp",
            "source/CD3D9*",
            "source/CBgfx*",
            "source/IrrAssimpImport.cpp",
            "source/HMDStereoRender.cpp",
        }

        links {
            "Psapi.lib",
        }

        configuration "Debug"
            links {
                "assimp-d.lib",
                "glew-d.lib",
                "bgfx-d.lib",
                "zlib-d.lib",
            }
        configuration "Release"
            links {
                "assimp.lib",
                "glew.lib",
                "bgfx.lib",
                "zlib.lib",
            }

    function create_app_project( app_path )
        leaf_name = string.sub(app_path, string.len("examples/") + 1);

        project (leaf_name)

            includedirs {
                "include",
                "3rdparty",
                "3rdparty/bgfx/include",
                "3rdparty/glfw/include",
                "examples/" .. leaf_name .. "/include",
            }

            files {
                "examples/" .. leaf_name .. "/**.h",
                "examples/" .. leaf_name .. "/**.cpp",
            }

            links {
                "opengl32.lib",
            }

            defines {
                "GLEW_STATIC",
            }

            configuration "Debug"
                links {
                    "uEngine-d.lib",
                    "imgui-d.lib",
                    "glfw-d.lib",
                    "glew-d.lib",
                }
            configuration "Release"
                links {
                    "uEngine.lib",
                    "imgui.lib",
                    "glfw.lib",
                    "glew.lib",
                }
    end

    local apps = os.matchdirs("examples/*")
    for _, app in ipairs(apps) do
        if not string.contains(app, "_") 
            and not string.contains(app, "Android")
            and not string.contains(app, "Mac")
            and not string.contains(app, "Demo")
            and not string.contains(app, "OculusSimple")
            and not string.contains(app, "Metaio")then
            create_app_project(app)
        end
    end 