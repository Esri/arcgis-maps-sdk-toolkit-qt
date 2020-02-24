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
#include "register.h"

// Toolkit Includes
#include "CoordinateConversionController.h"
#include "CoordinateConversionOption.h"
#include "CoordinateConversionResult.h"
#include "NorthArrowController.h"

// Qt Includes
#include <QQmlEngine>
#include <QQmlFileSelector>

// std includes
#include <type_traits>

namespace Esri
{
namespace ArcGISRuntime
{
namespace Toolkit
{

const char* const Namespace= "Esri.ArcGISRuntime.Toolkit";
const int VersionMajor = 100;
const int VersionMinor = 7;

namespace
{

template <typename T>
void registerComponent()
{
  static_assert(std::is_base_of<QObject, T>::value, "Must inherit QObject");
  auto name = QString("%1CPP").arg(T::staticMetaObject.className());
  name.remove("Esri::ArcGISRuntime::Toolkit::");
  qmlRegisterType<T>(Namespace, VersionMajor, VersionMinor, name.toLatin1());
}

void addFileSelector(QQmlEngine* engine)
{
  auto fileSelector = QQmlFileSelector::get(engine);
  if (!fileSelector)
    fileSelector = new QQmlFileSelector(engine, engine);
  fileSelector->setExtraSelectors({"cpp_api"});
}

}

void registerComponents(QQmlEngine* engine)
{
  addFileSelector(engine);
  registerComponent<NorthArrowController>();
  registerComponent<CoordinateConversionController>();
  registerComponent<CoordinateConversionOption>();
  registerComponent<CoordinateConversionResult>();
}

} // Toolkit
} // ArcGISRuntime
} // Esri
