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
  QImage img(QStringLiteral(":/esri.com/imports/Esri/ArcGISRuntime/Toolkit/images/pin-tear.svg"));
  callData->setImage(img);

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
