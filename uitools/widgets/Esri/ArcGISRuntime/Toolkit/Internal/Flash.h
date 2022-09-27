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
#ifndef ESRI_ARCGISRUNTIME_TOOLKIT_INTERNAL_FLASH_H
#define ESRI_ARCGISRUNTIME_TOOLKIT_INTERNAL_FLASH_H

// Qt headers
#include <QWidget>

class QPropertyAnimation;

namespace Esri::ArcGISRuntime::Toolkit {

class Flash : public QWidget
{
  Q_OBJECT
  Q_PROPERTY(int alpha READ alpha_ WRITE setAlpha_ NOTIFY alphaChanged)
  Q_PROPERTY(QPointF point READ point WRITE setPoint NOTIFY pointChanged)
  Q_PROPERTY(int radius READ radius WRITE setRadius NOTIFY radiusChanged)
public:

  Flash(QWidget* parent = nullptr);

  ~Flash() override;

  void setTargetColor(QColor color);

  void setPoint(QPointF point);

  QPointF point() const;

  void setRadius(int radius);

  int radius() const;

public slots:
  void play(int duration);

signals:
  void alphaChanged();

  void pointChanged();

  void radiusChanged();

protected:
  void paintEvent(QPaintEvent* event) override;

private:
  void setAlpha_(int alpha);

  int alpha_() const;

private:
  QColor m_color;
  QPointF m_point;
  int m_radius = 0;
};

} // Esri::ArcGISRuntime::Toolkit

#endif // ESRI_ARCGISRUNTIME_TOOLKIT_INTERNAL_FLASH_H
