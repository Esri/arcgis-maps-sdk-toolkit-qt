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

#ifndef ARWRAPPER_H
#define ARWRAPPER_H

#include <QSize>
#include <QOpenGLFunctions>
#include "TransformationMatrix.h"

#if defined Q_OS_IOS
#include "ArKitWrapper.h"
#elif defined Q_OS_ANDROID
#include "ArCoreWrapper.h"
#endif

namespace Esri
{
namespace ArcGISRuntime
{
namespace Toolkit
{

class ArcGISArView;

#ifdef Q_OS_IOS

// implementation of the AR kit for iOS.
class ArWrapper : public ArKitWrapper { using ArKitWrapper::ArKitWrapper; };

#elif defined Q_OS_ANDROID

// implementation of the AR core for Android.
class ArWrapper : public ArCoreWrapper { using ArCoreWrapper::ArCoreWrapper; };

#else

// default implementation for desktop platforms.
class ArWrapper
{
public:
  ArWrapper(ArcGISArView* arView);

  void init() const;
  void render() const;

  bool isValid() const;

  void startTracking();
  void stopTracking();
  void setSize(const QSize&);
  void setTextureId(GLuint);
  void update();

  TransformationMatrix transformationMatrix() const;
  float* transformedUvs() const;
  float* modelViewProjectionData() const;
  const float* pointCloudData() const;
  int32_t pointCloudSize() const;
};

#endif

} // Toolkit
} // ArcGISRuntime
} // Esri

#endif // ARWRAPPER_H
