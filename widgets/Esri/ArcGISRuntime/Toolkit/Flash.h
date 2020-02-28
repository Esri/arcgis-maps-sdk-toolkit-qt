#ifndef TOOLKIT_FLASH_H
#define TOOLKIT_FLASH_H

// Qt headers
#include <QWidget>

class QPropertyAnimation;

namespace Esri
{
namespace ArcGISRuntime
{
namespace Toolkit
{

class Flash : public QWidget
{
  Q_OBJECT
  Q_PROPERTY(int alpha READ alpha WRITE setAlpha NOTIFY alphaChanged)
  Q_PROPERTY(QPointF point READ point WRITE setPoint NOTIFY pointChanged)
  Q_PROPERTY(int radius READ radius WRITE setRadius NOTIFY radiusChanged)
public:

  Flash(QWidget* parent = nullptr);

  ~Flash() override;

  void setTargetColor(QColor color);

  void setPoint(QPointF color);
  QPointF point() const;

  void setRadius(int color);
  int radius() const;

  void setAlpha(int alpha);
  int alpha() const;

public slots:
  void play(int duration);

signals:
  void alphaChanged();
  void pointChanged();
  void radiusChanged();

protected:
  void paintEvent(QPaintEvent *event) override;

private:
  QColor m_color;
  QPointF m_point;
  int m_radius;
};

} // Toolkit
} // ArcGISRuntime
} // Esri

#endif // TOOLKIT_FLASH_H
