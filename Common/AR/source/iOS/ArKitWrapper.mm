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
#import <ARKit/ARKit.h>
#include <QMatrix4x4>
#include "ArcGISArViewInterface.h"
#include <array>
#include <QGuiApplication>
#include <QScreen>
#include "ArKitUtils.h"
#include "ArKitFrameRenderer.h"
#include "ArKitPointCloudRenderer.h"


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

// ARSessionDelegate overrides
- (void) session: (ARSession*) session didUpdateFrame: (ARFrame*) frame;

// ARSessionObserver overrides
- (void) session: (ARSession*) session cameraDidChangeTrackingState: (ARCamera*) camera;
//sessionWasInterrupted
//sessionInterruptionEnded
//sessionShouldAttemptRelocalization
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

@property (nonatomic) simd_float4x4 initialMatrix;
@property (nonatomic) bool resetInitialMatrix;

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
    self.textureDataUsed = false;
    self.resetInitialMatrix = true;
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
  self.arcGISArView->updateCamera(camera[0], camera[1], camera[2], camera[3], camera[4], camera[5], camera[6]);

  // udapte the field of view, based on the
  auto lens = [self lastLensIntrinsics: frame.camera];
  self.arcGISArView->updateFieldOfView(lens[0], lens[1], lens[2], lens[3], lens[4], lens[5]);

  // render the frame of the ArcGIS runtime
  self.arcGISArView->renderFrame();
}

- (void) session: (ARSession*) session cameraDidChangeTrackingState: (ARCamera*) camera
{
}

- (void) session: (ARSession*) session didFailWithError: (NSError*) error
{
  // not implemented.
}

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
  // todo: necessary?
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
  // reset the intial transformation matrix is required
  if (self.resetInitialMatrix)
  {
    self.initialMatrix = cameraTransform;
    self.resetInitialMatrix = false;
  }

//  cameraTransform = simd_mul(simd_inverse(self.initialMatrix), cameraTransform);

  // A quaternion used to compensate for the pitch being 90 degrees on `ARKit`; used to calculate the current
  // device transformation for each frame.
  const simd_quatf compensationQuat = { simd_float4 { 0.70710678118, 0.0, 0.0, 0.70710678118 }};
  simd_quatf finalQuat = simd_mul(compensationQuat, simd_quaternion(cameraTransform));

  // get the screen orientation
  const Qt::ScreenOrientations orientation = QGuiApplication::screens().front()->orientation();

  switch (orientation) {
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
      // todo: test and fix that
      break;
    }
    default:
      break;
  }

  // Calculate our final quaternion and create the new transformation matrix.
  const simd_quatf compensationQuat2 = { simd_float4 { -0.70710678118, 0, 0, 0.70710678118 }};
  finalQuat = simd_mul(compensationQuat2, finalQuat);

  finalQuat = simd_quaternion(cameraTransform);

  return {
    finalQuat.vector.x,
    finalQuat.vector.y,
    finalQuat.vector.z,
    finalQuat.vector.w,
    cameraTransform.columns[3].x,
    -cameraTransform.columns[3].z,
    cameraTransform.columns[3].y
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
  m_arKitFrameRenderer(new ArKitFrameRenderer)
{
  // Create an AR session configuration
  // todo: test if the COMPASS is enable
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

  // https://developer.apple.com/documentation/arkit/arconfiguration/2923553-issupported?language=objc
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
  [m_impl->arSession runWithConfiguration:m_impl->arConfiguration];
}

void ArKitWrapper::stopTracking()
{
//  m_impl->arSession->pause();
}

void ArKitWrapper::resetTracking()
{
  // https://developer.apple.com/documentation/arkit/arsession/2865608-runwithconfiguration?language=objc
  m_impl->arSessionDelegate.resetInitialMatrix = true;
}

void ArKitWrapper::setSize(const QSizeF& size)
{
  m_arKitFrameRenderer->setSize(size);
}

// this function run on the rendering thread
void ArKitWrapper::initGL()
{
  m_arKitFrameRenderer->initGL();

  if (m_arKitPointCloudRenderer)
    m_arKitPointCloudRenderer->initGL(); // for debugging the AR tracking
}

// this function run on the rendering thread
void ArKitWrapper::updateTextures()
{
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // ??

  m_arKitFrameRenderer->updateTextreDataY(static_cast<int>(m_impl->arSessionDelegate.widthY),
                                         static_cast<int>(m_impl->arSessionDelegate.heightY),
                                         m_impl->arSessionDelegate.textureDataY);

  m_arKitFrameRenderer->updateTextreDataCbCr(static_cast<int>(m_impl->arSessionDelegate.widthCbCr),
                                            static_cast<int>(m_impl->arSessionDelegate.heightCbCr),
                                            m_impl->arSessionDelegate.textureDataCbCr);

  m_impl->arSessionDelegate.textureDataUsed = false; // now, the texture data can be reused.
}

// this function run on the rendering thread
void ArKitWrapper::render()
{
  // updates the texture datas
  updateTextures();

  // render the AR frame
  m_arKitFrameRenderer->render();

  if (m_arKitPointCloudRenderer)
    m_arKitPointCloudRenderer->render(); // for debugging the AR tracking
}

bool ArKitWrapper::renderVideoFeed() const
{
  return m_renderVideoFeed;
}

void ArKitWrapper::setRenderVideoFeed(bool renderVideoFeed)
{
  m_renderVideoFeed = renderVideoFeed;
}

// doc: https://developer.apple.com/documentation/arkit/arframe/2875718-hittest?language=objc
std::array<double, 7> ArKitWrapper::hitTest(int x, int y) const
{
  // return a list of results, sorted from nearest to farthest (in distance from the camera).
  NSArray<ARHitTestResult*>* hitResults = [m_impl->arSession.currentFrame
      hitTest: CGPointMake(x, y) types: ARHitTestResultTypeFeaturePoint]; // ARHitTestResultType?

  if (!hitResults || [hitResults count] <= 0)
    return {};

  ARHitTestResult* hitResult = [hitResults objectAtIndex:0];
  if (!hitResult)
    return {};

  const simd_float4x4 transform = [hitResult worldTransform];
  return { 0, 0, 0, 1, transform.columns[3].x, -transform.columns[3].z, transform.columns[3].y };
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

/*!
 * \internal
 * Low access to the ARKit objects.
 */
template<>
ARSession* ArKitWrapper::arRawPtr<ARSession>() const
{
  Q_CHECK_PTR(m_impl);
  return m_impl->arSession;
}

/*!
 * \internal
 * Low access to the ARCore objects.
 */
template<>
ARConfiguration* ArKitWrapper::arRawPtr<ARConfiguration>() const
{
  Q_CHECK_PTR(m_impl);
  return m_impl->arConfiguration;
}

/*!
 * \internal
 * Low access to the ARCore objects.
 */
template<>
NSObject<ARSessionDelegate>* ArKitWrapper::arRawPtr<NSObject<ARSessionDelegate>>() const
{
  Q_CHECK_PTR(m_impl);
  return m_impl->arSessionDelegate;
}
