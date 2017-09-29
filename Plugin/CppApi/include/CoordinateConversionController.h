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

#ifndef COORDINATECONVERSIONCONTROLLER_H
#define COORDINATECONVERSIONCONTROLLER_H

#include <QQmlListProperty>

#include "AbstractTool.h"
#include "CoordinateConversionOptions.h"

#include "SpatialReference.h"
#include "Point.h"
#include "GeometryTypes.h"

namespace Esri
{
namespace ArcGISRuntime
{
namespace Toolkit
{

class CoordinateConversionResults;

class TOOLKIT_EXPORT CoordinateConversionController : public AbstractTool
{
  Q_OBJECT

  // set the options which will determine how many outputs there are and the formats
  Q_PROPERTY(QQmlListProperty<CoordinateConversionOptions> options READ options NOTIFY optionsChanged)

  // bind to the results, which will have "name" and "notation" roles
  Q_PROPERTY(CoordinateConversionResults* results READ results NOTIFY resultsChanged)

  // whether the controller should immediately convert on calling setPointToConvert or just
  // store the point for later
  Q_PROPERTY(bool runConversion READ runConversion WRITE setRunConversion NOTIFY runConversionChanged)

  // set the input mode and any corresponding conversion options
  Q_PROPERTY(CoordinateConversionOptions::CoordinateType inputMode READ inputMode WRITE setInputMode NOTIFY inputModeChanged)
  Q_PROPERTY(CoordinateConversionOptions::GarsConversionMode inputGarsConversionMode READ inputGarsConversionMode WRITE setInputGarsConversionMode NOTIFY inputGarsConversionModeChanged)
  Q_PROPERTY(CoordinateConversionOptions::MgrsConversionMode inputMgrsConversionMode READ inputMgrsConversionMode WRITE setInputMgrsConversionMode NOTIFY inputMgrsConversionModeChanged)
  Q_PROPERTY(CoordinateConversionOptions::UtmConversionMode inputUtmConversionMode READ inputUtmConversionMode WRITE setInputUtmConversionMode NOTIFY inputUtmConversionModeChanged)

  // internal: support for nested default property "options" objects
  Q_PRIVATE_PROPERTY(CoordinateConversionController::self(), QQmlListProperty<QObject> objects READ objects DESIGNABLE false)
  Q_CLASSINFO("DefaultProperty", "objects")

public:
  // convert the following notation using the input options specified
  Q_INVOKABLE void convertNotation(const QString& notation);

  // convert the previously passed in point
  Q_INVOKABLE void convertPoint();

  // copy contents to the clipboard
  Q_INVOKABLE void copyToClipboard(const QString& text);

signals:
  void optionsChanged();
  void resultsChanged();
  void inputModeChanged();
  void inputGarsConversionModeChanged();
  void inputMgrsConversionModeChanged();
  void inputUtmConversionModeChanged();
  void runConversionChanged();

public:
  CoordinateConversionController(QObject* parent = nullptr);
  ~CoordinateConversionController();

  CoordinateConversionOptions::CoordinateType inputMode() const;
  void setInputMode(CoordinateConversionOptions::CoordinateType inputMode);

  void setSpatialReference(const Esri::ArcGISRuntime::SpatialReference& spatialReference);
  void setPointToConvert(const Esri::ArcGISRuntime::Point& point);

  CoordinateConversionOptions::GarsConversionMode inputGarsConversionMode() const;
  void setInputGarsConversionMode(CoordinateConversionOptions::GarsConversionMode inputGarsConversionMode);

  CoordinateConversionOptions::MgrsConversionMode inputMgrsConversionMode() const;
  void setInputMgrsConversionMode(CoordinateConversionOptions::MgrsConversionMode inputMgrsConversionMode);

  CoordinateConversionOptions::UtmConversionMode inputUtmConversionMode() const;
  void setInputUtmConversionMode(CoordinateConversionOptions::UtmConversionMode inputUtmConversionMode);

  bool runConversion() const;
  void setRunConversion(bool runConversion);

  void addOption(CoordinateConversionOptions* option);
  void clearOptions();

  CoordinateConversionResults* results();

  QString toolName() const override;
  bool handleClick(const Point& pos) override;

private:
  QQmlListProperty<CoordinateConversionOptions> options();

  Esri::ArcGISRuntime::Point pointFromNotation(const QString& incomingNotation);
  QString convertPointInternal(CoordinateConversionOptions* option, const Esri::ArcGISRuntime::Point& point);

  CoordinateConversionController* self() { return this; }
  QQmlListProperty<QObject> objects();
  static void objectAppend(QQmlListProperty<QObject>* property, QObject* value);

  CoordinateConversionOptions::CoordinateType m_inputMode = CoordinateConversionOptions::CoordinateTypeUsng;
  Esri::ArcGISRuntime::Point m_pointToConvert;
  Esri::ArcGISRuntime::SpatialReference m_spatialReference;
  CoordinateConversionResults* m_results = nullptr;

  CoordinateConversionOptions::GarsConversionMode m_inputGarsConversionMode = CoordinateConversionOptions::GarsConversionModeCenter;
  CoordinateConversionOptions::MgrsConversionMode m_inputMgrsConversionMode = CoordinateConversionOptions::MgrsConversionModeAutomatic;
  CoordinateConversionOptions::UtmConversionMode  m_inputUtmConversionMode  = CoordinateConversionOptions::UtmConversionModeLatitudeBandIndicators;

  QList<CoordinateConversionOptions*> m_options;
  bool m_runConversion = false;
};

} // Toolkit
} // ArcGISRuntime
} // Esri

#endif // COORDINATECONVERSIONCONTROLLER_H
