/*******************************************************************************
 *  Copyright 2012-2019 Esri
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
#include "ArcGISArViewInterface.h"
#include "ArKitUtils.h"
#include "ArKitFrameRenderer.h"
#include "ArKitPlaneRenderer.h"
#include "ArKitPointCloudRenderer.h"

// Qt headers
#include <QMatrix4x4>
#include <QGuiApplication>
#include <QScreen>

// C++ header
#include <array>

// ARCore header
#import <ARKit/ARKit.h>

using namespace Esri::ArcGISRuntime::Toolkit;
using namespace Esri::ArcGISRuntime::Toolkit::Internal;

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

- (void) session: (ARSession*) session didUpdateFrame: (ARFrame*) frame;
- (void) session: (ARSession*) session cameraDidChangeTrackingState: (ARCamera*) camera;
- (void) session: (ARSession*) session didFailWithError: (NSError*) error;

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
@property (nonatomic) size_t sizeY; // used to determines if the raw array need to be reallocated.
@property (nonatomic) size_t sizeCbCr; // used to determines if the raw array need to be reallocated.
@property (nonatomic) bool textureDataUsed;
@property (nonatomic) NSTimeInterval timestamp;

@end

/*******************************************************************************
 ******************* Objective-C class implementations *************************
 ******************************************************************************/

@implementation ArcGISArSessionDelegate

-(id) init
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
    self.textureDataUsed = false;
  }
  return self;
}

- (void) session: (ARSession*) session didUpdateFrame: (ARFrame*) frame
{
  // copy the texture data is not used.
  if (!self.textureDataUsed)
  {
    [self copyPixelBuffers: frame.capturedImage];
  }

  // render the AR frame
  self.arcGISArView->update();

  // update the scene view camera
  auto camera = [self lastQuaternionTranslation: frame.camera.transform];
  self.arcGISArView->setTransformationMatrixInternal(camera[0], camera[1], camera[2], camera[3], camera[4], camera[5], camera[6]);

  // udapte the field of view, based on the
  auto lens = [self lastLensIntrinsics: frame.camera];
  self.arcGISArView->setFieldOfViewInternal(lens[0], lens[1], lens[2], lens[3], lens[4], lens[5]);

  // render the frame of the ArcGIS runtime
  self.arcGISArView->renderFrameInternal();
}

- (void) session: (ARSession*) session cameraDidChangeTrackingState: (ARCamera*) camera
{
  // not implemented.
}

- (void) session: (ARSession*) session didFailWithError: (NSError*) error
{
  // not implemented.
}

-(void) copyPixelBuffers: (CVImageBufferRef)pixelBuffer
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

  const size_t bytesPerRowY = CVPixelBufferGetBytesPerRowOfPlane(pixelBuffer, 0);
  const size_t bytesPerRowCbCr = CVPixelBufferGetBytesPerRowOfPlane(pixelBuffer, 1);

  const size_t sizeY = self.heightY * bytesPerRowY;
  const size_t sizeCbCr = self.heightCbCr * bytesPerRowCbCr;

  // reallocate if the size was changed
  if (sizeY != self.sizeY)
  {
    free(self.textureDataY);
    self.textureDataY = static_cast<uchar*>(malloc(sizeY));
    self.sizeY = sizeY;
  }

  if (sizeCbCr != self.sizeCbCr)
  {
    free(self.textureDataCbCr);
    self.textureDataCbCr = static_cast<uchar*>(malloc(sizeCbCr));
    self.sizeCbCr = sizeCbCr;
  }

  // copy the data from the texture data
  memcpy(self.textureDataY, dataY, sizeY);
  memcpy(self.textureDataCbCr, dataCbCr, sizeCbCr);

  // don't try to use the texture data until the last texture was displayed.
  self.textureDataUsed = true;

  // unmap
  CVPixelBufferUnlockBaseAddress(pixelBuffer, kCVPixelBufferLock_ReadOnly);
  CVPixelBufferRelease(pixelBuffer);
}

-(std::array<double, 7>) lastQuaternionTranslation: (simd_float4x4)cameraTransform
{
  simd_quatf finalQuat = simd_quaternion(cameraTransform);

  // get the screen orientation
  const Qt::ScreenOrientations orientation = QGuiApplication::screens().front()->orientation();

  switch (orientation)
  {
    case Qt::PortraitOrientation:
    {
      const simd_quatf orientationQuat = { simd_float4 { 0.0, 0.0, -0.70710678118, -0.70710678118 }};
      finalQuat = simd_mul(finalQuat, orientationQuat);
      break;
    }
    case Qt::LandscapeOrientation:
      // do nothing
      break;
    case Qt::InvertedPortraitOrientation:
    {
      const simd_quatf orientationQuat = { simd_float4 { 0.0, 0.0, -0.70710678118, 0.70710678118 }};
      finalQuat = simd_mul(finalQuat, orientationQuat);
      break;
    }
    case Qt::InvertedLandscapeOrientation:
    {
      const simd_quatf orientationQuat = { simd_float4 { 0.0, 0.0, 0.70710678118, 0.70710678118 }};
      finalQuat = simd_mul(finalQuat, orientationQuat);
      finalQuat = simd_mul(finalQuat, orientationQuat); // 2 rotations of 90 to do a 180 rotation
      break;
    }
    default:
      break;
  }

  return {
    finalQuat.vector.x,
    finalQuat.vector.y,
    finalQuat.vector.z,
    finalQuat.vector.w,
    cameraTransform.columns[3].x,
    cameraTransform.columns[3].y,
    cameraTransform.columns[3].z
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

ArKitWrapper::ArKitWrapper(ArcGISArViewInterface* arcGISArView) :
  m_impl(new ArKitWrapperPrivate),
  m_arKitFrameRenderer(new ArKitFrameRenderer)
{
  // Create an AR session configuration
  m_impl->arConfiguration = [[ARWorldTrackingConfiguration alloc] init];
  m_impl->arConfiguration.worldAlignment = ARWorldAlignmentGravityAndHeading;
  m_impl->arConfiguration.planeDetection = ARPlaneDetectionHorizontal;

  // do nothing if the device doesn't support AR feature.
  if (!ARWorldTrackingConfiguration.isSupported)
  {
    [m_impl->arConfiguration release];
    m_impl->arConfiguration = nullptr;
    return;
  }

  // Create an AR session
  m_impl->arSession = [[ARSession alloc] init];

  // delegate to get the frames
  m_impl->arSessionDelegate = [[ArcGISArSessionDelegate alloc]init];
  m_impl->arSessionDelegate.arcGISArView = arcGISArView;
  m_impl->arSession.delegate = m_impl->arSessionDelegate;

  // Run the view's session
  [m_impl->arSession runWithConfiguration:m_impl->arConfiguration options: ARSessionRunOptionResetTracking];
}

ArKitWrapper::~ArKitWrapper()
{
  Q_CHECK_PTR(m_impl);
  [m_impl->arConfiguration release];
  [m_impl->arSessionDelegate release];
  [m_impl->arSession release];
}

void ArKitWrapper::startTracking()
{
  Q_CHECK_PTR(m_impl);
  [m_impl->arSession runWithConfiguration:m_impl->arConfiguration options: ARSessionRunOptionResetTracking];
}

void ArKitWrapper::stopTracking()
{
  Q_CHECK_PTR(m_impl);
  [m_impl->arSession pause];
}

void ArKitWrapper::resetTracking()
{
  Q_CHECK_PTR(m_impl);
  [m_impl->arSession runWithConfiguration:m_impl->arConfiguration];
}

void ArKitWrapper::setSize(const QSizeF& size)
{
  m_screenSize = size;
  Q_CHECK_PTR(m_arKitFrameRenderer);
  m_arKitFrameRenderer->setSize(size);
}

// this function run on the rendering thread
void ArKitWrapper::initGL()
{
  Q_CHECK_PTR(m_arKitFrameRenderer);
  m_arKitFrameRenderer->initGL();

  if (m_arKitPlaneRenderer)
    m_arKitPlaneRenderer->initGL();

  if (m_arKitPointCloudRenderer)
    m_arKitPointCloudRenderer->initGL();
}

// this function run on the rendering thread
void ArKitWrapper::updateTextures()
{
  Q_CHECK_PTR(m_arKitFrameRenderer);
  m_arKitFrameRenderer->updateTextreDataY(static_cast<int>(m_impl->arSessionDelegate.widthY),
                                          static_cast<int>(m_impl->arSessionDelegate.heightY),
                                          m_impl->arSessionDelegate.textureDataY);

  m_arKitFrameRenderer->updateTextreDataCbCr(static_cast<int>(m_impl->arSessionDelegate.widthCbCr),
                                             static_cast<int>(m_impl->arSessionDelegate.heightCbCr),
                                             m_impl->arSessionDelegate.textureDataCbCr);

  Q_CHECK_PTR(m_impl);
  m_impl->arSessionDelegate.textureDataUsed = false; // now, the texture data can be reused.
}

// this function run on the rendering thread
void ArKitWrapper::render()
{
  // updates the texture datas
  updateTextures();

  // render the AR frame
  Q_CHECK_PTR(m_arKitFrameRenderer);
  m_arKitFrameRenderer->render();

  if (m_arKitPlaneRenderer)
    m_arKitPlaneRenderer->render();

  if (m_arKitPointCloudRenderer)
    m_arKitPointCloudRenderer->render();
}

bool ArKitWrapper::renderVideoFeed() const
{
  return m_renderVideoFeed;
}

void ArKitWrapper::setRenderVideoFeed(bool renderVideoFeed)
{
  m_renderVideoFeed = renderVideoFeed;
}

// properties for debug mode
QColor ArKitWrapper::pointCloudColor() const
{
  if (m_arKitPointCloudRenderer)
    return m_arKitPointCloudRenderer->pointCloudColor();
  return QColor();
}

void ArKitWrapper::setPointCloudColor(const QColor& pointCloudColor)
{
  if (pointCloudColor.isValid())
  {
    if (!m_arKitPointCloudRenderer)
    {
      m_arKitPointCloudRenderer.reset(new ArKitPointCloudRenderer(this));
      m_arKitPointCloudRenderer->initGL();
    }

    m_arKitPointCloudRenderer->setPointCloudColor(pointCloudColor);
  }
  else
  {
    m_arKitPointCloudRenderer.reset();
  }
}

int ArKitWrapper::pointCloudSize() const
{
  if (m_arKitPointCloudRenderer)
    return m_arKitPointCloudRenderer->pointCloudSize();
  return -1;
}

void ArKitWrapper::setPointCloudSize(int pointCloudSize)
{
  if (pointCloudSize > 0)
  {
    if (!m_arKitPointCloudRenderer)
    {
      m_arKitPointCloudRenderer.reset(new ArKitPointCloudRenderer(this));
      m_arKitPointCloudRenderer->initGL();
    }

    m_arKitPointCloudRenderer->setPointCloudSize(pointCloudSize);
  }
  else
  {
    m_arKitPointCloudRenderer.reset();
  }
}

QColor ArKitWrapper::planeColor() const
{
  // not implemented
  return QColor();
}

void ArKitWrapper::setPlaneColor(const QColor&)
{
  // not implemented
}

// doc: https://developer.apple.com/documentation/arkit/arframe/2875718-hittest?language=objc
std::array<double, 7> ArKitWrapper::hitTest(int x, int y) const
{
  // normalize the screen coordinates. The coordinates must be normalized between 0.0 and 1.0
  // in the image space, with respect to the screen size and the aspect ratio.
  const float xNormalized = x / m_screenSize.width() / 1.349634 + 0.174817;
  const float yNormalized = y / m_screenSize.height();
  const CGPoint screenPoint = CGPointMake(yNormalized, 1.0 - xNormalized);

  // return a list of results, sorted from nearest to farthest (in distance from the camera).
  Q_CHECK_PTR(m_impl);
  NSArray<ARHitTestResult*>* hitResults = [m_impl->arSession.currentFrame
      hitTest: screenPoint types: ARHitTestResultTypeEstimatedHorizontalPlane];

  if (!hitResults || [hitResults count] <= 0)
    return {};

  ARHitTestResult* hitResult = [hitResults objectAtIndex:0];
  if (!hitResult)
    return {};

  const simd_float4x4 transform = [hitResult worldTransform];
  return { 0.0, 0.0, 0.0, 1.0, transform.columns[3].x, transform.columns[3].y, transform.columns[3].z };
}

QMatrix4x4 ArKitWrapper::viewProjectionMatrix() const
{
  Q_CHECK_PTR(m_impl);
  simd_float4x4 transformMatrix = m_impl->arSession.currentFrame.camera.transform;
  return QMatrix4x4(reinterpret_cast<float*>(&transformMatrix));
}

std::vector<float> ArKitWrapper::pointCloudData() const
{
  Q_CHECK_PTR(m_impl);
  ARPointCloud* pointCloudData = [m_impl->arSession.currentFrame rawFeaturePoints];
  if (!pointCloudData)
    return {};

  const auto size = pointCloudData.count;
  if (size == 0)
    return {};

  const float* rawPoints = reinterpret_cast<const float*>(pointCloudData.points);
  return std::vector<float>(rawPoints, rawPoints + (3 * size));
}

/*!
  \internal
 * Low access to the ARKit objects.
 */
template<>
ARSession* ArKitWrapper::arRawPtr<ARSession>() const
{
  Q_CHECK_PTR(m_impl);
  return m_impl->arSession;
}

/*!
  \internal
  Low access to the ARCore objects.
 */
template<>
ARConfiguration* ArKitWrapper::arRawPtr<ARConfiguration>() const
{
  Q_CHECK_PTR(m_impl);
  return m_impl->arConfiguration;
}

/*!
  \internal
  Low access to the ARCore objects.
 */
template<>
NSObject<ARSessionDelegate>* ArKitWrapper::arRawPtr<NSObject<ARSessionDelegate>>() const
{
  Q_CHECK_PTR(m_impl);
  return m_impl->arSessionDelegate;
}
