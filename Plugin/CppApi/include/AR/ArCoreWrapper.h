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

#ifndef ARCOREWRAPPER_H
#define ARCOREWRAPPER_H

#include "arcore_c_api.h"
#include <QAndroidJniEnvironment>

namespace Esri
{
namespace ArcGISRuntime
{
namespace Toolkit // internal?
{

class ArCoreWrapper
{
  ArSession* session(); // static?

  JNIEnv* jniEnvironment();
  jobject applicationActivity();

  bool install();
  void create();
  void pause();
  void resume();
  void destroy();

private:
  QAndroidJniEnvironment m_jniEnvironment;

  jobject m_applicationActivity = nullptr;

  ArSession* m_arSession = nullptr; // static??

  static int32_t m_installRequested; // When your apllication launches or enters an AR mode,
        // it should call this method with user_requested_install = 1.


};

} // Toolkit
} // ArcGISRuntime
} // Esri

#endif // ARCOREWRAPPER_H
