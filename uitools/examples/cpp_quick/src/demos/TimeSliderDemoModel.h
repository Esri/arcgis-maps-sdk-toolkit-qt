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
  Q_INVOKABLE TimeSliderDemoModel(QObject* parent = nullptr);
  ~TimeSliderDemoModel() override;
};

#endif // ARCGIS_RUNTIME_TOOLKIT_CPP_QUICK_DEMO_TIMESLIDERDEMOMODEL_H
