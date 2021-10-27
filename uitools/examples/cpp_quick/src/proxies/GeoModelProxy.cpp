#include "GeoModelProxy.h"

#include <GeoModel.h>

GeoModelProxy::GeoModelProxy(QObject* parent) :
  QObject(parent),
  m_geoModel(qobject_cast<Esri::ArcGISRuntime::GeoModel*>(parent))
{
  if (m_geoModel)
  {
    using namespace Esri::ArcGISRuntime;
    connect(m_geoModel, &GeoModel::loadStatusChanged, this, &GeoModelProxy::loadStatusChanged);
  }
}

GeoModelProxy::~GeoModelProxy()
{
}

EnumsProxy::LoadStatus GeoModelProxy::loadStatus() const
{
  if (m_geoModel)
  {
    return static_cast<EnumsProxy::LoadStatus>(m_geoModel->loadStatus());
  }
  return EnumsProxy::LoadStatus::LoadStatusUnknown;
}