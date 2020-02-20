#ifndef NORTHARROWCONTROLLER_H
#define NORTHARROWCONTROLLER_H

#include <QObject>

namespace Esri {
namespace ArcGISRuntime {

class GeoView;

namespace Toolkit {

class NorthArrowController : public QObject
{
  Q_PROPERTY(QObject* geoView READ geoView WRITE setGeoView NOTIFY geoViewChanged)
  Q_PROPERTY(double heading READ heading WRITE setHeading NOTIFY headingChanged)
  Q_OBJECT

public:
  Q_INVOKABLE NorthArrowController(QObject* parent = nullptr);
  ~NorthArrowController();

  QObject* geoView() const;
  void setGeoView(QObject* mapView);

  double heading() const;
  void setHeading(double heading);

signals:
  void geoViewChanged();
  void headingChanged();

private:
  QObject* m_geoView = nullptr;
};

} // Toolkit
} // ArcGISRuntime
} // Esri

#endif // NORTHARROWCONTROLLER_H
