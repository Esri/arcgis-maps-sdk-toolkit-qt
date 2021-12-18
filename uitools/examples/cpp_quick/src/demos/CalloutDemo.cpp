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
#include "CalloutDemo.h"

#include "GeoView.h"
#include "MapQuickView.h"
#include "MapView.h"
#include "SceneQuickView.h"

using namespace Esri::ArcGISRuntime;

CalloutDemo::CalloutDemo(QObject* parent) :
  BaseDemo(parent)
{
  connect(this, &BaseDemo::geoViewChanged, this, &CalloutDemo::setUp);
}

CalloutDemo::~CalloutDemo()
{
}

void CalloutDemo::setUp()
{
  //initialize the callout
  MapQuickView* mv = static_cast<MapQuickView*>(geoView());
  CalloutData* callData = mv->calloutData();
  callData->setTitle("This is the location callout");
  callData->setDetail(QString("x: %1 y: %2").arg(m_xClickLoc, m_yClickLoc));
  QImage img(QStringLiteral(":/esri.com/imports/Esri/ArcGISRuntime/Toolkit/images/map_pin.png"));
  //callData->setImage(img);

  connect(mv, &MapQuickView::mouseClicked, this, [mv](QMouseEvent& mouse)
          {
            // check the geoView passed is a mapView, if so modify data
            auto callData = mv->calloutData();
            if (callData->isVisible())
              callData->setVisible(false);
            else
            {
              Point mapPoint(mv->screenToLocation(mouse.x(), mouse.y()));
              //atm the position is not working correctly with the setLocation
              callData->setLocation(mapPoint);
              callData->setDetail("x: " + QString::number(mouse.x()) + " y: " + QString::number(mouse.y()));
              callData->setVisible(true);
            }
          });
}
