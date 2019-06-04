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

#include "ArCoreWrapper.h"
#include <QtAndroid>
#include <QAndroidJniEnvironment>
#include <QDebug>

/*!
  \class Esri::ArcGISRuntime::Toolkit::ArcGISARView
  \ingroup AR
  \inmodule ArcGISQtToolkit
  \since Esri::ArcGISRuntime 100.6
  \brief ...
  \sa {AR}
 */

// TODO?

// ArCoreApk_checkAvailability https://developers.google.com/ar/reference/c/group/arcoreapk#group__arcoreapk_1ga448092e9b601bc6f87d13d05317413f1
// ARCore-optional applications must ensure that ArCoreApk_checkAvailability() returns one of the AR_AVAILABILITY_SUPPORTED_... values before calling this method.

namespace Esri
{
namespace ArcGISRuntime
{
namespace Toolkit
{

int32_t ArCoreWrapper::m_installRequested = 1;

ArSession* ArCoreWrapper::session()
{
  return m_arSession;
}

//The application's JNIEnv object
JNIEnv* ArCoreWrapper::jniEnvironment()
{
//  Step 1 get JNIEnv pointer by defining
//  JNIEXPORT jint JNI_OnLoad(JavaVM* vm, void* /*reserved*/).
//  You can define it (once per .so file) in any .cpp file you like


//  JNIEnv* env;
//  6 // get the JNIEnv pointer.
//  7 if (vm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6) != JNI_OK)
//  8 return JNI_ERR;


//  QAndroidJniEnvironment qjniEnv;
//  return qjniEnv->ExceptionCheck();
  return m_jniEnvironment;
}

//A jobject referencing the application's current Android Activity.
jobject ArCoreWrapper::applicationActivity()
{
  // Note: AR run in different thread, must to create local reference to avoid deleted object.
  if (m_applicationActivity == nullptr)
    m_applicationActivity = jniEnvironment()->NewLocalRef(QtAndroid::androidActivity().object<jobject>());

  // jobject context = QtAndroid::androidContext().object<jobject>();
  // jobject context_ = env->NewGlobalRef(context);
  // qDebug() << "--- 1 ---" << env << context << activity << context_;
  // jobject activity_ = env->NewLocalRef(activity);

  return m_applicationActivity;
}

// Initiates installation of ARCore if needed.
// May be called prior to ArSession_create()

// When your apllication launches or enters an AR mode, it should call this method with user_requested_install = 1.
// If ARCore is installed and compatible, this function will set out_install_status to AR_INSTALL_STATUS_INSTALLED.
// If ARCore is not currently installed or the installed version not compatible, the function will set out_install_status to AR_INSTALL_STATUS_INSTALL_REQUESTED and return immediately. Your current activity will then pause while the user is offered the opportunity to install it.
// When your activity resumes, you should call this method again, this time with user_requested_install = 0. This will either set out_install_status to AR_INSTALL_STATUS_INSTALLED or return an error code indicating the reason that installation could not be completed.

bool ArCoreWrapper::install()
{
  if (m_arSession != nullptr)
    return true;

  ArInstallStatus install_status;
  ArStatus error = ArCoreApk_requestInstall(jniEnvironment(), applicationActivity(), m_installRequested, &install_status);
  if (error != AR_SUCCESS)
  {
    //AR_ERROR_FATAL if an error occurs while checking for or requesting installation
    //AR_UNAVAILABLE_DEVICE_NOT_COMPATIBLE if ARCore is not supported on this device.
    //AR_UNAVAILABLE_USER_DECLINED_INSTALLATION if the user previously declined installation.
    return false;
  }

  switch (install_status)
  {
  case AR_INSTALL_STATUS_INSTALLED:
    // The requested resource is already installed.
    return true;
  case AR_INSTALL_STATUS_INSTALL_REQUESTED:
    // Installation of the resource was requested. The current activity will be paused.
    m_installRequested = 0;
    return false;
  }
}

// java: public static native long createNativeApplication(AssetManager assetManager);

// java: JniInterface.onResume(nativeApplication, getApplicationContext(), this);
// java: public static native void onResume(long nativeApplication, Context context, Activity activity);
void ArCoreWrapper::create()
{
  if (m_arSession != nullptr)
    return;

  // === ATTENTION!  ATTENTION!  ATTENTION! ===
  // This method can and will fail in user-facing situations.  Your
  // application must handle these cases at least somewhat gracefully.  See
  // HelloAR Java sample code for reasonable behavior.
  auto ar_status = ArSession_create(jniEnvironment(), applicationActivity(), &m_arSession);

  if (ar_status != AR_SUCCESS)
  {
    qDebug() << "ArSession_create fails.";
    return;
  }
  else
  {
    qDebug() << "Succed to run ArSession_create.";
  }

  if (m_arSession == nullptr)
  {
    qDebug() << "m_arSession is nullptr.";
    return;
  }
  else {
    qDebug() << "m_arSession is not null";
  }

  //request camera autorisation

}

void ArCoreWrapper::pause() {
  if (m_arSession != nullptr)
    ArSession_pause(m_arSession);
}

void ArCoreWrapper::resume() {
  if (m_arSession != nullptr)
    ArSession_resume(m_arSession);
}

void ArCoreWrapper::destroy() {
  if (m_arSession != nullptr)
    ArSession_destroy(m_arSession);
}

} // Toolkit
} // ArcGISRuntime
} // Esri
