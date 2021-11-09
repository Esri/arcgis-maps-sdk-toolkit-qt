#include "CalloutDemoModel.h"
#include "GeoView.h"
#include "MapQuickView.h"
#include "MapView.h"
#include "SceneQuickView.h"

using namespace Esri::ArcGISRuntime;

CalloutDemoModel::CalloutDemoModel(QObject *parent)
    : BaseDemoModel(parent), m_xClickLoc(), m_yClickLoc()
{
    connect(this, &BaseDemoModel::geoViewChanged, this, &CalloutDemoModel::setUp);
}

CalloutDemoModel::~CalloutDemoModel() {}

void CalloutDemoModel::setUp()
{
    //initialize the callout
    MapQuickView *mw = dynamic_cast<MapQuickView *>(geoView());
    CalloutData *callData = mw->calloutData();
    callData->setTitle("This is the location callout");
    callData->setDetail("x: " + QString::number(m_xClickLoc)
                        + " y: " + QString::number(m_yClickLoc));
    QImage img(QStringLiteral(":/esri.com/imports/Esri/ArcGISRuntime/Toolkit/images/map_pin.png"));
    callData->setImage(img);

    //connect to the mouseclick signal by casting geoview into child type that supports signals for mouse events
    apply([this](auto geoView) {
        using ViewType = std::remove_pointer_t<decltype(geoView)>;
        connect(geoView, &ViewType::mouseClicked, this, [geoView](QMouseEvent &mouse) {
            // check the geoView passed is a mapView, if so modify data
            if (auto mapViewQuick = qobject_cast<MapQuickView *>(geoView)) {
                auto callData = mapViewQuick->calloutData();
                if (callData->isVisible())
                    callData->setVisible(false);
                else {
                    Point mapPoint(mapViewQuick->screenToLocation(mouse.x(),mouse.y()));
                    //atm the position is not working correctly with the setLocation
                    callData->setLocation(mapPoint);
                    callData->setDetail("x: " + QString::number(mouse.x())
                                        + " y: " + QString::number(mouse.y()));
                    callData->setVisible(true);
                }
            } else
                qDebug() << "can't show callout on non MapView elemement";
        });
    });
}
