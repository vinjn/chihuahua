/*
Copyright (C) 2012 Luca Siciliano

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation
files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy,
modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies
or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT
OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef __HMD_STEREO_RENDER__
#define __HMD_STEREO_RENDER__

#include "irrlicht.h"

struct HMDDescriptor
{
    int hResolution;
    int vResolution;
    float hScreenSize;
    float vScreenSize;
    float interpupillaryDistance;
    float lensSeparationDistance;
    float eyeToScreenDistance;
    float distortionK[4];
};

class HMDStereoRender
{
public:
    IRRLICHT_API HMDStereoRender(ue::scene::ISceneManager* smgr, ue::video::IVideoDriver* driver, ue::ITimer* timer, HMDDescriptor HMD, ue::f32 worldScale = 1.0);
    IRRLICHT_API ~HMDStereoRender();

    IRRLICHT_API HMDDescriptor getHMD();
    IRRLICHT_API void setHMD(HMDDescriptor HMD);

    IRRLICHT_API ue::f32 getWorldScale();
    IRRLICHT_API void setWorldScale(ue::f32 worldScale);

    IRRLICHT_API void drawAll();

private:
    ue::video::IVideoDriver* _driver;
    ue::video::ITexture* _renderTexture;
    ue::scene::ISceneManager* _smgr;

    HMDDescriptor _HMD;
    ue::f32 _worldScale;
    ue::core::matrix4 _projectionLeft;
    ue::core::matrix4 _projectionRight;
    ue::f32 _eyeSeparation;
    ue::f32 _lensShift;

    ue::core::rect<ue::s32> _viewportLeft;
    ue::core::rect<ue::s32> _viewportRight;

    ue::scene::ICameraSceneNode* _pCamera;

    ue::video::SMaterial _renderMaterial;
    ue::video::S3DVertex _planeVertices[4];
    ue::u16 _planeIndices[6];
    ue::ITimer* _timer;

    class OculusDistorsionCallback : public ue::video::IShaderConstantSetCallBack
    {
    public:
        ue::f32 scale[2];
        ue::f32 scaleIn[2];
        ue::f32 lensCenter[2];
        ue::f32 hmdWarpParam[4];
        virtual void OnSetConstants(ue::video::IMaterialRendererServices* services, ue::s32 userData)
        {
//            ue::video::IVideoDriver* driver = services->getVideoDriver();
            services->setPixelShaderConstant("scale", scale, 2);
            services->setPixelShaderConstant("scaleIn", scaleIn, 2);
            services->setPixelShaderConstant("lensCenter", lensCenter, 2);
            services->setPixelShaderConstant("hmdWarpParam", hmdWarpParam, 4);
        }
    };
    OculusDistorsionCallback _distortionCB;
};
#endif
