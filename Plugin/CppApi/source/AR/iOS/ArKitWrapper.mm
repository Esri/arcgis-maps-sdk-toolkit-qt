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

/*******************************************************************************
 ******************* Objective-C class declarations ****************************
 ******************************************************************************/

@interface ArcGISArSessionDelegate : NSObject<ARSessionDelegate>

-(id)init;
-(void)session: (ARSession*)session didUpdateFrame:(ARFrame*)frame;

@property (nonatomic) ArcGISArView* arView;
@property (nonatomic) ARFrame* frame;

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
  }
  return self;
}

-(void)session: (ARSession*)session didUpdateFrame:(ARFrame*)frame
{
  self.frame = frame;
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

  static GLuint createTextureId(const CVPixelBufferRef& pixelBuffer, int pixelFormat, size_t planeIndex);
};


GLuint ArKitWrapper::ArKitWrapperPrivate::createTextureId(const CVPixelBufferRef& pixelBuffer, int pixelFormat, size_t planeIndex)
{
  // Called from the render thread, so there is a current OpenGL context
  size_t width = CVPixelBufferGetWidthOfPlane(pixelBuffer, planeIndex);
  size_t height = CVPixelBufferGetHeightOfPlane(pixelBuffer, planeIndex);

  CVOpenGLESTextureCacheRef textureCache = nullptr;
  CVReturn status = CVOpenGLESTextureCacheCreate(kCFAllocatorDefault,
                                                 nullptr,
                                                 [EAGLContext currentContext],
                                                 nullptr,
                                                 &textureCache);

  qDebug() << "------------- createTextureId" << (int)status << width << height;
  if (status != kCVReturnSuccess )
  {
    qDebug() << "Error creating texture cache";
    return 0;
  }
  // Then, create a CVPixelBuffer-backed OpenGL ES texture image from the texture cache:

  //  CVOpenGLESTextureCacheFlush(m_renderer->m_textureCache, 0);
  // reuse the cache? https://code.woboq.org/qt5/qtmultimedia/src/plugins/avfoundation/camera/avfcamerarenderercontrol.mm.html#164

  CVOpenGLESTextureRef texture = nullptr;
  status = CVOpenGLESTextureCacheCreateTextureFromImage(kCFAllocatorDefault,
                                                        textureCache /*_CVGLTextureCache*/,
                                                        pixelBuffer /*CVImageBufferRef*/,
                                                        nullptr,
                                                        GL_TEXTURE_2D,
                                                        pixelFormat /*GL_RGBA, GL_LUMINANCE, GL_RGBA8_OES, GL_RED, and GL_RG*/,
                                                        static_cast<int>(width),
                                                        static_cast<int>(height),
                                                        GL_RGBA /*_formatInfo->glFormat*/ /*GL_RGBA and GL_LUMINANCE GL_BGRA*/,
                                                        GL_UNSIGNED_BYTE /*_formatInfo->glType*/ /*GL_UNSIGNED_BYTE*/,
                                                        planeIndex,
                                                        &texture);

  if (status != kCVReturnSuccess)
  {
    qDebug() << "Error creating texture from buffer";
    return 0;
  }

  // Finally, get an OpenGL ES texture name from the CVPixelBuffer-backed OpenGL ES texture image:
  return CVOpenGLESTextureGetName(texture);

  //  CVMetalTextureRef texture = nullptr;
  //  CVReturn status = CVOpenGLESTextureCacheCreateTextureFromImage(nullptr, capturedImageTextureCache, pixelBuffer,
  //                                                              nullptr, pixelFormat, width, height, planeIndex, &texture);
}

/*******************************************************************************
 ******************** C++ public class implementation **************************
 ******************************************************************************/

//TODO: test performances with ARCNView from AR kit. Integration of the ARCNView in Qt?

ArKitWrapper::ArKitWrapper(ArcGISArView* arView) :
  m_impl(new ArKitWrapperPrivate),
  m_arKitFrameRenderer(this),
  m_arKitPointCloudRenderer(this)
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
  m_arKitPointCloudRenderer.init();
}

void ArKitWrapper::beforeRendering()
{
  // create textures ids
  CVPixelBufferRef pixelBuffer = m_impl->arSessionDelegate.frame.capturedImage;
  if (CVPixelBufferGetPlaneCount(pixelBuffer) < 2)
    return;

  qDebug() << "---- createTextureIds";
  m_textureIdY = ArKitWrapperPrivate::createTextureId(pixelBuffer, GL_RED, 0);
  m_textureIdCbCr = ArKitWrapperPrivate::createTextureId(pixelBuffer, GL_RG, 1);
}

void ArKitWrapper::afterRendering()
{
}

void ArKitWrapper::render()
{
  beforeRendering();
  qDebug() << "----- render" << m_textureIdY << m_textureIdCbCr;
  if (m_textureIdY != 0 && m_textureIdCbCr != 0)
  {
    m_arKitFrameRenderer.render(m_textureIdY, m_textureIdCbCr);
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
