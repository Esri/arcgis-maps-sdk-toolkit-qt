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

#include "ArWrapper.h"

using namespace Esri::ArcGISRuntime;
using namespace Esri::ArcGISRuntime::Toolkit;

#ifdef Q_OS_IOS

// implemented in ArKitWrapper

#elif defined Q_OS_ANDROID

// implemented in ArCoreWrapper

#else

ArWrapper::ArWrapper(void*)
{
}

void ArWrapper::init() const
{
}

void ArWrapper::render() const
{
}

bool ArWrapper::isValid() const
{
  return false;
}

// default implementation for desktop platforms.
void ArWrapper::startTracking()
{
}

void ArWrapper::stopTracking()
{
}

void ArWrapper::setSize(const QSize&)
{
}

void ArWrapper::setTextureId(GLuint)
{
}

void ArWrapper::update()
{
}

QObject* ArWrapper::transformationMatrix() const
{
  return nullptr;
}

float* ArWrapper::transformedUvs() const
{
  return nullptr;
}

float* ArWrapper::modelViewProjectionData() const
{
  return nullptr;
}

const float* ArWrapper::pointCloudData() const
{
  return nullptr;
}

int32_t ArWrapper::pointCloudSize() const
{
  return 0;
}

#endif
