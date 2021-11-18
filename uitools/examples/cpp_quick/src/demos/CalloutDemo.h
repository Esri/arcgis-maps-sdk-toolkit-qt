#ifndef ARCGIS_RUNTIME_TOOLKIT_CPP_QUICK_DEMO_CALLOUTDEMO_H
#define ARCGIS_RUNTIME_TOOLKIT_CPP_QUICK_DEMO_CALLOUTDEMO_H

#include "BaseDemo.h"

#include <QObject>
#include <QQmlEngine>

class CalloutDemo : public BaseDemo
{
  Q_OBJECT
  QML_ELEMENT
public:
  Q_INVOKABLE CalloutDemo(QObject* parent = nullptr);
  ~CalloutDemo() override;

private slots:
  void setUp();

private:
  double m_xClickLoc = 0.0;
  double m_yClickLoc = 0.0;
};

#endif // ARCGIS_RUNTIME_TOOLKIT_CPP_QUICK_DEMO_CALLOUTDEMO_H
