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

#ifndef ARWRAPPER_H
#define ARWRAPPER_H

// C++ API headers
#include "Deprecated.h"

// Qt headers
#include <QColor>
#include <QSize>
#include <QtGlobal>

// STL headers
#include <array>

#if defined Q_OS_IOS
#include "ArKitWrapper.h"
#elif defined Q_OS_ANDROID
#include "ArCoreWrapper.h"
#endif

namespace Esri::ArcGISRuntime::Toolkit::Internal {

class ArcGISArView;

#ifdef Q_OS_IOS

class ArWrapper : public ArKitWrapper { using ArKitWrapper::ArKitWrapper; };

#elif defined Q_OS_ANDROID

// implementation of the AR core for Android.
class ArWrapper : public ArCoreWrapper { using ArCoreWrapper::ArCoreWrapper; };

#else

// default implementation for desktop platforms.
class QRT_DEPRECATED ArWrapper
{
public:
  ArWrapper(void*);

  void initGL() const;
  void render() const;

  void startTracking();
  void stopTracking();
  void resetTracking();

  void setSize(const QSize&);

  bool renderVideoFeed() const;
  void setRenderVideoFeed(bool renderVideoFeed);

  std::array<double, 7> hitTest(int, int) const;

  // properties for debug mode
  QColor pointCloudColor() const;
  void setPointCloudColor(const QColor& pointCloudColor);

  int pointCloudSize() const;
  void setPointCloudSize(int pointCloudSize);

  QColor planeColor() const;
  void setPlaneColor(const QColor& planeColor);

  // low access to the ARKit/ARCore objects
  template<typename ArRawPtr>
  ArRawPtr* arRawPtr() const;
};

#endif

} // Esri::ArcGISRuntime::Toolkit::Internal

#endif // ARWRAPPER_H
