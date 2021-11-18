#ifndef ARCGIS_RUNTIME_TOOLKIT_CPP_QUICK_DEMO_NORTHARROWDEMO_H
#define ARCGIS_RUNTIME_TOOLKIT_CPP_QUICK_DEMO_NORTHARROWDEMO_H

#include "BaseDemo.h"

#include <QObject>
#include <QQmlEngine>

class NorthArrowDemo : public BaseDemo
{
  Q_OBJECT
  QML_ELEMENT
public:
  Q_INVOKABLE NorthArrowDemo(QObject* parent = nullptr);
  ~NorthArrowDemo() override;
};

#endif // ARCGIS_RUNTIME_TOOLKIT_CPP_QUICK_DEMO_NORTHARROWDEMO_H
