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
#ifndef ESRI_ARCGISRUNTIME_TOOLKIT_COORDINATECONVERSIONRESULT_H
#define ESRI_ARCGISRUNTIME_TOOLKIT_COORDINATECONVERSIONRESULT_H

// Toolkit headers
#include "CoordinateConversionOption.h"

// Qt headers
#include <QObject>
#include <QPointer>

// ArcGISRuntime headers
#include <Point.h>

namespace Esri
{
namespace ArcGISRuntime
{
namespace Toolkit
{

/*!
 * \brief a CoordinateConversionResult stores the textual representation of a
 * a point converted to a string using the formatting given in a 
 * CoordinateConversionOption.
 */
class CoordinateConversionResult : public QObject
{
  Q_OBJECT
  Q_PROPERTY(QString name READ name NOTIFY nameChanged)
  Q_PROPERTY(QString notation READ notation WRITE setNotation NOTIFY notationChanged)
  Q_PROPERTY(CoordinateConversionOption* type READ type WRITE setType NOTIFY typeChanged)
public:
  /*!
   * \brief Constructor
   * \param parent Owning parent QObject.
   */
  Q_INVOKABLE CoordinateConversionResult(QObject* parent = nullptr);

  /*!
   * \brief Destructor
   */
  ~CoordinateConversionResult() override;

  /*!
   * \brief Returns the name of this result. The name is the name as given
   * by the associated CoordinateConversionOption.
   * \sa type
   * \return name of result as dictated by the associated type.
   */
  QString name() const;

  /*!
   * \brief The textual representation of a point.
   * \return String representing a point.
   */
  QString notation() const;

  /*!
   * \brief Set the notation of this object.
   * \note This object performs no checks on setNotation when called explicitly,
   * and can contain any string data. E.g. A string containing the message
   * "No point set."
   * \param notation String to set.
   */
  void setNotation(const QString& notation);

  /*!
   * \brief The CoordinateConversionOption used to format points passed into
   * this object.
   * \return The current associated CoordinateConversionOption.
   */
  CoordinateConversionOption* type() const;

  /*!
   * \brief Set the CoordinateConversionOption used to format results.
   * \note Setting this clears the currently set notation string to a blank
   * string.
   */
  void setType(CoordinateConversionOption* type);

signals:
  /*! Emitted when the type changes or type name changes. */
  void nameChanged();

  /*! Emitted when the notation string changes. */
  void notationChanged();

  /*! Emitted when the type changes */
  void typeChanged();

public slots:
  /*!
   * \brief Given a point, updates the notation of this object to the textual
   * representation of the point as dictated by the formatting options given
   * in type.
   * \param point to store in notation field.
   */
  void updateCoordinatePoint(const Esri::ArcGISRuntime::Point& point);

  /*!
   * \brief Takes the current string stored in notation and store it in the
   * clipboard as if the user had copied the text themselves.
   */
  void copyNotationToClipboard() const;

private:
  QString m_name;
  QString m_notation;
  QPointer<CoordinateConversionOption> m_type;
};

} // Toolkit
} // ArcGISRuntime
} // Esri

#endif // ESRI_ARCGISRUNTIME_TOOLKIT_COORDINATECONVERSIONRESULT_H
