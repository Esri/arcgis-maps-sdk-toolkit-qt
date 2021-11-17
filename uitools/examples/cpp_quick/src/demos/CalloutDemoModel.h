#ifndef ARCGIS_RUNTIME_TOOLKIT_CPP_QUICK_DEMO_CALLOUTDEMOMODEL_H
#define ARCGIS_RUNTIME_TOOLKIT_CPP_QUICK_DEMO_CALLOUTDEMOMODEL_H

#include "BaseDemoModel.h"

#include <QObject>
#include <QQmlEngine>

class CalloutDemoModel : public BaseDemoModel
{
  Q_OBJECT
  QML_ELEMENT
public:
  CalloutDemoModel(QObject* parent = nullptr);
  ~CalloutDemoModel() override;

private slots:
  void setUp();

private:
  double m_xClickLoc = 0.0;
  double m_yClickLoc = 0.0;
};

#endif // ARCGIS_RUNTIME_TOOLKIT_CPP_QUICK_DEMO_CALLOUTDEMOMODEL_H
