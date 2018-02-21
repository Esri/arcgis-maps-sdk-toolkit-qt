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

#ifndef ABSTRACT_TOOL_H
#define ABSTRACT_TOOL_H

#include "ToolkitCommon.h"

#include <QObject>
#include <QString>
#include <QVariantMap>

namespace Esri
{
namespace ArcGISRuntime
{

class Error;
class Point;

namespace Toolkit
{

class TOOLKIT_EXPORT AbstractTool : public QObject
{
  Q_OBJECT

  Q_PROPERTY(bool active READ isActive WRITE setActive NOTIFY activeChanged)

public:
  AbstractTool(QObject* parent = nullptr);
  virtual ~AbstractTool();

  virtual QString toolName() const = 0;
  virtual bool handleClick(const Point& pos);

  virtual void setProperties(const QVariantMap& properties);

  virtual void setActive(bool active);
  bool isActive() const;

signals:
  void activeChanged();
  void errorOccurred(const Error& error);
  void propertyChanged(const QString& propertyName, const QVariant& propertyValue);
  void toolErrorOccurred(const QString& errorMessage, const QString& additionalMessage);

protected:
  bool m_active = false;
};

} // Toolkit
} // ArcGISRuntime
} // Esri

#endif // ABSTRACT_TOOL_H
