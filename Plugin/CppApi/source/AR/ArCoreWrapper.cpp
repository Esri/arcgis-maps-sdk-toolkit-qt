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

#include "ArcGISARView.h"

/*!
  \class Esri::ArcGISRuntime::Toolkit::ArcGISARView
  \ingroup AR
  \inmodule ArcGISQtToolkit
  \since Esri::ArcGISRuntime 100.6
  \brief ...
  \sa {AR}
 */

namespace Esri
{
namespace ArcGISRuntime
{
namespace Toolkit
{

#include "arcore_c_api.h"
#include <QtAndroid>
#include <QAndroidJniEnvironment>
#include <QDebug>

static ArSession* s_ar_session = nullptr;
static bool s_install_requested = false;

void ar_destroy() {
    if (s_ar_session != nullptr)
        ArSession_destroy(s_ar_session);
}

void ar_pause() {
    if (s_ar_session != nullptr)
        ArSession_pause(s_ar_session);
}

// java: public static native long createNativeApplication(AssetManager assetManager);

// java: JniInterface.onResume(nativeApplication, getApplicationContext(), this);
// java: public static native void onResume(long nativeApplication, Context context, Activity activity);
void initArSession() {
    if (s_ar_session != nullptr)
        return;

    QAndroidJniEnvironment env;
    // Note: AR run in different thread, must to create local reference to avoid deleted object.
    jobject activity = env->NewLocalRef(QtAndroid::androidActivity().object<jobject>());

// jobject context = QtAndroid::androidContext().object<jobject>();
// jobject context_ = env->NewGlobalRef(context);
// qDebug() << "--- 1 ---" << env << context << activity << context_;
// jobject activity_ = env->NewLocalRef(activity);

    if (!env || !activity) {
        qDebug() << "Fails to init env or activity pointers.";
        return;
    }

    switch (install_status) {
    case AR_INSTALL_STATUS_INSTALLED:
        break;
    case AR_INSTALL_STATUS_INSTALL_REQUESTED:
        s_install_requested = true;
        return;
    }

    // === ATTENTION!  ATTENTION!  ATTENTION! ===
    // This method can and will fail in user-facing situations.  Your
    // application must handle these cases at least somewhat gracefully.  See
    // HelloAR Java sample code for reasonable behavior.
    ar_status = ArSession_create(env, context, &s_ar_session);
    if (ar_status != AR_SUCCESS) {
        qDebug() << "ArSession_create fails.";
        return;
    }
    else {
        qDebug() << "Succed to run ArSession_create.";
    }

    if (s_ar_session == nullptr) {
        qDebug() << "s_ar_session is nullptr.";
        return;
    }
    else {
        qDebug() << "s_ar_session is not null";
    }

}

} // Toolkit
} // ArcGISRuntime
} // Esri
