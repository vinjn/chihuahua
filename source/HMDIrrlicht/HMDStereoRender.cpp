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

#include "HMDStereoRender.h"

#include <iostream>
#include <cassert>

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace std;

static const char* vertexShader = R"(
attribute vec3 inVertexPosition;
attribute vec2 inTexCoord0;
varying vec2 vTextureCoord0;

void main() {
  gl_Position = vec4(inVertexPosition.xy, 0.0, 1.0);
  vTextureCoord0 = inTexCoord0;
}
)";

static const char *fragShader = R"(
uniform vec2 scale;
uniform vec2 scaleIn;
uniform vec2 lensCenter;
uniform vec4 hmdWarpParam;
uniform sampler2D texid;
varying vec2 vTextureCoord0;
void main()
{
  vec2 uv = (vTextureCoord0*2.0)-1.0; // range from [0,1] to [-1,1]
  vec2 theta = (uv-lensCenter)*scaleIn;
  float rSq = theta.x*theta.x + theta.y*theta.y;
  vec2 rvector = theta*(hmdWarpParam.x + hmdWarpParam.y*rSq + hmdWarpParam.z*rSq*rSq + hmdWarpParam.w*rSq*rSq*rSq);
  vec2 tc = (lensCenter + scale * rvector);
  tc = (tc+1.0)/2.0; // range from [-1,1] to [0,1]
  if (any(bvec2(clamp(tc, vec2(0.0,0.0), vec2(1.0,1.0))-tc)))
    gl_FragColor = vec4(0.0, 0.0, 0.0, 1.0);
  else
    gl_FragColor = texture2D(texid, tc);
}
)";

HMDStereoRender::HMDStereoRender(irr::scene::ISceneManager* smgr, irr::video::IVideoDriver* driver, irr::ITimer* timer, 
    HMDDescriptor HMD, f32 worldScale)
: 
_renderTexture(NULL)
{
    _smgr = smgr;
    _driver = driver;
    _timer = timer;
    _worldScale = worldScale;

    // Create perspectiva camera used for rendering
    _pCamera = _smgr->addCameraSceneNode();

    // Init shader parameters
    _distortionCB.scale[0] = 1.0f; _distortionCB.scale[1] = 1.0f;
    _distortionCB.scaleIn[0] = 1.0f; _distortionCB.scaleIn[1] = 1.0f;
    _distortionCB.lensCenter[0] = 0.0f; _distortionCB.lensCenter[1] = 0.0f;
    _distortionCB.hmdWarpParam[0] = 1.0f; _distortionCB.hmdWarpParam[1] = 0.0f; _distortionCB.hmdWarpParam[2] = 0.0f; _distortionCB.hmdWarpParam[3] = 0.0f;

    // Plane
    _planeVertices[0] = video::S3DVertex(-1.0f, -1.0f, 1.0f, 1, 1, 0, video::SColor(255, 0, 255, 255), 0.0f, 0.0f);
    _planeVertices[1] = video::S3DVertex(-1.0f, 1.0f, 1.0f, 1, 1, 0, video::SColor(255, 0, 255, 255), 0.0f, 1.0f);
    _planeVertices[2] = video::S3DVertex(1.0f, 1.0f, 1.0f, 1, 1, 0, video::SColor(255, 0, 255, 255), 1.0f, 1.0f);
    _planeVertices[3] = video::S3DVertex(1.0f, -1.0f, 1.0f, 1, 1, 0, video::SColor(255, 0, 255, 255), 1.0f, 0.0f);
    _planeIndices[0] = 0; _planeIndices[1] = 1; _planeIndices[2] = 2; _planeIndices[3] = 0; _planeIndices[4] = 2; _planeIndices[5] = 3;

    // Create shader material
    _renderMaterial.Wireframe = false;
    _renderMaterial.Lighting = false;
    _renderMaterial.TextureLayer[0].TextureWrapU = ETC_CLAMP;
    _renderMaterial.TextureLayer[0].TextureWrapV = ETC_CLAMP;

    IGPUProgrammingServices* gpu = _driver->getGPUProgrammingServices();
    _renderMaterial.MaterialType = (E_MATERIAL_TYPE)gpu->addHighLevelShaderMaterial(vertexShader, "main", EVST_VS_2_0, fragShader, "main", EPST_PS_2_0, &_distortionCB);

    setHMD(HMD);
}

HMDStereoRender::~HMDStereoRender() {
}

void HMDStereoRender::setHMD(HMDDescriptor HMD) {
    _HMD = HMD;

    // Compute aspect ratio and FOV
    float aspect = HMD.hResolution / (2.0f*HMD.vResolution);

    // Fov is normally computed with:
    //   2*atan2(HMD.vScreenSize,2*HMD.eyeToScreenDistance)
    // But with lens distortion it is increased (see Oculus SDK Documentation)
    float r = -1.0f - (4.0f * (HMD.hScreenSize / 4.0f - HMD.lensSeparationDistance / 2.0f) / HMD.hScreenSize);
    float distScale = (HMD.distortionK[0] + HMD.distortionK[1] * pow(r, 2) + HMD.distortionK[2] * pow(r, 4) + HMD.distortionK[3] * pow(r, 6));
    float fov = 2.0f*atan2(HMD.vScreenSize*distScale, 2.0f*HMD.eyeToScreenDistance);

    // Compute camera projection matrices
    matrix4 centerProjection = matrix4().buildProjectionMatrixPerspectiveFovLH(fov, aspect, 1, 10000);
    float h = 4 * (HMD.hScreenSize / 4 - HMD.interpupillaryDistance / 2) / HMD.hScreenSize;
    _projectionLeft = matrix4().setTranslation(vector3df(h, 0.0, 0.0)) * centerProjection;
    _projectionRight = matrix4().setTranslation(vector3df(-h, 0.0, 0.0)) * centerProjection;

    // Compute camera offset
    _eyeSeparation = _worldScale * HMD.interpupillaryDistance / 2.0f;

    // Compute Viewport
    _viewportLeft = rect<s32>(0, 0, HMD.hResolution / 2, HMD.vResolution);
    _viewportRight = rect<s32>(HMD.hResolution / 2, 0, HMD.hResolution, HMD.vResolution);

    // Distortion shader parameters
    _lensShift = 4.0f * (HMD.hScreenSize / 4.0f - HMD.lensSeparationDistance / 2.0f) / HMD.hScreenSize;

    _distortionCB.scale[0] = 1.0f / distScale;
    _distortionCB.scale[1] = 1.0f*aspect / distScale;

    _distortionCB.scaleIn[0] = 1.0f;
    _distortionCB.scaleIn[1] = 1.0f / aspect;

    _distortionCB.hmdWarpParam[0] = HMD.distortionK[0];
    _distortionCB.hmdWarpParam[1] = HMD.distortionK[1];
    _distortionCB.hmdWarpParam[2] = HMD.distortionK[2];
    _distortionCB.hmdWarpParam[3] = HMD.distortionK[3];

    // Create render target
    if (_driver->queryFeature(video::EVDF_RENDER_TO_TARGET))
    {
        if (_renderTexture != NULL) _renderTexture->drop();
        _renderTexture = _driver->addRenderTargetTexture(dimension2d<u32>(HMD.hResolution*distScale / 2.0f, HMD.vResolution*distScale));
        _renderMaterial.setTexture(0, _renderTexture);
    }
    else {
        // Render to target not supported
        assert(0);
    }
}


void HMDStereoRender::drawAll() {

    ICameraSceneNode* camera = _smgr->getActiveCamera();
    camera->OnAnimate(_timer->getTime());

    auto oldViewPort = _driver->getViewPort();

    // Render Left
    _driver->setRenderTarget(_renderTexture, true, true, video::SColor(0, 0, 0, 0));

    _pCamera->setProjectionMatrix(_projectionLeft);

    vector3df r = camera->getRotation();
    vector3df tx(-_eyeSeparation, 0.0, 0.0);
    tx.rotateXZBy(-r.Y);
    tx.rotateYZBy(-r.X);
    tx.rotateXYBy(-r.Z);

    _pCamera->setPosition(camera->getPosition() + tx);
    _pCamera->setTarget(camera->getTarget() + tx);

    _smgr->setActiveCamera(_pCamera);
    _smgr->drawAll();

    _driver->setRenderTarget(0, false, false, video::SColor(0, 100, 100, 100));
    _driver->setViewPort(_viewportLeft);

    _distortionCB.lensCenter[0] = _lensShift;

    _driver->setMaterial(_renderMaterial);
    _driver->drawIndexedTriangleList(_planeVertices, 4, _planeIndices, 2);

    // Render Right
    _driver->setRenderTarget(_renderTexture, true, true, video::SColor(0, 0, 0, 0));
    _pCamera->setProjectionMatrix(_projectionRight);

    vector3df r2 = camera->getRotation();
    vector3df tx2(-_eyeSeparation, 0.0, 0.0);
    tx.rotateXZBy(-r2.Y);
    tx.rotateYZBy(-r2.X);
    tx.rotateXYBy(-r2.Z);

    _pCamera->setPosition(camera->getPosition() + tx2);
    _pCamera->setTarget(camera->getTarget() + tx2);

    _smgr->drawAll();

    _driver->setRenderTarget(0, false, false, video::SColor(0, 100, 100, 100));
    _driver->setViewPort(_viewportRight);

    _distortionCB.lensCenter[0] = -_lensShift;

    _driver->setMaterial(_renderMaterial);
    _driver->drawIndexedTriangleList(_planeVertices, 4, _planeIndices, 2);

    _smgr->setActiveCamera(camera);
    _driver->setViewPort(oldViewPort);
}
