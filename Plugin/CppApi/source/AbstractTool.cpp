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

#include "AbstractTool.h"

namespace Esri
{
namespace ArcGISRuntime
{
namespace Toolkit
{
/*!
  \class Esri::ArcGISRuntime::Toolkit::AbstractTool
  \inmodule ArcGISQtToolkit
  \brief Base class for classes that implement individual tools.
  \since Esri::ArcGISRuntime 100.2

  AbstractTool provides functions and properties that all derived tools share.

  \note You cannot create an object of this class. Instead, create a tool
  object from a derived class.
 */

/*!
   \brief The constructor that accepts an optional \a parent object.
 */
AbstractTool::AbstractTool(QObject* parent /*= nullptr*/):
  QObject(parent)
{

}

/*!
   \brief The destructor.
 */
AbstractTool::~AbstractTool()
{

}

/*!
  \brief Reimplement this method in subclasses to handle a click at
  geographic coordinate \a pos.

  Returns \c false.
 */
bool AbstractTool::handleClick(const Point& pos)
{
  Q_UNUSED(pos)
  return false;
}

/*!
  \brief Reimplement this method in subclasses to set tool properties.
  \list
  \li \a properties - A QVariantMap containing property settings.
  \endlist

  A tool property is defined as a key-value pair where the key is a QString
  and the value is a QVariant. Properties are useful for general settings
  which you wish to persist for the tool, such as default modes.
 */
void AbstractTool::setProperties(const QVariantMap&)
{

}

/*!
   \brief Sets whether this tool is active to \a active.

   \sa activeChanged
 */
void AbstractTool::setActive(bool active)
{
  if (m_active == active)
    return;

  m_active = active;

  emit activeChanged();
}

/*!
   \brief Returns whether this tool is active (\c true) or not (\c false).
 */
bool AbstractTool::isActive() const
{
  return m_active;
}

// Signals
/*!
  \fn void Esri::ArcGISRuntime::Toolkit::AbstractTool::errorOccurred(const Error& error)
  \brief Signal emitted when this tool encounters an ArcGISRuntime SDK Qt error.

  \list
    \li \a error - Details about the error that occurred.
  \endlist
 */

/*!
  \fn void Esri::ArcGISRuntime::Toolkit::AbstractTool::propertyChanged(const QString& propertyName, const QVariant& propertyValue)
  \brief Signal emitted when a property of this tool changes.

  \list
    \li \a propertyName - The changed property's name.
    \li \a propertyValue - The changed property's new value.
  \endlist
 */

/*!
  \fn void Esri::ArcGISRuntime::Toolkit::AbstractTool::toolErrorOccurred(const QString& errorMessage)
  \brief Signal emitted when the tool encounters an error.

  \list
    \li \a errorMessage - The message describing the error.
  \endlist
 */

// Properties

/*!
  \fn QString AbstractTool::toolName() const
  \brief Returns the name of this tool.
 */

} // Toolkit
} // ArcGISRuntime
} // Esri
