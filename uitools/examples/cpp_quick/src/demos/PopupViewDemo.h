// Copyright 2021 ESRI
//
// All rights reserved under the copyright laws of the United States
// and applicable international laws, treaties, and conventions.
//
// You may freely redistribute and use this sample code, with or
// without modification, provided you include the original copyright
// notice and use restrictions.
//
// See the Sample code usage restrictions document for further information.
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
