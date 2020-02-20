#include "CoordinateConversionResult.h"

namespace Esri
{
namespace ArcGISRuntime
{
namespace Toolkit
{

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

QString CoordinateConversionOption::name() const
{
  return m_name;
}

void CoordinateConversionOption::setName(const QString& name)
{
  m_name = name;
  emit nameChanged();
}

int CoordinateConversionOption::type() const
{
  return m_type;
}

void CoordinateConversionOption::setType(int type)
{
  m_type = type;
  emit typeChanged();
}

QString CoordinateConversionOption::notation() const
{
  return m_notation;
}

void CoordinateConversionOption::setNotation(const QString& notation)
{
  m_notation = notation;
  emit notationChanged();
}

} // Toolkit
} // ArcGISRuntime
} // Esri