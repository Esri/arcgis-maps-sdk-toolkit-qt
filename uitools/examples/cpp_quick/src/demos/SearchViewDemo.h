#ifndef ARCGIS_RUNTIME_TOOLKIT_CPP_QUICK_DEMO_SEARCHVIEWDEMO_H
#define ARCGIS_RUNTIME_TOOLKIT_CPP_QUICK_DEMO_SEARCHVIEWDEMO_H

#include "BaseDemo.h"

#include <QObject>
#include <QQmlEngine>

class SearchViewDemo : public BaseDemo
{
  Q_OBJECT
  QML_ELEMENT
public:
  Q_INVOKABLE SearchViewDemo(QObject* parent = nullptr);
  ~SearchViewDemo() override;
};

#endif // ARCGIS_RUNTIME_TOOLKIT_CPP_QUICK_DEMO_SEARCHVIEWDEMO_H
