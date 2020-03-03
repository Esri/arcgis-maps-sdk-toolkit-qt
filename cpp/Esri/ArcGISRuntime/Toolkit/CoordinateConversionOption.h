/*******************************************************************************
 *  Copyright 2012-2020 Esri
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 ******************************************************************************/
#ifndef ESRI_ARCGISRUNTIME_TOOLKIT_COORDINATECONVERSIONOPTION_H
#define ESRI_ARCGISRUNTIME_TOOLKIT_COORDINATECONVERSIONOPTION_H

// Qt headers
#include <QObject>

// ArcGISRuntime headers
#include <GeometryTypes.h>
#include <Point.h>
#include <SpatialReference.h>

namespace Esri
{
namespace ArcGISRuntime
{

namespace Toolkit
{

/*!
 * \brief a CoordinateConversionOption is a collection of properties that
 * dictates how a Point should be converted to and from a string.
 * 
 * a CoordinateConversionOption is able to convert between Point <--> QString
 * using the formatting options it is currently set with.
 */
class CoordinateConversionOption : public QObject
{
  Q_OBJECT
  Q_CLASSINFO("RegisterEnumClassesUnscoped", "false")
  Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
  Q_PROPERTY(CoordinateType outputMode READ outputMode WRITE setOutputMode NOTIFY outputModeChanged)
  Q_PROPERTY(bool addSpaces READ addSpaces WRITE setAddSpaces NOTIFY addSpacesChanged)
  Q_PROPERTY(int precision READ precision WRITE setPrecision NOTIFY precisionChanged)
  Q_PROPERTY(int decimalPlaces READ decimalPlaces WRITE setDecimalPlaces NOTIFY decimalPlacesChanged)
  Q_PROPERTY(MgrsConversionMode mgrsConversionMode READ mgrsConversionMode WRITE setMgrsConversionMode NOTIFY mgrsConversionModeChanged)
  Q_PROPERTY(LatitudeLongitudeFormat latLonFormat READ latLonFormat WRITE setLatLonFormat NOTIFY latLonFormatChanged)
  Q_PROPERTY(UtmConversionMode utmConversionMode READ utmConversionMode WRITE setUtmConversionMode NOTIFY utmConversionModeChanged)
public:
  /*! Fundamental coordinate format.*/
  enum CoordinateType
  {
    Gars,
    GeoRef,
    LatLon,
    Mgrs,
    Usng,
    Utm
  };
  Q_ENUM(CoordinateType)

public:
  /*!
   * \brief Constructor
   * \param parent Owning parent QObject.
   */
  Q_INVOKABLE CoordinateConversionOption(QObject* parent = nullptr);

  /*!
   * \brief Destructor
   */
  ~CoordinateConversionOption() override;

  /*!
   * \brief Converts a Point to a QString based on the properties set in this
   * CoordinateConversionOption
   * 
   * \param point Point to convert to string.
   * \return String representation of point with the current formatting options
   * applied.
   */
  Q_INVOKABLE QString prettyPrint(const Point& point) const;

  /*!
   * \brief Given a string called point, converts it to a Point using the
   * current formatting options applied.
   * 
   * This conversion has the chance of failing, in which case an invalid
   * default-constructed Point will be returned.
   * 
   * \param point QString to conver to a Point.
   * \param spatialReference SpatialReference to use in conversion.
   * \return QString as a Point. 
   */
  Q_INVOKABLE Point pointFromString(const QString& point,
                                    const SpatialReference& spatialReference);

  /*!
   * \brief The user friendly name of this option.
   */
  QString name() const;

  /*!
   * \brief Set name.
   * \sa name
   */
  void setName(const QString& name);

  /*!
   * \brief The conversion type of this option.
   * \return CoordinateType 
   */
  CoordinateType outputMode() const;

  /*!
   * \brief Set converion type.
   * \sa outputMode
   */
  void setOutputMode(CoordinateType outputMode);

  /*!
   * \brief Flags whether to add spaces in notation (if applicable).
   */
  bool addSpaces() const;

  /*!
   * \brief Set addSpaces.
   * \sa addSpaces
   */
  void setAddSpaces(bool addSpaces);

  /*!
   * \brief Precision of conversion format (if applicable).
   */
  int precision() const;

  /*!
   * \brief Set the precision.
   * \sa precision
   */
  void setPrecision(int precision);

  /*!
   * \brief The number of decimal spaces in the format (if applicable).
   */
  int decimalPlaces() const;

  /*!
   * \brief Set decimalPlaces
   * \sa decimalPlaces
   */
  void setDecimalPlaces(int decimalPlaces);

  /*!
   * \brief The MGRS conversion mode (if applicable).
   */
  MgrsConversionMode mgrsConversionMode() const;

  /*!
   * \brief Set mgrsConversionMode
   * \sa mgrsConversionMode
   */
  void setMgrsConversionMode(MgrsConversionMode mgrsConversionMode);

  /*!
   * \brief The Latitude and Longitude format (if applicable).
   */
  LatitudeLongitudeFormat latLonFormat() const;

  /*!
   * \brief Sets latLonFormat
   * \sa latLonFormat
   */
  void setLatLonFormat(LatitudeLongitudeFormat latLonFormat);

  /*!
   * \brief The UTM conversion mode (if applicable).
   */
  UtmConversionMode utmConversionMode() const;

  /*!
   * \brief Sets utmConversionMode
   * \sa utmConversionMode
   */
  void setUtmConversionMode(UtmConversionMode utmConversionMode);

  /*!
   * \brief The GARS conversion mode (if applicable).
   */
  GarsConversionMode garsConvesrionMode() const;

  /*!
   * \brief Sets garsConversionMode
   * \sa garsConversionMode
   */
  void setGarsConversionMode(GarsConversionMode conversionMode);

signals:
  /*! \brief Emits when name property changes. */
  void nameChanged();

  /*! \brief Emits when outputMode property changes. */
  void outputModeChanged();

  /*! \brief Emits when addSpaces property changes. */
  void addSpacesChanged();

  /*! \brief Emits when precision property changes. */
  void precisionChanged();

  /*! \brief Emits when decimalPlaces property changes. */
  void decimalPlacesChanged();

  /*! \brief Emits when mgrsConversionMode property changes. */
  void mgrsConversionModeChanged();

  /*! \brief Emits when latLonFormat property changes. */
  void latLonFormatChanged();

  /*! \brief Emits when utmConversionMode property changes. */
  void utmConversionModeChanged();

  /*! \brief Emits when garsConversionMode property changes. */
  void garsConversionModeChanged();

private:
  QString m_name;
  CoordinateType m_outputMode;
  bool m_addSpaces;
  int m_precision;
  int m_decimalPlaces;
  MgrsConversionMode m_mgrsConversionMode;
  LatitudeLongitudeFormat m_latLonFormat;
  UtmConversionMode m_utmConversionMode;
  GarsConversionMode m_garsConvesrionMode;
};

} // Toolkit
} // ArcGISRuntime
} // Esri

#endif // ESRI_ARCGISRUNTIME_TOOLKIT_COORDINATECONVERSIONOPTION_H
