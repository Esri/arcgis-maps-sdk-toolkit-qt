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
#include "CoordinateConversionResult.h"

// Qt headers
#include <QClipboard>
#include <QGuiApplication>

// ArcGISRuntime headers
#include "Point.h"

namespace Esri::ArcGISRuntime::Toolkit {

/*!
  \class Esri::ArcGISRuntime::Toolkit::CoordinateConversionResult
  \inmodule EsriArcGISRuntimeToolkit
  \brief A \c CoordinateConversionResult stores the textual representation of a
  a point converted to a string using the formatting given in a
  \c CoordinateConversionOption.
  \sa Esri::ArcGISRuntime::Toolkit::CoordinateConversionOption
 */

/*!
  \brief Constructor
  \list
   \li \a parent Owning parent QObject.
  \endlist
 */
CoordinateConversionResult::CoordinateConversionResult(QObject* parent):
  QObject(parent)
{
}

/*!
  \brief The destructor.
 */
CoordinateConversionResult::~CoordinateConversionResult()
{
}

/*!
  \brief Returns the name of this result. The name is the name as given
  by the associated CoordinateConversionOption.
  \sa type()

  Returns name of result as dictated by the associated type.
 */
QString CoordinateConversionResult::name() const
{
  if (m_type)
    return m_type->name();
  else
    return QString();
}

/*!
  \brief The \c CoordinateConversionOption used to format points passed into
  this object.
  Returns he current associated \c CoordinateConversionOption.
 */
CoordinateConversionOption* CoordinateConversionResult::type() const
{
  return m_type;
}

/*!
  \brief Set the \c CoordinateConversionOption used to format results to
  \a type.
  \note Setting this clears the currently set notation string to a blank
  string.
 */
void CoordinateConversionResult::setType(CoordinateConversionOption* type)
{
  if (type == m_type)
    return;

  if (m_type)
    disconnect(m_type.data(), &CoordinateConversionOption::nameChanged, this, &CoordinateConversionResult::nameChanged);

  m_type = type;
  m_notation = "";

  if (m_type)
    connect(m_type.data(), &CoordinateConversionOption::nameChanged, this, &CoordinateConversionResult::nameChanged);

  emit typeChanged();
  emit nameChanged();
  emit notationChanged();
}

/*!
 \brief The textual representation of a point.
 Returns A string representing a point.
 */
QString CoordinateConversionResult::notation() const
{
  return m_notation;
}

/*!
  \brief Set the notation of this object.
  \note This object performs no checks on setNotation when called explicitly,
  and can contain any string data. E.g. A string containing the message
  \c{"No point set"}.
  \list
  \li \a notation String to set.
  \endlist
 */
void CoordinateConversionResult::setNotation(const QString& notation)
{
  if (m_notation == notation)
    return;

  m_notation = notation;
  emit notationChanged();
}

/*!
  \fn void Esri::ArcGISRuntime::Toolkit::CoordinateConversionResult::updateCoordinatePoint(const Esri::ArcGISRuntime::Point& point)
  \brief Given a \a point, updates the notation of this object to the textual
  representation of the point as dictated by the formatting options given
  in type.
 */
void CoordinateConversionResult::updateCoordinatePoint(const Point& point)
{
  if(!m_type)
    m_notation = "";
  else
    m_notation = m_type->prettyPrint(point);

  emit notationChanged();
}

/*!
 * \brief Takes the current string stored in notation and store it in the
 * clipboard as if the user had copied the text themselves.
 */
void CoordinateConversionResult::copyNotationToClipboard() const
{
  auto clipboard = QGuiApplication::clipboard();
  if (clipboard && !m_notation.isEmpty())
    clipboard->setText(m_notation);
}

/*!
  \fn void Esri::ArcGISRuntime::Toolkit::CoordinateConversionResult::nameChanged()
  \brief Emitted when the type changes or type name changes.
 */

/*!
  \fn void Esri::ArcGISRuntime::Toolkit::CoordinateConversionResult::notationChanged()
  \brief Emitted when the notation string changes.
 */

/*!
  \fn void Esri::ArcGISRuntime::Toolkit::CoordinateConversionResult::typeChanged()
  \brief Emitted when the type changes.
 */

/*!
 \property Esri::ArcGISRuntime::Toolkit::CoordinateConversionResult::name
 */

/*!
 \property Esri::ArcGISRuntime::Toolkit::CoordinateConversionResult::notation
 */

/*!
 \property Esri::ArcGISRuntime::Toolkit::CoordinateConversionResult::type
 */

} // Esri::ArcGISRuntime::Toolkit
