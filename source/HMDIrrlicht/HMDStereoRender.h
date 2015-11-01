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

#include <irrlicht.h>

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
    HMDStereoRender(irr::scene::ISceneManager* smgr, irr::video::IVideoDriver* driver, irr::ITimer* timer, HMDDescriptor HMD, irr::f32 worldScale = 1.0);
    ~HMDStereoRender();

    HMDDescriptor getHMD();
    void setHMD(HMDDescriptor HMD);

    irr::f32 getWorldScale();
    void setWorldScale(irr::f32 worldScale);

    void drawAll();

private:
    irr::video::IVideoDriver* _driver;
    irr::video::ITexture* _renderTexture;
    irr::scene::ISceneManager* _smgr;

    HMDDescriptor _HMD;
    irr::f32 _worldScale;
    irr::core::matrix4 _projectionLeft;
    irr::core::matrix4 _projectionRight;
    irr::f32 _eyeSeparation;
    irr::f32 _lensShift;

    irr::core::rect<irr::s32> _viewportLeft;
    irr::core::rect<irr::s32> _viewportRight;

    irr::scene::ICameraSceneNode* _pCamera;

    irr::video::SMaterial _renderMaterial;
    irr::video::S3DVertex _planeVertices[4];
    irr::u16 _planeIndices[6];
    irr::ITimer* _timer;

    class OculusDistorsionCallback : public irr::video::IShaderConstantSetCallBack
    {
    public:
        irr::f32 scale[2];
        irr::f32 scaleIn[2];
        irr::f32 lensCenter[2];
        irr::f32 hmdWarpParam[4];
        virtual void OnSetConstants(irr::video::IMaterialRendererServices* services, irr::s32 userData)
        {
            irr::video::IVideoDriver* driver = services->getVideoDriver();
            services->setPixelShaderConstant("scale", scale, 2);
            services->setPixelShaderConstant("scaleIn", scaleIn, 2);
            services->setPixelShaderConstant("lensCenter", lensCenter, 2);
            services->setPixelShaderConstant("hmdWarpParam", hmdWarpParam, 4);
        }
    };
    OculusDistorsionCallback _distortionCB;
};
#endif
