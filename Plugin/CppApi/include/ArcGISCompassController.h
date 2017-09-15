// Copyright 2016 ESRI
//
// All rights reserved under the copyright laws of the United States
// and applicable international laws, treaties, and conventions.
//
// You may freely redistribute and use this sample code, with or
// without modification, provided you include the original copyright
// notice and use restrictions.
//
// See the Sample code usage restrictions document for further information.
//

#ifndef ARCGISCOMPASSCONTROLLER_H
#define ARCGISCOMPASSCONTROLLER_H

#include <QObject>

#include "ToolkitCommon.h"
#include "AbstractTool.h"

namespace Esri
{
namespace ArcGISRuntime
{

class MapQuickView;
class SceneQuickView;

namespace Toolkit
{
class TOOLKIT_EXPORT ArcGISCompassController : public QObject, public AbstractTool
{
  Q_OBJECT

  // whether compass should be visible if rotation/heading is 0
  Q_PROPERTY(bool autoHide READ autoHide WRITE setAutoHide NOTIFY autoHideChanged)

  // rotation or heading of the MapView/SceneView (respectively). Used to determine Compass rotation
  Q_PROPERTY(double heading READ heading WRITE setHeading NOTIFY headingChanged)

signals:
  void headingChanged();
  void autoHideChanged();

public:
  Q_INVOKABLE void setHeading(double rotation);
  Q_INVOKABLE void setAutoHide(bool autoHide);

public:
  ArcGISCompassController(QObject *parent = nullptr);
  ~ArcGISCompassController();

  bool setView(Esri::ArcGISRuntime::MapQuickView* mapView);
  bool setView(Esri::ArcGISRuntime::SceneQuickView* sceneView);

  double heading() const;
  bool autoHide() const;

  QString toolName() const override;

private:
  double m_heading = 0.0;
  bool m_autoHide = true;
  Esri::ArcGISRuntime::MapQuickView* m_mapView = nullptr;
  Esri::ArcGISRuntime::SceneQuickView* m_sceneView = nullptr;
};

} // Toolkit
} // ArcGISRuntime
} // Esri

#endif // ARCGISCOMPASSCONTROLLER_H
