#include "Camera.h"
#include "MapQuickView.h"
#include "SceneQuickView.h"
#include "NorthArrowController.h"

namespace Esri
{
namespace ArcGISRuntime
{
namespace Toolkit
{

NorthArrowController::NorthArrowController(QObject *parent) : QObject(parent)
{
}

NorthArrowController::~NorthArrowController()
{
}

void NorthArrowController::setHeading(double rotation)
{
    // Not sure if this is a good way to do this because the heading will be changed in the signal handling
    if (m_mapView)
        m_mapView->setViewpointRotation(rotation);
    else if (m_sceneView)
    {
        Esri::ArcGISRuntime::Camera currentCamera = m_sceneView->currentViewpointCamera();
        Esri::ArcGISRuntime::Camera updatedCamera = currentCamera.rotateTo(rotation, currentCamera.pitch(), currentCamera.roll());
        m_sceneView->setViewpointCamera(updatedCamera);
    }
    else
        return;
}

void NorthArrowController::setAutoHide(bool autoHide)
{
    m_autoHide = autoHide;
    emit autoHideChanged();
}

bool NorthArrowController::setView(Esri::ArcGISRuntime::MapQuickView* mapView)
{
    if (!mapView)
        return false;

    // NorthArrowController can only be set to one MapView or SceneView
    // might change this restriction later
    if (m_sceneView != nullptr || m_mapView != nullptr)
        return false;

    // set mapView
    m_mapView = mapView;
    m_heading = mapView->mapRotation();

    connect(m_mapView, &Esri::ArcGISRuntime::MapQuickView::mapRotationChanged, this,
    [this]()
    {
        m_heading = m_mapView->mapRotation();
        emit headingChanged();
    });

    return true;
}

bool NorthArrowController::setView(Esri::ArcGISRuntime::SceneQuickView* sceneView)
{
    if (!sceneView)
        return false;

    // NorthArrowController can only be set to one MapView or SceneView
    // might change this restriction later
    if (m_sceneView != nullptr || m_mapView != nullptr)
        return false;

    // set SceneView
    m_sceneView = sceneView;
    m_heading = m_sceneView->currentViewpointCamera().heading();

    connect(m_sceneView, &Esri::ArcGISRuntime::SceneQuickView::viewpointChanged, this,
    [this]()
    {
        m_heading = m_sceneView->currentViewpointCamera().heading();
        emit headingChanged();
    });

    return true;
}

double NorthArrowController::heading() const
{
    return m_heading;
}

bool NorthArrowController::autoHide() const
{
    return m_autoHide;
}

} // Toolkit
} // ArcGISRuntime
} // Esri
