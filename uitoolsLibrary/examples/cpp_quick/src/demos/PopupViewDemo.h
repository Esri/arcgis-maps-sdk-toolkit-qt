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

#ifndef ARCGIS_RUNTIME_TOOLKIT_CPP_QUICK_DEMO_POPUPVIEWDEMO_H
#define ARCGIS_RUNTIME_TOOLKIT_CPP_QUICK_DEMO_POPUPVIEWDEMO_H

#include "BaseDemo.h"
#include "FeatureLayer.h"
#include "PopupManager.h"

#include <QObject>
#include <QQmlEngine>

class PopupViewDemo : public BaseDemo
{
  Q_OBJECT
  Q_PROPERTY(QObject* popupManager READ popupManager_ WRITE setPopupManager_ NOTIFY popupManagerChanged)
  QML_ELEMENT
public:
  Q_INVOKABLE PopupViewDemo(QObject* parent = nullptr);
  ~PopupViewDemo() override;

protected:
  Esri::ArcGISRuntime::Map* initMap_(QObject* parent) const override;
  Esri::ArcGISRuntime::Scene* initScene_(QObject* parent) const override;

signals:
  void popupManagerChanged();

private slots:
  void setUp();

private:
  QObject* popupManager_();
  void setPopupManager_(QObject* popupManager);

private:
  Esri::ArcGISRuntime::FeatureLayer* m_featureLayer = nullptr;
  QObject* m_popupManager = nullptr;
};

#endif // ARCGIS_RUNTIME_TOOLKIT_CPP_QUICK_DEMO_POPUPVIEWDEMO_H
