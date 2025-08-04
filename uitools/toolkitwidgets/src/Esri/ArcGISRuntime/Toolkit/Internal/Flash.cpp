
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

namespace Esri::ArcGISRuntime::Toolkit {

/*!
  \internal
  This class is an internal implementation detail and is subject to change.
*/

Flash::Flash(QWidget* parent):
  QWidget(parent)
{
  setAttribute(Qt::WA_TranslucentBackground, true);
}

Flash::~Flash()
{
}

void Flash::paintEvent(QPaintEvent* /*event*/)
{
  QPainter painter(this);
  painter.setBrush(QBrush(m_color, Qt::SolidPattern));
  painter.setPen(Qt::NoPen);
  painter.drawEllipse(m_point, m_radius, m_radius);
}

void Flash::setTargetColor(QColor color)
{
  m_color = std::move(color);
}

void Flash::setAlpha_(int alpha)
{
  if (m_color.alpha() == alpha)
    return;

  m_color.setAlpha(alpha);
  emit alphaChanged();
  update();
}

int Flash::alpha_() const
{
  return m_color.alpha();
}

void Flash::setPoint(QPointF point)
{
  if (m_point == point)
    return;

  m_point = std::move(point);
  emit pointChanged();
  update();
}

QPointF Flash::point() const
{
  return m_point;
}

void Flash::setRadius(int radius)
{
  if (m_radius == radius)
    return;

  m_radius = radius;
  emit radiusChanged();
  update();
}

int Flash::radius() const
{
  return m_radius;
}

void Flash::play(int duration)
{
  auto animation = new QPropertyAnimation(this, "alpha", this);
  animation->setKeyValues(QVariantAnimation::KeyValues { {0, 0}, {0.5, 255}, {1.0, 0} });
  animation->setDuration(duration);
  connect(animation, &QPropertyAnimation::finished, this, &QObject::deleteLater);
  animation->start();
}

} // Esri::ArcGISRuntime::Toolkit
