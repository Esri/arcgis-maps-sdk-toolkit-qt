
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
#include <Point.h>

namespace Esri::ArcGISRuntime::Toolkit {

/*!
  \internal
  This class is an internal implementation detail and is subject to change.
 */

CoordinateConversionResult::CoordinateConversionResult(QObject* parent):
  QObject(parent)
{
}

CoordinateConversionResult::~CoordinateConversionResult()
{
}

QString CoordinateConversionResult::name() const
{
  if (m_type)
    return m_type->name();
  else
    return QString();
}

CoordinateConversionOption* CoordinateConversionResult::type() const
{
  return m_type;
}

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

QString CoordinateConversionResult::notation() const
{
  return m_notation;
}

void CoordinateConversionResult::setNotation(const QString& notation)
{
  if (m_notation == notation)
    return;

  m_notation = notation;
  emit notationChanged();
}

void CoordinateConversionResult::updateCoordinatePoint(const Point& point)
{
  if(!m_type)
    m_notation = "";
  else
    m_notation = m_type->prettyPrint(point);

  emit notationChanged();
}

void CoordinateConversionResult::copyNotationToClipboard() const
{
  auto clipboard = QGuiApplication::clipboard();
  if (clipboard && !m_notation.isEmpty())
    clipboard->setText(m_notation);
}

} // Esri::ArcGISRuntime::Toolkit
