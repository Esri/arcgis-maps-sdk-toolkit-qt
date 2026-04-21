
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

#include "CalloutData.h"
#include "GeoView.h"
#include "LocalSceneQuickView.h"
#include "MapQuickView.h"
#include "Point.h"
#include "SceneQuickView.h"

#include <QImage>
#include <QMouseEvent>

using namespace Esri::ArcGISRuntime;

namespace
{
  Point screenToCalloutLocation(MapQuickView* geoView, const QPointF& clickPosition)
  {
    return geoView->screenToLocation(clickPosition.x(), clickPosition.y());
  }

  Point screenToCalloutLocation(SceneQuickView* geoView, const QPointF& clickPosition)
  {
    return geoView->screenToBaseSurface(clickPosition.x(), clickPosition.y());
  }

  Point screenToCalloutLocation(LocalSceneQuickView* geoView, const QPointF& clickPosition)
  {
    return geoView->screenToBaseSurface(clickPosition.x(), clickPosition.y());
  }
} // namespace

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
  apply([this](auto* geoView)
  {
    using ViewType = std::remove_pointer_t<decltype(geoView)>;

    auto* calloutData = geoView->calloutData();
    calloutData->setTitle("This is the location callout");
    calloutData->setImage(QImage(QStringLiteral(":/Esri/ArcGISRuntime/Toolkit/pin-tear.svg")));

    connect(geoView, &ViewType::mouseClicked, this, [geoView](QMouseEvent& mouse)
    {
      auto* viewCalloutData = geoView->calloutData();
      if (viewCalloutData->isVisible())
      {
        viewCalloutData->setVisible(false);
      }
      else
      {
        const auto clickPosition = mouse.position();
        const Point mapPoint(screenToCalloutLocation(geoView, clickPosition));
        viewCalloutData->setLocation(mapPoint);
        viewCalloutData->setDetail(QString("x: %1 y: %2").arg(QString::number(clickPosition.x()), QString::number(clickPosition.y())));
        viewCalloutData->setVisible(true);
      }
    });
  });
}
