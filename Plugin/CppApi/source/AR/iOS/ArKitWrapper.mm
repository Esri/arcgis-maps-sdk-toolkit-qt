/*******************************************************************************
 *  Copyright 2012-2018 Esri
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 ******************************************************************************/

#include "ArKitWrapper.h"
#import <ARKit/ARKit.h>
#include <QMatrix4x4>
#include "ArcGISArViewInterface.h"
#include <QThread>
#include <array>

using namespace Esri::ArcGISRuntime;
using namespace Esri::ArcGISRuntime::Toolkit;

// Wrapp the AR Kit
//
// The rendering code is based on the code example given in the ARKit documentation:
// https://developer.apple.com/documentation/arkit/displaying_an_ar_experience_with_metal?language=objc
//
// https://stackoverflow.com/questions/32850012/what-is-the-most-efficient-way-to-display-cvimagebufferref-on-ios
// https://discussions.apple.com/thread/2597309

/*******************************************************************************
 ******************* Objective-C class declarations ****************************
 ******************************************************************************/

@interface ArcGISArSessionDelegate : NSObject<ARSessionDelegate>

-(id) init;
-(void) session: (ARSession*)session didUpdateFrame:(ARFrame*)frame;
-(void) copyPixelBuffers: (CVImageBufferRef)pixelBuffer;
-(std::array<double, 7>) lastQuaternionTranslation: (simd_float4x4)cameraTransform;
-(std::array<double, 6>) lastLensIntrinsics: (ARCamera*)camera;

@property (nonatomic) ArcGISArViewInterface* arcGISArView;
@property (nonatomic) uchar* textureDataY;
@property (nonatomic) uchar* textureDataCbCr;
@property (nonatomic) size_t widthY;
@property (nonatomic) size_t widthCbCr;
@property (nonatomic) size_t heightY;
@property (nonatomic) size_t heightCbCr;
@property (nonatomic) bool render_in_progress;

@end

/*******************************************************************************
 ******************* Objective-C class implementations *************************
 ******************************************************************************/

@implementation ArcGISArSessionDelegate

-(id)init
{
  if (self = [super init])
  {
    self.arcGISArView = nullptr;
    self.textureDataY = nullptr;
    self.textureDataCbCr = nullptr;
    self.widthY = 0;
    self.widthCbCr = 0;
    self.heightY = 0;
    self.heightCbCr = 0;
    self.render_in_progress = false;
  }
  return self;
}

-(void)session: (ARSession*)session didUpdateFrame:(ARFrame*)frame
{
  if (!frame || self.render_in_progress)
    return;

  // don't try to render an new frame if there is a rendering in progress
  self.render_in_progress = true;

  // release data
  if (self.textureDataY)
  {
    free(self.textureDataY);
    self.textureDataY = nullptr;
  }

  if (self.textureDataCbCr)
  {
    free(self.textureDataCbCr);
    self.textureDataCbCr = nullptr;
  }

  [self copyPixelBuffers: frame.capturedImage];

  // render the AR frame
  self.arcGISArView->update();

  // update the scene view camera
  auto camera = [self lastQuaternionTranslation: frame.camera.transform];
  self.arcGISArView->updateCamera(camera[0], camera[1], camera[2], camera[3], camera[4], camera[5], camera[6]);

  // udapte the field of view, based on the
  auto lens = [self lastLensIntrinsics: frame.camera];
  self.arcGISArView->updateFieldOfView(lens[0], lens[1], lens[2], lens[3], lens[4], lens[5]);

  // render the frame of the ArcGIS runtime
  self.arcGISArView->renderFrame();
}

// The first texture
// https://en.wikipedia.org/wiki/YCbCr
// 4:2:0

-(void)copyPixelBuffers: (CVImageBufferRef)pixelBuffer
{
  // map
  CVPixelBufferRetain(pixelBuffer); // retains the new PB
  CVPixelBufferLockBaseAddress(pixelBuffer, kCVPixelBufferLock_ReadOnly);

  // create buffers
  uchar* dataY = static_cast<uchar*>(CVPixelBufferGetBaseAddressOfPlane(pixelBuffer, 0));
  uchar* dataCbCr = static_cast<uchar*>(CVPixelBufferGetBaseAddressOfPlane(pixelBuffer, 1));

  self.widthY = CVPixelBufferGetWidthOfPlane(pixelBuffer, 0);
  self.widthCbCr = CVPixelBufferGetWidthOfPlane(pixelBuffer, 1);

  self.heightY = CVPixelBufferGetHeightOfPlane(pixelBuffer, 0);
  self.heightCbCr = CVPixelBufferGetHeightOfPlane(pixelBuffer, 1);

  const size_t bytesPerRow_0 = CVPixelBufferGetBytesPerRowOfPlane(pixelBuffer, 0);
  const size_t bytesPerRow_1 = CVPixelBufferGetBytesPerRowOfPlane(pixelBuffer, 1);

  const size_t size_0 = self.heightY * bytesPerRow_0;
  const size_t size_1 = self.heightCbCr * bytesPerRow_1;

  self.textureDataY = static_cast<uchar*>(malloc(size_0));
  self.textureDataCbCr = static_cast<uchar*>(malloc(size_1));

  memcpy(self.textureDataY, dataY, size_0);
  memcpy(self.textureDataCbCr, dataCbCr, size_1);

  // unmap
  CVPixelBufferUnlockBaseAddress(pixelBuffer, kCVPixelBufferLock_ReadOnly);
  CVPixelBufferRelease(pixelBuffer);
}

-(std::array<double, 7>) lastQuaternionTranslation: (simd_float4x4)cameraTransform
{
  // uses float not double. How to convert simd_float4x4 to simd_double4x4?

  // A quaternion used to compensate for the pitch being 90 degrees on `ARKit`; used to calculate the current
  // device transformation for each frame.
  simd_quatf compensationQuat = { simd_float4 { 0.70710678118, 0.0, 0.0, 0.70710678118 }};

  // Calculate our final quaternion and create the new transformation matrix.
  simd_quatf finalQuat = simd_mul(compensationQuat, simd_quaternion(cameraTransform));

  const float translationTransformationFactor = 1.0;

  return { finalQuat.vector.x,
        finalQuat.vector.y,
        finalQuat.vector.z,
        finalQuat.vector.w,
        (cameraTransform.columns[3].x) * translationTransformationFactor,
        (-cameraTransform.columns[3].z) * translationTransformationFactor,
        (cameraTransform.columns[3].y) * translationTransformationFactor
  };
}

-(std::array<double, 6>) lastLensIntrinsics: (ARCamera*)camera
{
  auto intrinsics = camera.intrinsics;
  auto imageResolution = camera.imageResolution;

  return {
    intrinsics.columns[0][0],
    intrinsics.columns[1][1],
    intrinsics.columns[2][0],
    intrinsics.columns[2][1],
    imageResolution.width,
    imageResolution.height
  };
}

@end

/*******************************************************************************
 ******************** C++ private class implementation *************************
 ******************************************************************************/

struct ArKitWrapper::ArKitWrapperPrivate {
  ARSession* arSession = nullptr;
  ARWorldTrackingConfiguration* arConfiguration = nullptr;
  ArcGISArSessionDelegate* arSessionDelegate = nullptr;
};

/*******************************************************************************
 ******************** C++ public class implementation **************************
 ******************************************************************************/

//TODO: test performances with ARCNView from AR kit. Integration of the ARCNView in Qt?

ArKitWrapper::ArKitWrapper(ArcGISArViewInterface* arcGISArView) :
  m_impl(new ArKitWrapperPrivate),
  m_arKitPointCloudRenderer(this),
  m_textureY(QOpenGLTexture::Target2D),
  m_textureCbCr(QOpenGLTexture::Target2D)
{
  // Create an AR session
  m_impl->arSession = [[ARSession alloc] init];

  // Create an AR session configuration
  m_impl->arConfiguration = [ARWorldTrackingConfiguration new];
  m_impl->arConfiguration.worldAlignment = ARWorldAlignmentGravityAndHeading;

  // delegate to get the frames
  m_impl->arSessionDelegate = [[ArcGISArSessionDelegate alloc]init];
  m_impl->arSessionDelegate.arcGISArView = arcGISArView;
  m_impl->arSession.delegate = m_impl->arSessionDelegate;

  // Run the view's session
  [m_impl->arSession runWithConfiguration:m_impl->arConfiguration];
}

ArKitWrapper::~ArKitWrapper()
{
  Q_CHECK_PTR(m_impl);
  [m_impl->arSessionDelegate release];
  delete m_impl;
}

void ArKitWrapper::startTracking()
{
  // Not implemented.
}

void ArKitWrapper::stopTracking()
{
  // Not implemented.
}

QSizeF ArKitWrapper::size() const
{
  // Not implemented.
  return QSize();
}

void ArKitWrapper::setSize(const QSizeF& /*size*/)
{
  // Not implemented.
}

// this function run on the rendering thread
void ArKitWrapper::initGL()
{
  m_arKitFrameRenderer.initGL();
  // m_arKitPointCloudRenderer.initGL(); // for debugging the AR tracking
}

// this function run on the rendering thread
void ArKitWrapper::beforeRendering()
{
  // create textures ids
  if (m_textureY.isCreated())
    m_textureY.destroy();

  if (m_textureCbCr.isCreated())
    m_textureCbCr.destroy();

  m_textureY.setSize(static_cast<int>(m_impl->arSessionDelegate.widthY),
                     static_cast<int>(m_impl->arSessionDelegate.heightY));
  m_textureCbCr.setSize(static_cast<int>(m_impl->arSessionDelegate.widthCbCr),
                        static_cast<int>(m_impl->arSessionDelegate.heightCbCr));

  m_textureY.setFormat(QOpenGLTexture::R8_UNorm);
  m_textureCbCr.setFormat(QOpenGLTexture::RG8_UNorm);

  m_textureY.allocateStorage();
  m_textureCbCr.allocateStorage();

  m_textureY.setData(QOpenGLTexture::Red, QOpenGLTexture::UInt8, m_impl->arSessionDelegate.textureDataY);
  m_textureCbCr.setData(QOpenGLTexture::RG, QOpenGLTexture::UInt8, m_impl->arSessionDelegate.textureDataCbCr);
}

// this function run on the rendering thread
void ArKitWrapper::afterRendering()
{
  m_impl->arSessionDelegate.render_in_progress = false;
}

// this function run on the rendering thread
void ArKitWrapper::render()
{
  beforeRendering();

  if (m_textureY.textureId() != 0 && m_textureCbCr.textureId() != 0)
  {
    m_arKitFrameRenderer.render(m_textureY.textureId(), m_textureCbCr.textureId());
    // m_arKitPointCloudRenderer.render(); // for debugging the AR tracking
  }

  afterRendering();
}

float* ArKitWrapper::modelViewProjectionData() const
{
  // Not implemented.
  return nullptr;
}

const float* ArKitWrapper::pointCloudData() const
{
  // Not implemented.
  return nullptr;
}

int32_t ArKitWrapper::pointCloudSize() const
{
  // Not implemented.
  return 0;
}
