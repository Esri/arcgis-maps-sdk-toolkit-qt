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
#include "Flash.h"

// Qt headers
#include <QPainter>
#include <QPropertyAnimation>
#include <QTimer>

namespace Esri
{
namespace ArcGISRuntime
{
namespace Toolkit
{

/*!
  \internal
  \class Esri::ArcGISRuntime::Toolkit::Flash
  \inmodule EsriArcGISRuntimeToolkit
  \brief a \c Flash just exists to display a flashing blue dot on a map
  for the \c CoordinateConversion tool, then delete itself after its animation
  is complete.
 */

/*!
  \brief Constructor
  \list
  \li \a parent Parent widget
  \endlist
 */
Flash::Flash(QWidget* parent):
  QWidget(parent)
{
  setAttribute(Qt::WA_TranslucentBackground, true);
}

/*!
  \brief Destructor.
 */
Flash::~Flash()
{
}

/*!
  \brief Paints this widget
  \list
    \li \a event paintEvent. Not used.
  \endlist
 */
void Flash::paintEvent(QPaintEvent* /*event*/)
{
  QPainter painter(this);
  painter.setBrush(QBrush(m_color, Qt::SolidPattern));
  painter.setPen(Qt::NoPen);
  painter.drawEllipse(m_point, m_radius, m_radius);
}

/*!
  \brief Set color this object will flash as.
  \list
    \li \a color Color to set. Alpha value is overridden.
  \endlist
 */
void Flash::setTargetColor(QColor color)
{
  m_color = std::move(color);
}

/*! \brief Sets the current \a alpha (for animation) */
void Flash::setAlpha_(int alpha)
{
  if (m_color.alpha() == alpha)
    return;

  m_color.setAlpha(alpha);
  emit alphaChanged();
  update();
}


/*! \brief Returns the current alpha (for animation) */
int Flash::alpha_() const
{
  return m_color.alpha();
}

/*!
  \brief Set the point on the screen the flash will appear relative to the
  parent.
  \list
    \li \a point Point to appear. 
  \endlist
 */
void Flash::setPoint(QPointF point)
{
  if (m_point == point)
    return;

  m_point = std::move(point);
  emit pointChanged();
  update();
}

/*!
  \brief Returns the point this image appears at.
 */
QPointF Flash::point() const
{
  return m_point;
}

/*!
  \brief Set the radius of the circle of this flash.
  \list
    \li \a radius Size of radius.
  \endlist
 */
void Flash::setRadius(int radius)
{
  if (m_radius == radius)
    return;

  m_radius = radius;
  emit radiusChanged();
  update();
}

/*!
  \brief Returns the radius of this circle.
 */
int Flash::radius() const
{
  return m_radius;
}

/*!
  \brief When called will begin animating this flash.
  
  After the animation is finished this object will delete itself.
  
  \list
    \li \a duration Lifetime of animation in ms.
  \endlist
 */
void Flash::play(int duration)
{
  auto animation = new QPropertyAnimation(this, "alpha", this);
  animation->setKeyValues(QVariantAnimation::KeyValues { {0, 0}, {0.5, 255}, {1.0, 0} });
  animation->setDuration(duration);
  connect(animation, &QPropertyAnimation::finished, this, &QObject::deleteLater);
  animation->start();
}

/*! 
  \fn void Esri::ArcGISRuntime::Toolkit::Flash::alphaChanged()
  \brief emitted when the \c alpha changes. 
*/

/*! 
  \fn void Esri::ArcGISRuntime::Toolkit::Flash::pointChanged()
  \brief emitted when the \c point changes. 
 */

/*! 
  \fn void Esri::ArcGISRuntime::Toolkit::Flash::radiusChanged()
  \brief emitted when the \c radius changes.
 */

/*!
 \property Esri::ArcGISRuntime::Toolkit::Flash::alpha
 */

/*!
 \property Esri::ArcGISRuntime::Toolkit::Flash::point
 */

/*!
 \property Esri::ArcGISRuntime::Toolkit::Flash::radius
 */

} // Toolkit
} // ArcGISRuntime
} // Esri
