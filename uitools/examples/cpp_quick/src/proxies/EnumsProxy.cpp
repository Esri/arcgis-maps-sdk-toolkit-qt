#include "EnumsProxy.h"

/*!
    \internal
    \class EnumsProxy
    \brief This class exposes `Enums.LoadStatus` such that it is accessible to QML.
*/

EnumsProxy::EnumsProxy(QObject* parent) :
  QObject(parent)
{
}

EnumsProxy::~EnumsProxy()
{
}
