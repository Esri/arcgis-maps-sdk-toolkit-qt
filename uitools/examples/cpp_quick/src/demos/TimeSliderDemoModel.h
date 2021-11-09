#ifndef ARCGIS_RUNTIME_TOOLKIT_CPP_QUICK_DEMO_TIMESLIDERDEMOMODEL_H
#define ARCGIS_RUNTIME_TOOLKIT_CPP_QUICK_DEMO_TIMESLIDERDEMOMODEL_H

#include "BaseDemoModel.h"

#include <QObject>
#include <QQmlEngine>

class TimeSliderDemoModel : public BaseDemoModel
{
    Q_OBJECT
    QML_ELEMENT
public:
    TimeSliderDemoModel(QObject *parent = nullptr);
    ~TimeSliderDemoModel() override;

protected:
    Esri::ArcGISRuntime::Map *initMap_(QObject *parent) const override;
    Esri::ArcGISRuntime::Scene *initScene_(QObject *parent) const override;
};

#endif // ARCGIS_RUNTIME_TOOLKIT_CPP_QUICK_DEMO_TIMESLIDERDEMOMODEL_H
