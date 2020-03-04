/*******************************************************************************
 *  Copyright 2012-2020 Esri
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
#ifndef ESRI_ARCGISRUNTIME_TOOLKIT_REGISTER_H
#define ESRI_ARCGISRUNTIME_TOOLKIT_REGISTER_H
/*!
* \file register.h
*
* This file contains the registration function required to register the C++
* controllers provided by the Toolkit with your application.
*
* If you intend to utilize the toolkit with [Map|Scene]QuickView in your
* application, invoking registerComponents is required! Please refer to
* README.md for more information on workflows.
*/

class QQmlEngine;

namespace Esri
{
namespace ArcGISRuntime
{
namespace Toolkit
{

/*!
 * \brief This registration function is required to register all the C++
 * controllers within your application in QML.
 * 
 * For example this will expose the class provided by NorthArrowController.h in
 * QML as NorthArrowController.
 * 
 * Internally, this function add a new `cpp_api` selector to your QmlEngine's
 * file selector. This is the mechanism utilized to override, say,
 * NorthArrowController.qml with the NorthArrowController provided by C++.
 * 
 * \param engine The QML engine.
 */
void registerComponents(QQmlEngine* engine);

} // Toolkit
} // ArcGISRuntime
} // Esri

#endif // ESRI_ARCGISRUNTIME_TOOLKIT_REGISTER_H
