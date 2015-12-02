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

    project "glew"
        kind "StaticLib"

        includedirs {
            "source/glew",
        }

        files {
            "source/glew/glew.c",
        }

        defines {
            "GLEW_STATIC",
        }

    project "zlib"
        kind "StaticLib"

        includedirs {
            "source/Irrlicht/zlib",
        }

        files {
            "source/Irrlicht/zlib/*.c",
        }

    project "bgfx"
        kind "StaticLib"

        includedirs {
            "include",
            "source/khronos",
        }

        configuration "vs*"
            includedirs {
                "include/compat/msvc",
            }

        files {
            "source/bgfx/src/*.h",
            "source/bgfx/src/*.cpp",
        }

        removefiles {
            "source/bgfx/src/amalgamated.cpp",
        }

    project "assimp"
        kind "StaticLib"

        includedirs {
            "source/assimp/include",
            "source/assimp/code/BoostWorkaround",
            "source/assimp/contrib/rapidjson/include",
        }

        files {
            "source/assimp/include/**",
            "source/assimp/code/Assimp.cpp",
            "source/assimp/code/BaseImporter.cpp",
            "source/assimp/code/ColladaLoader.cpp",
            "source/assimp/code/ColladaParser.cpp",
            "source/assimp/code/PlyParser.cpp",
            "source/assimp/code/PlyLoader.cpp",
            "source/assimp/code/BaseProcess.cpp",
            "source/assimp/code/FBXAnimation.cpp",
            "source/assimp/code/FBXBinaryTokenizer.cpp",
            "source/assimp/code/FBXConverter.cpp",
            "source/assimp/code/FBXDeformer.cpp",
            "source/assimp/code/FBXDocument.cpp",
            "source/assimp/code/FBXDocumentUtil.cpp",
            "source/assimp/code/FBXImporter.cpp",
            "source/assimp/code/FBXMaterial.cpp",
            "source/assimp/code/FBXMeshGeometry.cpp",
            "source/assimp/code/FBXModel.cpp",
            "source/assimp/code/FBXNodeAttribute.cpp",
            "source/assimp/code/FBXParser.cpp",
            "source/assimp/code/FBXProperties.cpp",
            "source/assimp/code/FBXTokenizer.cpp",
            "source/assimp/code/FBXUtil.cpp",
            "source/assimp/code/ConvertToLHProcess.cpp",
            "source/assimp/code/DefaultIOStream.cpp",
            "source/assimp/code/DefaultIOSystem.cpp",
            "source/assimp/code/DefaultLogger.cpp",
            "source/assimp/code/GenVertexNormalsProcess.cpp",
            "source/assimp/code/Importer.cpp",
            "source/assimp/code/ImporterRegistry.cpp",
            "source/assimp/code/MaterialSystem.cpp",
            "source/assimp/code/PostStepRegistry.cpp",
            "source/assimp/code/ProcessHelper.cpp",
            "source/assimp/code/ScenePreprocessor.cpp",
            "source/assimp/code/SGSpatialSort.cpp",
            "source/assimp/code/SkeletonMeshBuilder.cpp",
            "source/assimp/code/SpatialSort.cpp",
            "source/assimp/code/TriangulateProcess.cpp",
            "source/assimp/code/ValidateDataStructure.cpp",
            "source/assimp/code/Version.cpp",
            "source/assimp/code/VertexTriangleAdjacency.cpp",
            "source/assimp/code/ObjFileImporter.cpp",
            "source/assimp/code/ObjFileMtlImporter.cpp",
            "source/assimp/code/ObjFileParser.cpp",
            "source/assimp/code/glTFUtil.cpp",
            "source/assimp/code/glTFImporter.cpp",
            "source/assimp/contrib/ConvertUTF/ConvertUTF.c",
            "source/assimp/contrib/irrXML/*",
        }
-- 
    project "uEngine"
        kind "SharedLib"

        includedirs {
            "include",
            "src",
            "source/assimp/include",
        }

        defines {
            "UENGINE_EXPORTS",
            "IRRLICHT_EXPORTS",
            "GLEW_STATIC",
        }

        files {
            "include/*",
            "source/Irrlicht/*.h",
            "source/Irrlicht/CAnimatedMeshHalfLife.cpp",
            "source/Irrlicht/CAnimatedMeshMD2.cpp",
            "source/Irrlicht/CAnimatedMeshMD3.cpp",
            "source/Irrlicht/CAnimatedMeshSceneNode.cpp",
            "source/Irrlicht/CAttributes.cpp",
            "source/Irrlicht/CBillboardSceneNode.cpp",
            "source/Irrlicht/CBoneSceneNode.cpp",
            "source/Irrlicht/CCameraSceneNode.cpp",
            "source/Irrlicht/CColorConverter.cpp",
            "source/Irrlicht/CCubeSceneNode.cpp",
            "source/Irrlicht/CDefaultSceneNodeAnimatorFactory.cpp",
            "source/Irrlicht/CDefaultSceneNodeFactory.cpp",
            "source/Irrlicht/CDMFLoader.cpp",
            "source/Irrlicht/CDummyTransformationSceneNode.cpp",
            "source/Irrlicht/CEmptySceneNode.cpp",
            "source/Irrlicht/CFileList.cpp",
            "source/Irrlicht/CFileSystem.cpp",
            "source/Irrlicht/CFPSCounter.cpp",
            "source/Irrlicht/leakHunter.cpp",
            "source/Irrlicht/CGeometryCreator.cpp",
            "source/Irrlicht/CImage.cpp",
            "source/Irrlicht/CImageLoaderBMP.cpp",
            "source/Irrlicht/CImageLoaderDDS.cpp",
            "source/Irrlicht/CImageLoaderJPG.cpp",
            "source/Irrlicht/CImageLoaderPCX.cpp",
            "source/Irrlicht/CImageLoaderPNG.cpp",
            "source/Irrlicht/CImageLoaderPPM.cpp",
            "source/Irrlicht/CImageLoaderPSD.cpp",
            "source/Irrlicht/CImageLoaderRGB.cpp",
            "source/Irrlicht/CImageLoaderTGA.cpp",
            "source/Irrlicht/CImageLoaderWAL.cpp",
            "source/Irrlicht/CImageWriterBMP.cpp",
            "source/Irrlicht/CImageWriterJPG.cpp",
            "source/Irrlicht/CImageWriterPCX.cpp",
            "source/Irrlicht/CImageWriterPNG.cpp",
            "source/Irrlicht/CImageWriterPPM.cpp",
            "source/Irrlicht/CImageWriterPSD.cpp",
            "source/Irrlicht/CImageWriterTGA.cpp",
            "source/Irrlicht/CImageLoaderPVR.cpp",
            "source/Irrlicht/CIrrDeviceConsole.cpp",
            "source/Irrlicht/CIrrDeviceFB.cpp",
            "source/Irrlicht/CIrrDeviceLinux.cpp",
            "source/Irrlicht/CIrrDeviceSDL.cpp",
            "source/Irrlicht/CIrrDeviceStub.cpp",
            "source/Irrlicht/CIrrDeviceWin32.cpp",
            "source/Irrlicht/CIrrDeviceWinCE.cpp",
            "source/Irrlicht/CLightSceneNode.cpp",
            "source/Irrlicht/CLimitReadFile.cpp",
            "source/Irrlicht/CLogger.cpp",
            "source/Irrlicht/CMD2MeshFileLoader.cpp",
            "source/Irrlicht/CMemoryFile.cpp",
            "source/Irrlicht/CMeshCache.cpp",
            "source/Irrlicht/CMeshManipulator.cpp",
            "source/Irrlicht/CMeshSceneNode.cpp",
            "source/Irrlicht/CMeshTextureLoader.cpp",
            "source/Irrlicht/CMetaTriangleSelector.cpp",
            "source/Irrlicht/CMountPointReader.cpp",
            "source/Irrlicht/CNullDriver.cpp",
            "source/Irrlicht/COctreeSceneNode.cpp",
            "source/Irrlicht/COctreeTriangleSelector.cpp",
            "source/Irrlicht/CEGLManager.cpp",
            "source/Irrlicht/COGLES2Driver.cpp",
            "source/Irrlicht/COGLES2ExtensionHandler.cpp",
            "source/Irrlicht/COGLES2MaterialRenderer.cpp",
            "source/Irrlicht/COGLES2FixedPipelineRenderer.cpp",
            "source/Irrlicht/COGLES2NormalMapRenderer.cpp",
            "source/Irrlicht/COGLES2ParallaxMapRenderer.cpp",
            "source/Irrlicht/COGLES2Renderer2D.cpp",
            "source/Irrlicht/COGLES2Texture.cpp",
            "source/Irrlicht/COSOperator.cpp",
            "source/Irrlicht/CParticleAnimatedMeshSceneNodeEmitter.cpp",
            "source/Irrlicht/CParticleAttractionAffector.cpp",
            "source/Irrlicht/CParticleBoxEmitter.cpp",
            "source/Irrlicht/CParticleCylinderEmitter.cpp",
            "source/Irrlicht/CParticleFadeOutAffector.cpp",
            "source/Irrlicht/CParticleGravityAffector.cpp",
            "source/Irrlicht/CParticleMeshEmitter.cpp",
            "source/Irrlicht/CParticlePointEmitter.cpp",
            "source/Irrlicht/CParticleRingEmitter.cpp",
            "source/Irrlicht/CParticleRotationAffector.cpp",
            "source/Irrlicht/CParticleScaleAffector.cpp",
            "source/Irrlicht/CParticleSphereEmitter.cpp",
            "source/Irrlicht/CParticleSystemSceneNode.cpp",
            "source/Irrlicht/CProfiler.cpp",
            "source/Irrlicht/CReadFile.cpp",
            "source/Irrlicht/CSceneCollisionManager.cpp",
            "source/Irrlicht/CSceneLoaderIrr.cpp",
            "source/Irrlicht/CSceneManager.cpp",
            "source/Irrlicht/CSceneNodeAnimatorCameraFPS.cpp",
            "source/Irrlicht/CSceneNodeAnimatorCameraMaya.cpp",
            "source/Irrlicht/CSceneNodeAnimatorCollisionResponse.cpp",
            "source/Irrlicht/CSceneNodeAnimatorDelete.cpp",
            "source/Irrlicht/CSceneNodeAnimatorFlyCircle.cpp",
            "source/Irrlicht/CSceneNodeAnimatorFlyStraight.cpp",
            "source/Irrlicht/CSceneNodeAnimatorFollowSpline.cpp",
            "source/Irrlicht/CSceneNodeAnimatorRotation.cpp",
            "source/Irrlicht/CSceneNodeAnimatorTexture.cpp",
            "source/Irrlicht/CShadowVolumeSceneNode.cpp",
            "source/Irrlicht/CSkinnedMesh.cpp",
            "source/Irrlicht/CSkyBoxSceneNode.cpp",
            "source/Irrlicht/CSkyDomeSceneNode.cpp",
            "source/Irrlicht/CSphereSceneNode.cpp",
            "source/Irrlicht/CTarReader.cpp",
            "source/Irrlicht/CTerrainSceneNode.cpp",
            "source/Irrlicht/CTerrainTriangleSelector.cpp",
            "source/Irrlicht/CTextSceneNode.cpp",
            "source/Irrlicht/CTriangleBBSelector.cpp",
            "source/Irrlicht/CTriangleSelector.cpp",
            "source/Irrlicht/CVideoModeList.cpp",
            "source/Irrlicht/CVolumeLightSceneNode.cpp",
            "source/Irrlicht/CWADReader.cpp",
            "source/Irrlicht/CWaterSurfaceSceneNode.cpp",
            "source/Irrlicht/CWriteFile.cpp",
            "source/Irrlicht/CXMLReader.cpp",
            "source/Irrlicht/CXMLWriter.cpp",
            "source/Irrlicht/CZBuffer.cpp",
            "source/Irrlicht/CZipReader.cpp",
            "source/Irrlicht/Irrlicht.cpp",
            "source/Irrlicht/irrXML.cpp",
            "source/Irrlicht/os.cpp",
            "source/Irrlicht/CWGLManager.cpp",
            "source/Irrlicht/CD3D9*",
            "source/Irrlicht/CBgfx*",
            "source/assimp/IrrAssimpImport.cpp",
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

    -- function create_app_project( app_path )
    --     leaf_name = string.sub(app_path, string.len("apps/") + 1);

    --     project (leaf_name)

    --         includedirs {
    --             "include",
    --             "../opencv-lib/include",
    --             "../eigen", -- for dvo-slam 
    --             "../opencv-gfx/include",
    --             "apps/" .. leaf_name .. "/include",
    --         }

    --         if CUDA_PATH ~= nil then
    --             includedirs { 
    --                 path.join("$(CUDA_PATH)", "include"),
    --             }
    --             links {
    --                 "cuda.lib",
    --                 "cudart.lib",
    --                 "nvrtc.lib"
    --             }
    --             configuration {"x86", "windows"}
    --                 libdirs {
    --                     path.join("$(CUDA_PATH)", "lib/win32"),
    --                 }
    --             configuration {"x64", "windows"}
    --                 libdirs {
    --                     path.join("$(CUDA_PATH)", "lib/x64"),
    --                 }
    --         end

    --         files {
    --             "apps/" .. leaf_name .. "/**",
    --         }

    --         links {
    --             "opengl32.lib",
    --             "glu32.lib",
    --         }

    --         configuration "Debug"
    --             links {
    --                 "opencv-rgbd-d.lib",
    --                 "opencv-gfx-d.lib",
    --             }
    --         configuration "Release"
    --             links {
    --                 "opencv-rgbd.lib",
    --                 "opencv-gfx.lib",
    --             }
    -- end

    -- local apps = os.matchdirs("apps/*")
    -- for _, app in ipairs(apps) do
    --     create_app_project(app)
    -- end 