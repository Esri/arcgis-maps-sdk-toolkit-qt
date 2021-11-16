#ifndef ARCGIS_RUNTIME_TOOLKIT_CPP_QUICK_DEMO_OVERVIEWMAPDEMOMODEL_H
#define ARCGIS_RUNTIME_TOOLKIT_CPP_QUICK_DEMO_OVERVIEWMAPDEMOMODEL_H

#include "BaseDemoModel.h"

#include <QObject>
#include <QQmlEngine>

class OverviewMapDemoModel : public BaseDemoModel
{
  Q_OBJECT
  QML_ELEMENT
public:
  OverviewMapDemoModel(QObject* parent = nullptr);
  ~OverviewMapDemoModel() override;
};

#endif // ARCGIS_RUNTIME_TOOLKIT_CPP_QUICK_DEMO_OVERVIEWMAPDEMOMODEL_H
