/*******************************************************************************
 *  Copyright 2012-2022 Esri
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

#ifndef ARCGIS_RUNTIME_TOOLKIT_CPP_QUICK_DEMO_BASEMAPGALLERYDEMO_H
#define ARCGIS_RUNTIME_TOOLKIT_CPP_QUICK_DEMO_BASEMAPGALLERYDEMO_H

// Qt headers
#include <QObject>
#include <QQmlEngine>

// Other headers
#include "BaseDemo.h"

Q_MOC_INCLUDE("BasemapGalleryController.h")

namespace Esri::ArcGISRuntime::Toolkit {
  class BasemapGalleryController;
} // Esri::ArcGISRuntime::Toolkit

class BasemapGalleryDemo : public BaseDemo
{
  Q_OBJECT
  QML_ELEMENT
  Q_PROPERTY(Esri::ArcGISRuntime::Toolkit::BasemapGalleryController* controller READ controller CONSTANT)
public:
  Q_INVOKABLE BasemapGalleryDemo(QObject* parent = nullptr);
  ~BasemapGalleryDemo() override;

  Esri::ArcGISRuntime::Toolkit::BasemapGalleryController* controller() const;

protected:
  Esri::ArcGISRuntime::Map* initMap_(QObject* parent) const override;
  Esri::ArcGISRuntime::Scene* initScene_(QObject* parent) const override;

private:
  Esri::ArcGISRuntime::Toolkit::BasemapGalleryController* m_controller;
};

#endif // ARCGIS_RUNTIME_TOOLKIT_CPP_QUICK_DEMO_BASEMAPGALLERYDEMO_H
