// COPYRIGHT 1995-2018 ESRI
// TRADE SECRETS: ESRI PROPRIETARY AND CONFIDENTIAL
// Unpublished material - all rights reserved under the
// Copyright Laws of the United States and applicable international
// laws, treaties, and conventions.
//
// For additional information, contact:
// Environmental Systems Research Institute, Inc.
// Attn: Contracts and Legal Services Department
// 380 New York Street
// Redlands, California, 92373
// USA
//
// email: contracts@esri.com
/// \file AutoDisconnector.h

#ifndef AUTODISCONNECTOR_H
#define AUTODISCONNECTOR_H

// Qt headers
#include <QMetaObject>

class AutoDisconnector
{
public:
  AutoDisconnector() :
    m_conn(){};
  explicit AutoDisconnector(QMetaObject::Connection&& connection):
    m_conn(std::move(connection))
  {
  }

  ~AutoDisconnector()
  {
    QObject::disconnect(m_conn);
  }

  void set(QMetaObject::Connection&& connection)
  {
    m_conn = std::move(connection);
  }

private:
  Q_DISABLE_COPY(AutoDisconnector)
  QMetaObject::Connection m_conn;
};

#endif // AUTODISCONNECTOR_H
