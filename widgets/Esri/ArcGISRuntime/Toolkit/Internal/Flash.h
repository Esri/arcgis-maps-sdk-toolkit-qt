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

namespace Esri
{
namespace ArcGISRuntime
{
namespace Toolkit
{

/*!
 * \internal
 * \brief a Flash just exists to display a flashing blue dot on a map
 * for the CoordinateConversion tool, then delete itself after its animation
 * is complete.
 */
class Flash : public QWidget
{
  Q_OBJECT
  Q_PROPERTY(int alpha READ alpha_ WRITE setAlpha_ NOTIFY alphaChanged)
  Q_PROPERTY(QPointF point READ point WRITE setPoint NOTIFY pointChanged)
  Q_PROPERTY(int radius READ radius WRITE setRadius NOTIFY radiusChanged)
public:

  /*!
   * \brief Constructor
   * \param parent Parent widget
   */
  Flash(QWidget* parent = nullptr);

  /*!
   * \brief Destructor.
   */
  ~Flash() override;

  /*!
   * \brief Set color this object will flash as.
   * \param color Color to set. Alpha value is overridden.
   */
  void setTargetColor(QColor color);

  /*!
   * \brief Set the point on the screen the flash will appear relative to the
   * parent.
   * \param point Point to appear. 
   */
  void setPoint(QPointF point);

  /*!
   * \brief Returns the point this image appears at.
   */
  QPointF point() const;

  /*!
   * \brief Set the radius of the circle of this flash.
   * \param radius Size of radius.
   */
  void setRadius(int radius);

  /*!
   * \brief Returns the radius of this circle.
   */
  int radius() const;

public slots:
  /*!
   * \brief When called will begin animating this flash.
   * 
   * After the animation is finished this object will delete itself.
   * 
   * \param duration Lifetime of animation in ms.
   */
  void play(int duration);

signals:
  /*! \brief emitted when the alpha changes. */
  void alphaChanged();

  /*! \brief emitted when the point changes. */
  void pointChanged();

  /*! \brief emitted when the radius changes. */
  void radiusChanged();

protected:
  /*!
   * \brief Paints this widget
   * \param event paintEvent. Not used.
   */
  void paintEvent(QPaintEvent *event) override;

private:
  /*! \brief Sets the current alpha (for animation) */
  void setAlpha_(int alpha);

  /*! \brief Gets the current alpha (for animation) */
  int alpha_() const;

private:
  QColor m_color;
  QPointF m_point;
  int m_radius = 0;
};

} // Toolkit
} // ArcGISRuntime
} // Esri

#endif // ESRI_ARCGISRUNTIME_TOOLKIT_INTERNAL_FLASH_H
