#include "Flash.h"

// Qt headers
#include <QPainter>
#include <QPropertyAnimation>
#include <QTimer>

#include <QDebug>

namespace Esri
{
namespace ArcGISRuntime
{
namespace Toolkit
{

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
  m_color = color;
}

void Flash::setAlpha(int alpha)
{
  if (m_color.alpha() == alpha)
    return;
  m_color.setAlpha(alpha);
  emit alphaChanged();
  update();
}

int Flash::alpha() const
{
  return m_color.alpha();
}

void Flash::setPoint(QPointF point)
{
  if (m_point == point)
    return;

  m_point = point;
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

} // Toolkit
} // ArcGISRuntime
} // Esri
