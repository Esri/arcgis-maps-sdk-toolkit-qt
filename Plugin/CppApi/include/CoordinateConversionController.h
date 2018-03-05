// Copyright 2017 ESRI
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

#include "AbstractTool.h"

#include "SpatialReference.h"
#include "Point.h"
#include "GeometryTypes.h"

#include <QAbstractListModel>
#include <QPointF>

namespace Esri
{
namespace ArcGISRuntime
{
namespace Toolkit
{

class CoordinateConversionOptions;
class CoordinateConversionResults;

class TOOLKIT_EXPORT CoordinateConversionController : public AbstractTool
{
  Q_OBJECT

  // bind to the results, which will have "name" and "notation" roles
  Q_PROPERTY(QAbstractListModel* results READ results NOTIFY resultsChanged)

  // whether the controller should immediately convert on calling setPointToConvert or just
  // store the point for later
  Q_PROPERTY(bool runConversion READ runConversion WRITE setRunConversion NOTIFY runConversionChanged)

  // set the input mode and any corresponding conversion options
  Q_PROPERTY(QString inputFormat READ inputFormat WRITE setInputFormat NOTIFY inputFormatChanged)
  Q_PROPERTY(QStringList coordinateFormats READ coordinateFormats NOTIFY coordinateFormatsChanged)
  Q_PROPERTY(QString pointToConvert READ pointToConvert NOTIFY pointToConvertChanged)

  Q_PROPERTY(bool captureMode READ isCaptureMode WRITE setCaptureMode NOTIFY captureModeChanged)

public:

  // convert the following notation using the input options specified
  Q_INVOKABLE void convertNotation(const QString& notation);

  // convert the previously passed in point
  Q_INVOKABLE void convertPoint();

  // copy contents to the clipboard
  Q_INVOKABLE void copyToClipboard(const QString& text);

  // clear the current set of results
  Q_INVOKABLE QPointF screenCoordinate(double screenWidth, double screenHeight) const;
  Q_INVOKABLE void clearResults();

  Q_INVOKABLE void addCoordinateFormat(const QString& newFormat);
  Q_INVOKABLE void removeCoordinateFormat(const QString& formatToRemove);

signals:
  void optionsChanged();
  void resultsChanged();
  void inputModeChanged();
  void runConversionChanged();
  void pointToConvertChanged();
  void coordinateFormatsChanged();
  void inputFormatChanged();
  void captureModeChanged();

public:
  CoordinateConversionController(QObject* parent = nullptr);
  ~CoordinateConversionController();

  void setSpatialReference(const Esri::ArcGISRuntime::SpatialReference& spatialReference);
  void setPointToConvert(const Esri::ArcGISRuntime::Point& point);

  bool runConversion() const;
  void setRunConversion(bool runConversion);

  void addOption(CoordinateConversionOptions* option);
  void clearOptions();

  QAbstractListModel* results();

  QString toolName() const override;

  void setProperties(const QVariantMap& properties) override;

  QString pointToConvert() const;

  QStringList coordinateFormats() const;

  QString inputFormat() const;
  void setInputFormat(const QString &inputFormat);

  bool isCaptureMode() const;
  void setCaptureMode(bool captureMode);

public slots:
  void onMouseClicked(const Esri::ArcGISRuntime::Point& clickedPoint);
  void onLocationChanged(const Esri::ArcGISRuntime::Point& location);

private:
  CoordinateConversionResults* resultsInternal();

  Esri::ArcGISRuntime::Point pointFromNotation(const QString& incomingNotation);
  QString convertPointInternal(CoordinateConversionOptions* option, const Esri::ArcGISRuntime::Point& point) const;

  bool isInputFormat(CoordinateConversionOptions* option) const;
  bool isFormat(CoordinateConversionOptions* option, const QString& formatName) const;

  Esri::ArcGISRuntime::Point m_pointToConvert;
  Esri::ArcGISRuntime::SpatialReference m_spatialReference;
  CoordinateConversionResults* m_results = nullptr;

  QList<CoordinateConversionOptions*> m_options;
  bool m_runConversion = true;

  QStringList m_coordinateFormats;
  QString m_inputFormat;
  bool m_captureMode = false;
};

} // Toolkit
} // ArcGISRuntime
} // Esri

#endif // COORDINATECONVERSIONCONTROLLER_H
