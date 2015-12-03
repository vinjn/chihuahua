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
            "ASSIMP_BUILD_NO_OWN_ZLIB",
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
            "3rdparty/assimp/code/glTFUtil.cpp",
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
        }

        files {
            "include/*",
            "Scene3D/src/Scene3D.h",
            "Scene3D/src/Scene3D.cpp",
            -- "Scene3D/src/Javascript/Scene3D_v7.cpp",
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