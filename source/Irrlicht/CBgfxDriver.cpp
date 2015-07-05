// Copyright (C) 2014 Patryk Nadrowski
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in Irrlicht.h

#ifndef __C_BGFX_DRIVER_H_INCLUDED__
#define __C_BGFX_DRIVER_H_INCLUDED__

#include "IrrCompileConfig.h"

#include "SIrrCreationParameters.h"

#ifdef _IRR_COMPILE_WITH_BGFX_

#include "CNullDriver.h"
#include "IMaterialRendererServices.h"
#include "EDriverFeatures.h"
#include "IContextManager.h"

#include "bgfx.h"
#include "bgfxplatform.h"

namespace irr
{
    namespace video
    {
        class CBgfxDriver : public CNullDriver, public IMaterialRendererServices
        {
        public:
            //! constructor
            CBgfxDriver(const SIrrlichtCreationParameters& params,
                io::IFileSystem* io
#if defined(_IRR_COMPILE_WITH_X11_DEVICE_) || defined(_IRR_WINDOWS_API_) || defined(_IRR_COMPILE_WITH_ANDROID_DEVICE_) || defined(_IRR_COMPILE_WITH_FB_DEVICE_)
                , IContextManager* contextManager
#elif defined(_IRR_COMPILE_WITH_IPHONE_DEVICE_)
                , CIrrDeviceIPhone* device
#endif
                ) :CNullDriver(io, params.WindowSize)
            {
                bgfx::winSetHwnd((HWND)params.WindowId);
                bgfx::init();

                uint32_t reset = BGFX_RESET_VSYNC;
                bgfx::reset(ScreenSize.Width, ScreenSize.Height, reset);

                uint32_t debug = BGFX_DEBUG_TEXT;
                bgfx::setDebug(debug);
            }

            //! destructor
            virtual ~CBgfxDriver()
            {
                bgfx::shutdown();
            }

            //! clears the zbuffer
            virtual bool beginScene(bool backBuffer = true, bool zBuffer = true,
                SColor color = SColor(255, 0, 0, 0),
                const SExposedVideoData& videoData = SExposedVideoData(),
                core::rect<s32>* sourceRect = 0)
            {
                // Set view 0 clear state.
                bgfx::setViewClear(0
                    , (backBuffer ? BGFX_CLEAR_COLOR : 0) | (zBuffer ? BGFX_CLEAR_DEPTH : 0) | 0
                    , color.color
                    , 1.0f
                    , 0
                    );

                return true;
            }

            //! presents the rendered scene on the screen, returns false if failed
            virtual bool endScene()
            {
                bgfx::frame();

                return true;
            }

            //! sets transformation
            virtual void setTransform(E_TRANSFORMATION_STATE state, const core::matrix4& mat)
            {

            }

            //! updates hardware buffer if needed
            virtual bool updateHardwareBuffer(SHWBufferLink *HWBuffer)
            {
                return false;
            }

            //! Create hardware buffer from mesh
            virtual SHWBufferLink *createHardwareBuffer(const scene::IMeshBuffer* mb)
            {
                return NULL;
            }

            //! Delete hardware buffer (only some drivers can)
            virtual void deleteHardwareBuffer(SHWBufferLink *HWBuffer) {}

            //! Draw hardware buffer
            virtual void drawHardwareBuffer(SHWBufferLink *HWBuffer) {}

            //! draws a vertex primitive list
            virtual void drawVertexPrimitiveList(const void* vertices, u32 vertexCount,
                const void* indexList, u32 primitiveCount,
                E_VERTEX_TYPE vType, scene::E_PRIMITIVE_TYPE pType, E_INDEX_TYPE iType) {}

            //! queries the features of the driver, returns true if feature is available
            virtual bool queryFeature(E_VIDEO_DRIVER_FEATURE feature) const
            {
                // TODO:
                return FeatureEnabled[feature];
            }

            //! Sets a material.
            virtual void setMaterial(const SMaterial& material) {}

            //! draws an 2d image, using a color (if color is other then Color(255,255,255,255)) and the alpha channel of the texture if wanted.
            virtual void draw2DImage(const video::ITexture* texture,
                const core::position2d<s32>& destPos,
                const core::rect<s32>& sourceRect, const core::rect<s32>* clipRect = 0,
                SColor color = SColor(255, 255, 255, 255), bool useAlphaChannelOfTexture = false) {}

            //! draws a set of 2d images
            virtual void draw2DImageBatch(const video::ITexture* texture,
                const core::position2d<s32>& pos,
                const core::array<core::rect<s32> >& sourceRects,
                const core::array<s32>& indices, s32 kerningWidth = 0,
                const core::rect<s32>* clipRect = 0,
                SColor color = SColor(255, 255, 255, 255),
                bool useAlphaChannelOfTexture = false) {}

            //! Draws a part of the texture into the rectangle.
            virtual void draw2DImage(const video::ITexture* texture, const core::rect<s32>& destRect,
                const core::rect<s32>& sourceRect, const core::rect<s32>* clipRect = 0,
                const video::SColor* const colors = 0, bool useAlphaChannelOfTexture = false) {}

            // rotateType
            // int: 0 - 1 - 2 -3
            // degree: 0 - 90 - 180 - 270
            void draw2DImageRotated(const video::ITexture* texture, const core::rect<s32>& destRect,
                const core::rect<s32>& sourceRect, int rotateType = 0, const core::rect<s32>* clipRect = 0,
                const video::SColor* const colors = 0, bool useAlphaChannelOfTexture = false) {}

            void draw2DImageBatch(const video::ITexture* texture,
                const core::array<core::position2d<s32> >& positions,
                const core::array<core::rect<s32> >& sourceRects,
                const core::rect<s32>* clipRect,
                SColor color,
                bool useAlphaChannelOfTexture) {}

            //! draw an 2d rectangle
            virtual void draw2DRectangle(SColor color, const core::rect<s32>& pos,
                const core::rect<s32>* clip = 0) {}

            //!Draws an 2d rectangle with a gradient.
            virtual void draw2DRectangle(const core::rect<s32>& pos,
                SColor colorLeftUp, SColor colorRightUp, SColor colorLeftDown, SColor colorRightDown,
                const core::rect<s32>* clip = 0) {}

            //! Draws a 2d line.
            virtual void draw2DLine(const core::position2d<s32>& start,
                const core::position2d<s32>& end,
                SColor color = SColor(255, 255, 255, 255)) {}

            //! Draws a single pixel
            virtual void drawPixel(u32 x, u32 y, const SColor & color) {}

            //! Draws a 3d line.
            virtual void draw3DLine(const core::vector3df& start,
                const core::vector3df& end,
                SColor color = SColor(255, 255, 255, 255)) {}

            //! Draws a pixel
            //			virtual void drawPixel(u32 x, u32 y, const SColor & color){}

            //! Returns the name of the video driver.
            virtual const wchar_t* getName() const
            {
                return L"bgfx";
            }

            //! deletes all dynamic lights there are
            virtual void deleteAllDynamicLights() {}

            //! adds a dynamic light
            virtual s32 addDynamicLight(const SLight& light)
            {
                return -1;
            }

            //! Turns a dynamic light on or off
            /** \param lightIndex: the index returned by addDynamicLight
            \param turnOn: true to turn the light on, false to turn it off */
            virtual void turnLightOn(s32 lightIndex, bool turnOn) {}

            //! returns the maximal amount of dynamic lights the device can handle
            virtual u32 getMaximalDynamicLightAmount() const
            {
                return 0;
            }

            //! Returns the maximum texture size supported.
            virtual core::dimension2du getMaxTextureSize() const
            {
                return core::dimension2du();
            }

            //! Draws a shadow volume into the stencil buffer.
            virtual void drawStencilShadowVolume(const core::array<core::vector3df>& triangles, bool zfail, u32 debugDataVisible = 0) _IRR_OVERRIDE_ {}

            //! Fills the stencil shadow with color.
            virtual void drawStencilShadow(bool clearStencilBuffer = false,
                video::SColor leftUpEdge = video::SColor(0, 0, 0, 0),
                video::SColor rightUpEdge = video::SColor(0, 0, 0, 0),
                video::SColor leftDownEdge = video::SColor(0, 0, 0, 0),
                video::SColor rightDownEdge = video::SColor(0, 0, 0, 0)) _IRR_OVERRIDE_ {}

            //! sets a viewport
            virtual void setViewPort(const core::rect<s32>& area)
            {
                bgfx::setViewRect(0, area.UpperLeftCorner.X, area.UpperLeftCorner.Y, area.LowerRightCorner.X, area.LowerRightCorner.Y);
            }

            //! Only used internally by the engine
            virtual void OnResize(const core::dimension2d<u32>& size) 
            {
                bgfx::reset(size.Width, size.Height);
            }

            //! Returns type of video driver
            virtual E_DRIVER_TYPE getDriverType() const
            {
                return EDT_BGFX;
            }

            //! get color format of the current color buffer
            virtual ECOLOR_FORMAT getColorFormat() const
            {
                return ECF_UNKNOWN;
            }

            //! Returns the transformation set by setTransform
            virtual const core::matrix4& getTransform(E_TRANSFORMATION_STATE state) const
            {
                return core::matrix4();
            }

            //! Can be called by an IMaterialRenderer to make its work easier.
            virtual void setBasicRenderStates(const SMaterial& material, const SMaterial& lastmaterial, bool resetAllRenderstates) {}

            //! Compare in SMaterial doesn't check texture parameters, so we should call this on each OnRender call.
            virtual void setTextureRenderStates(const SMaterial& material, bool resetAllRenderstates) {}

            //! Get a vertex shader constant index.
            virtual s32 getVertexShaderConstantID(const c8* name)
            {
                return -1;
            }

            //! Get a pixel shader constant index.
            virtual s32 getPixelShaderConstantID(const c8* name)
            {
                return -1;
            }

            //! Sets a vertex shader constant.
            virtual void setVertexShaderConstant(const f32* data, s32 startRegister, s32 constantAmount = 1) {}

            //! Sets a pixel shader constant.
            virtual void setPixelShaderConstant(const f32* data, s32 startRegister, s32 constantAmount = 1) {}

            //! Sets a constant for the vertex shader based on an index.
            virtual bool setVertexShaderConstant(s32 index, const f32* floats, int count)
            {
                return false;
            }

            //! Int interface for the above.
            virtual bool setVertexShaderConstant(s32 index, const s32* ints, int count)
            {
                return false;
            }

            //! Sets a constant for the pixel shader based on an index.
            virtual bool setPixelShaderConstant(s32 index, const f32* floats, int count)
            {
                return false;
            }

            //! Int interface for the above.
            virtual bool setPixelShaderConstant(s32 index, const s32* ints, int count)
            {
                return false;
            }

            //! Adds a new material renderer to the VideoDriver
            virtual s32 addShaderMaterial(const c8* vertexShaderProgram, const c8* pixelShaderProgram,
                IShaderConstantSetCallBack* callback, E_MATERIAL_TYPE baseMaterial, s32 userData)
            {
                return -1;
            }

            //! Adds a new material renderer to the VideoDriver
            virtual s32 addHighLevelShaderMaterial(
                const c8* vertexShaderProgram,
                const c8* vertexShaderEntryPointName = 0,
                E_VERTEX_SHADER_TYPE vsCompileTarget = EVST_VS_1_1,
                const c8* pixelShaderProgram = 0,
                const c8* pixelShaderEntryPointName = 0,
                E_PIXEL_SHADER_TYPE psCompileTarget = EPST_PS_1_1,
                const c8* geometryShaderProgram = 0,
                const c8* geometryShaderEntryPointName = "main",
                E_GEOMETRY_SHADER_TYPE gsCompileTarget = EGST_GS_4_0,
                scene::E_PRIMITIVE_TYPE inType = scene::EPT_TRIANGLES,
                scene::E_PRIMITIVE_TYPE outType = scene::EPT_TRIANGLE_STRIP,
                u32 verticesOut = 0,
                IShaderConstantSetCallBack* callback = 0,
                E_MATERIAL_TYPE baseMaterial = video::EMT_SOLID,
                s32 userData = 0,
                E_GPU_SHADING_LANGUAGE shadingLang = EGSL_DEFAULT)
            {
                return -1;
            }

            //! Returns pointer to the IGPUProgrammingServices interface.
            virtual IGPUProgrammingServices* getGPUProgrammingServices()
            {
                return this;
            }

            //! Returns a pointer to the IVideoDriver interface.
            virtual IVideoDriver* getVideoDriver()
            {
                return this;
            }

            //! Returns the maximum amount of primitives
            virtual u32 getMaximalPrimitiveCount() const
            {
                return 0;
            }

            virtual ITexture* addRenderTargetTexture(const core::dimension2d<u32>& size,
                const io::path& name, const ECOLOR_FORMAT format = ECF_UNKNOWN)
            {
                return NULL;
            }

            virtual bool setRenderTarget(video::ITexture* texture, bool clearBackBuffer,
                bool clearZBuffer, SColor color)
            {
                return false;
            }

            //! Clears the ZBuffer.
            virtual void clearZBuffer() {}

            //! Returns an image created from the last rendered frame.
            virtual IImage* createScreenShot(video::ECOLOR_FORMAT format = video::ECF_UNKNOWN, video::E_RENDER_TARGET target = video::ERT_FRAME_BUFFER)
            {
                return NULL;
            }

            //! Set/unset a clipping plane.
            virtual bool setClipPlane(u32 index, const core::plane3df& plane, bool enable = false)
            {
                return false;
            }

            //! Enable/disable a clipping plane.
            virtual void enableClipPlane(u32 index, bool enable) {}

            //! Returns the graphics card vendor name.
            virtual core::stringc getVendorInfo()
            {
                return "bgfx";
            }

            // returns the current size of the screen or rendertarget
            virtual const core::dimension2d<u32>& getCurrentRenderTargetSize() const
            {
                return core::dimension2d<u32>();
            }

        private:
            //! returns a device dependent texture from a software surface (IImage)
            virtual ITexture* createDeviceDependentTexture(IImage* surface, const io::path& name, void* mipmapData)
            {
                return NULL;
            }

            //! returns a device dependent texture from a software surface (IImage)
            virtual ITexture* createDeviceDependentTextureCube(const io::path& name, IImage* posXImage, IImage* negXImage,
                IImage* posYImage, IImage* negYImage, IImage* posZImage, IImage* negZImage)
            {
                return NULL;
            }
        };

        IVideoDriver* createBgfxDriver(const SIrrlichtCreationParameters& params,
            io::IFileSystem* io
#if defined(_IRR_COMPILE_WITH_X11_DEVICE_) || defined(_IRR_WINDOWS_API_) || defined(_IRR_COMPILE_WITH_ANDROID_DEVICE_) || defined(_IRR_COMPILE_WITH_FB_DEVICE_)
            , IContextManager* contextManager
#elif defined(_IRR_COMPILE_WITH_IPHONE_DEVICE_)
            , CIrrDeviceIPhone* device
#endif
            )
        {
#ifdef _IRR_COMPILE_WITH_BGFX_
            return new CBgfxDriver(params, io
#if defined(_IRR_COMPILE_WITH_X11_DEVICE_) || defined(_IRR_WINDOWS_API_) || defined(_IRR_COMPILE_WITH_ANDROID_DEVICE_) || defined(_IRR_COMPILE_WITH_FB_DEVICE_)
                , contextManager
#elif defined(_IRR_COMPILE_WITH_IPHONE_DEVICE_)
                , device
#endif
                );
#else
            return 0;
#endif //  _IRR_COMPILE_WITH_BGFX_
        }
    } // end namespace video
} // end namespace irr

#endif // _IRR_COMPILE_WITH_OPENGL_

#endif
