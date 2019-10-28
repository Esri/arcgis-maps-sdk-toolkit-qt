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

#include "ArWrapper.h"

using namespace Esri::ArcGISRuntime::Toolkit::Internal;

#ifdef Q_OS_IOS

// implemented in ArKitWrapper

#elif defined Q_OS_ANDROID

// implemented in ArCoreWrapper

#else

// default implementation for desktop platforms.

ArWrapper::ArWrapper(void*)
{
}

void ArWrapper::initGL() const
{
}

void ArWrapper::render() const
{
}

void ArWrapper::startTracking()
{
}

void ArWrapper::stopTracking()
{
}

void ArWrapper::resetTracking()
{
}

void ArWrapper::setSize(const QSize&)
{
}

bool ArWrapper::renderVideoFeed() const
{
  return false;
}

void ArWrapper::setRenderVideoFeed(bool)
{
}

std::array<double, 7> ArWrapper::hitTest(int, int) const
{
  return {};
}

// properties for debug mode
QColor ArWrapper::pointCloudColor() const
{
  return QColor();
}

void ArWrapper::setPointCloudColor(const QColor&)
{
}

int ArWrapper::pointCloudSize() const
{
  return -1;
}

void ArWrapper::setPointCloudSize(int)
{
}

QColor ArWrapper::planeColor() const
{
  return QColor();
}

void ArWrapper::setPlaneColor(const QColor&)
{
}

template<typename ArRawPtr>
ArRawPtr* ArWrapper::arRawPtr() const
{
  return nullptr;
}

#endif
