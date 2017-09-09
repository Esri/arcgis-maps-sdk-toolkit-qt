#ifndef NorthArrowController_H
#define NorthArrowController_H

#include <QObject>
#include "AbstractTool.h"

namespace Esri
{
namespace ArcGISRuntime
{

class GeoView;
class MapQuickView;
class SceneQuickView;

namespace Toolkit
{
class NorthArrowController : public QObject
{
    Q_OBJECT

    // whether compass should be visible if rotation/heading is 0
    Q_PROPERTY(bool autoHide READ autoHide WRITE setAutoHide NOTIFY autoHideChanged)

    // rotation or heading of the MapView/SceneView (respectively). Used to determine Compass rotation
    Q_PROPERTY(double heading READ heading WRITE setHeading NOTIFY headingChanged)

public:
    NorthArrowController(QObject *parent = nullptr);
    ~NorthArrowController();

    // mutators
    Q_INVOKABLE void setHeading(double rotation);
    Q_INVOKABLE void setAutoHide(bool autoHide);
    bool setView(Esri::ArcGISRuntime::MapQuickView* mapView);
    bool setView(Esri::ArcGISRuntime::SceneQuickView* sceneView);

    // accessors
    double heading() const;
    bool autoHide() const;

signals:
    void headingChanged();
    void autoHideChanged();

private:
    double m_heading = 0;
    bool m_autoHide = true; // by default, compass will be hidden if heading/rotation is 0
    Esri::ArcGISRuntime::MapQuickView* m_mapView = nullptr;
    Esri::ArcGISRuntime::SceneQuickView* m_sceneView = nullptr;
};

} // Toolkit
} // ArcGISRuntime
} // Esri

#endif // NorthArrowControllerController_H
