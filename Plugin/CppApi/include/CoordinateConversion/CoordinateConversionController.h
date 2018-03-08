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

class QMouseEvent;

namespace Esri
{
namespace ArcGISRuntime
{
  class GeoView;

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

  // set the input format etc.
  Q_PROPERTY(QString inputFormat READ inputFormat WRITE setInputFormat NOTIFY inputFormatChanged)
  Q_PROPERTY(QStringList coordinateFormats READ coordinateFormats NOTIFY coordinateFormatsChanged)
  Q_PROPERTY(QString pointToConvert READ pointToConvert NOTIFY pointToConvertChanged)

  // whether the tool is in "capture mode" (sets the target to a clicked point) or "live" mode (uses current location)
  Q_PROPERTY(bool captureMode READ isCaptureMode WRITE setCaptureMode NOTIFY captureModeChanged)

  Q_PROPERTY(QObject* geoView READ geoView WRITE setGeoView NOTIFY geoViewChanged)

public:

  // convert the following notation using the input options specified
  Q_INVOKABLE void convertNotation(const QString& notation);

  // convert the previously passed in point
  Q_INVOKABLE void convertPoint();

  // copy contents to the clipboard
  Q_INVOKABLE void copyToClipboard(const QString& text);

  // get the current input position in screen coordinates
  Q_INVOKABLE QPointF screenCoordinate(double screenWidth, double screenHeight) const;

  // zooms the geoview to the current input position
  Q_INVOKABLE void zoomTo();

  // clear the current set of results
  Q_INVOKABLE void clearResults();

  // adds the specified format to the set of results the tool will produce
  Q_INVOKABLE void addCoordinateFormat(const QString& newFormat);

  // remove the specified format from the set of results the tool will produce
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
  void geoViewChanged();

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

  QObject* geoView() const;
  void setGeoView(QObject* geoView);

public slots:

  void handleMouseClicked(QMouseEvent& clickedPoint);
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
  Esri::ArcGISRuntime::GeoView* m_geoView;
};

} // Toolkit
} // ArcGISRuntime
} // Esri

#endif // COORDINATECONVERSIONCONTROLLER_H
