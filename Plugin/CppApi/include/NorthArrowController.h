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

#ifndef NorthArrowController_H
#define NorthArrowController_H

#include <QObject>

#include "AbstractTool.h"

namespace Esri
{
namespace ArcGISRuntime
{

class MapQuickView;
class SceneQuickView;

namespace Toolkit
{
class TOOLKIT_EXPORT NorthArrowController : public QObject
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
  NorthArrowController(QObject *parent = nullptr);
  ~NorthArrowController();

  bool setView(Esri::ArcGISRuntime::MapQuickView* mapView);
  bool setView(Esri::ArcGISRuntime::SceneQuickView* sceneView);

  double heading() const;
  bool autoHide() const;

private:
  double m_heading;
  bool m_autoHide;
  Esri::ArcGISRuntime::MapQuickView* m_mapView;
  Esri::ArcGISRuntime::SceneQuickView* m_sceneView;
};

} // Toolkit
} // ArcGISRuntime
} // Esri

#endif // NorthArrowControllerController_H
