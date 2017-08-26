// Copyright 2016 ESRI
//
// All rights reserved under the copyright laws of the United States
// and applicable international laws, treaties, and conventions.
//
// You may freely redistribute and use this sample code, with or
// without modification, provided you include the original copyright
// notice and use restrictions.
//
// See the Sample code usage restrictions document for further information.
//

#ifndef ArcGISRuntimeSolutions_H
#define ArcGISRuntimeSolutions_H

#include <QQmlExtensionPlugin>

namespace Esri
{
namespace ArcGISRuntime
{
namespace Solutions
{

class ArcGISRuntimeSolutions : public QQmlExtensionPlugin
{
  Q_OBJECT
  Q_PLUGIN_METADATA(IID QQmlExtensionInterface_iid)

public:
  explicit ArcGISRuntimeSolutions(QObject* parent = nullptr);

  void registerTypes(const char* uri);
};

} // Solutions
} // ArcGISRuntime
} // Esri

#endif // ArcGISRuntimeSolutions_H
