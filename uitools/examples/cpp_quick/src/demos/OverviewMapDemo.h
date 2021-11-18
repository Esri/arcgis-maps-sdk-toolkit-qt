#ifndef ARCGIS_RUNTIME_TOOLKIT_CPP_QUICK_DEMO_OVERVIEWMAPDEMO_H
#define ARCGIS_RUNTIME_TOOLKIT_CPP_QUICK_DEMO_OVERVIEWMAPDEMO_H

#include "BaseDemo.h"

#include <QObject>
#include <QQmlEngine>

class OverviewMapDemo : public BaseDemo
{
  Q_OBJECT
  QML_ELEMENT
public:
  Q_INVOKABLE OverviewMapDemo(QObject* parent = nullptr);
  ~OverviewMapDemo() override;
};

#endif // ARCGIS_RUNTIME_TOOLKIT_CPP_QUICK_DEMO_OVERVIEWMAPDEMO_H
