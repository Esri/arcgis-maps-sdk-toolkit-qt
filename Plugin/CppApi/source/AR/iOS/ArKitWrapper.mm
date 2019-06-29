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
#include "ArcGISArView.h"
#include <QThread>

using namespace Esri::ArcGISRuntime;
using namespace Esri::ArcGISRuntime::Toolkit;

/*!
  \class Esri::ArcGISRuntime::Toolkit::ArcGISARView
  \ingroup AR
  \inmodule ArcGISQtToolkit
  \since Esri::ArcGISRuntime 100.6
  \brief ...
  \sa {AR}
 */

#define QDEBUG qDebug() << QThread::currentThread()

/*******************************************************************************
 ******************* Objective-C class declarations ****************************
 ******************************************************************************/

@interface ArcGISArSessionDelegate : NSObject<ARSessionDelegate>

-(id)init;
-(void)session: (ARSession*)session didUpdateFrame:(ARFrame*)frame;

@property (nonatomic) ArcGISArView* arView;
@property (nonatomic, retain) ARFrame* frame;
@property (nonatomic) CVImageBufferRef pixelBuffer; // CVPixelBufferRef
@property (nonatomic) uchar* textureDataY;
@property (nonatomic) uchar* textureDataCbCr;
@property (nonatomic) size_t width_0;
@property (nonatomic) size_t width_1;
@property (nonatomic) size_t height_0;
@property (nonatomic) size_t height_1;
@property (atomic) bool render_in_progress;

@end

/*******************************************************************************
 ******************* Objective-C class implementations *************************
 ******************************************************************************/

@implementation ArcGISArSessionDelegate

-(id)init
{
  if (self = [super init])
  {
    self.arView = nullptr;
    self.frame = nullptr;
    self.pixelBuffer = nullptr;
    self.textureDataY = nullptr;
    self.textureDataCbCr = nullptr;
    self.width_0 = 0;
    self.width_1 = 0;
    self.height_0 = 0;
    self.height_1 = 0;
    self.render_in_progress = false;
  }
  return self;
}

-(void)session: (ARSession*)session didUpdateFrame:(ARFrame*)frame
{
  if (!frame || self.render_in_progress)
  {
    QDEBUG << "----- no frame or render in progress";
    return;
  }

  self.render_in_progress = true;

  self.frame = frame;
  self.pixelBuffer = frame.capturedImage;

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

  // map
  CVPixelBufferRetain(self.pixelBuffer); // retains the new PB
  CVPixelBufferLockBaseAddress(self.pixelBuffer, kCVPixelBufferLock_ReadOnly);

  // create buffers
  uchar* dataY = static_cast<uchar*>(CVPixelBufferGetBaseAddressOfPlane(self.pixelBuffer, 0));
  uchar* dataCbCr = static_cast<uchar*>(CVPixelBufferGetBaseAddressOfPlane(self.pixelBuffer, 1));

  self.width_0 = CVPixelBufferGetWidthOfPlane(self.pixelBuffer, 0);
  self.width_1 = CVPixelBufferGetWidthOfPlane(self.pixelBuffer, 1);

  self.height_0 = CVPixelBufferGetHeightOfPlane(self.pixelBuffer, 0);
  self.height_1 = CVPixelBufferGetHeightOfPlane(self.pixelBuffer, 1);

  const size_t bytesPerRow_0 = CVPixelBufferGetBytesPerRowOfPlane(self.pixelBuffer, 0);
  const size_t bytesPerRow_1 = CVPixelBufferGetBytesPerRowOfPlane(self.pixelBuffer, 1);

  const size_t size_0 = self.height_0 * bytesPerRow_0;
  const size_t size_1 = self.height_1 * bytesPerRow_1;

  self.textureDataY = static_cast<uchar*>(malloc(size_0));
  self.textureDataCbCr = static_cast<uchar*>(malloc(size_1));

  memcpy(self.textureDataY, dataY, size_0);
  memcpy(self.textureDataCbCr, dataCbCr, size_1);

  // unmap
  CVPixelBufferUnlockBaseAddress(self.pixelBuffer, kCVPixelBufferLock_ReadOnly);
  CVPixelBufferRelease(self.pixelBuffer); // release the previous PB if necessary

  // update
  self.arView->updateCamera();
  self.arView->update();
}

@end

/*******************************************************************************
 ******************** C++ private class implementation *************************
 ******************************************************************************/

struct ArKitWrapper::ArKitWrapperPrivate {
  ARSession* arSession = nullptr;
  ARWorldTrackingConfiguration* arConfiguration = nullptr;
  ArcGISArSessionDelegate* arSessionDelegate = nullptr;
  bool isSupported = false;
};

/*******************************************************************************
 ******************** C++ public class implementation **************************
 ******************************************************************************/

//TODO: test performances with ARCNView from AR kit. Integration of the ARCNView in Qt?

ArKitWrapper::ArKitWrapper(ArcGISArView* arView) :
  m_impl(new ArKitWrapperPrivate),
  m_arKitFrameRenderer(this),
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
  m_impl->arSessionDelegate.arView = arView;
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

bool ArKitWrapper::isValid() const
{
  return true;
}

void ArKitWrapper::startTracking()
{
}

void ArKitWrapper::stopTracking()
{
}

QSizeF ArKitWrapper::size() const
{
  return QSize();
}

void ArKitWrapper::setSize(const QSizeF& /*size*/)
{
}

QMatrix4x4 ArKitWrapper::viewMatrix() const
{
  return QMatrix4x4();
}

QMatrix4x4 ArKitWrapper::projectionMatrix() const
{
  return QMatrix4x4();
}

const float* ArKitWrapper::pose() const
{
  return nullptr;
}

void ArKitWrapper::init()
{
  m_arKitFrameRenderer.init();
//  m_arKitPointCloudRenderer.init();
}

void ArKitWrapper::beforeRendering()
{
  // create textures ids
  if (m_textureY.isCreated())
    m_textureY.destroy();

  if (m_textureCbCr.isCreated())
    m_textureCbCr.destroy();

  m_textureY.setSize(m_impl->arSessionDelegate.width_0, m_impl->arSessionDelegate.height_0);
  m_textureCbCr.setSize(m_impl->arSessionDelegate.width_1, m_impl->arSessionDelegate.height_1);

  m_textureY.setFormat(QOpenGLTexture::R8_UNorm);
  m_textureCbCr.setFormat(QOpenGLTexture::RG8_UNorm);

  m_textureY.allocateStorage();
  m_textureCbCr.allocateStorage();

  m_textureY.setData(QOpenGLTexture::Red, QOpenGLTexture::UInt8, m_impl->arSessionDelegate.textureDataY);
  m_textureCbCr.setData(QOpenGLTexture::RG, QOpenGLTexture::UInt8, m_impl->arSessionDelegate.textureDataCbCr);
}

void ArKitWrapper::afterRendering()
{
  m_impl->arSessionDelegate.render_in_progress = false;
}

void ArKitWrapper::render()
{
  beforeRendering();

  if (m_textureY.textureId() != 0 && m_textureCbCr.textureId() != 0)
  {
    m_arKitFrameRenderer.render(m_textureY.textureId(), m_textureCbCr.textureId());
    // m_arKitPointCloudRenderer.render();
  }

  afterRendering();
}

bool ArKitWrapper::install()
{
  return true;
}

void ArKitWrapper::create()
{
}

void ArKitWrapper::pause()
{
}

void ArKitWrapper::resume()
{
}

void ArKitWrapper::update()
{
}

void ArKitWrapper::destroy()
{
}

TransformationMatrix ArKitWrapper::transformationMatrix() const
{
  simd_float4x4 cameraTransform = m_impl->arSessionDelegate.frame.camera.transform;
  // uses float not double. How to convert simd_float4x4 to simd_double4x4?

  /// A quaternion used to compensate for the pitch being 90 degrees on `ARKit`; used to calculate the current
  /// device transformation for each frame.
  simd_quatf compensationQuat = { simd_float4 { 0.70710678118, 0.0, 0.0, 0.70710678118 }};

  // Calculate our final quaternion and create the new transformation matrix.
  simd_quatf finalQuat = simd_mul(compensationQuat, simd_quaternion(cameraTransform));

  const float translationTransformationFactor = 1.0;
  return TransformationMatrix(finalQuat.vector.x,
                              finalQuat.vector.y,
                              finalQuat.vector.z,
                              finalQuat.vector.w,
                              (cameraTransform.columns[3].x) * translationTransformationFactor,
                              (-cameraTransform.columns[3].z) * translationTransformationFactor,
                              (cameraTransform.columns[3].y) * translationTransformationFactor);
}

float* ArKitWrapper::transformedUvs() const
{
  return nullptr;
}

float* ArKitWrapper::modelViewProjectionData() const
{
  return nullptr;
}

const float* ArKitWrapper::pointCloudData() const
{
  return nullptr;
}

int32_t ArKitWrapper::pointCloudSize() const
{
  return 0;
}
