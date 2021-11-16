#ifndef ARCGIS_RUNTIME_TOOLKIT_CPP_QUICK_DEMO_SEARCHVIEWDEMOMODEL_H
#define ARCGIS_RUNTIME_TOOLKIT_CPP_QUICK_DEMO_SEARCHVIEWDEMOMODEL_H

#include "BaseDemoModel.h"

#include <QObject>
#include <QQmlEngine>

class SearchViewDemoModel : public BaseDemoModel
{
  Q_OBJECT
  QML_ELEMENT
public:
  SearchViewDemoModel(QObject* parent = nullptr);
  ~SearchViewDemoModel() override;
};

#endif
