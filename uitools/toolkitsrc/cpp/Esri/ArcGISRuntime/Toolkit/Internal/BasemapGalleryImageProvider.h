/*******************************************************************************
 *  Copyright 2012-2021 Esri
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
#ifndef ESRI_ARCGISRUNTIME_TOOLKIT_INTERNAL_BASEMAPGALLERYIMAGEPROVIDER_H
#define ESRI_ARCGISRUNTIME_TOOLKIT_INTERNAL_BASEMAPGALLERYIMAGEPROVIDER_H
#ifdef CPP_ARCGISRUNTIME_TOOLKIT

#include <QQuickImageProvider>
#include <QUuid>

namespace Esri::ArcGISRuntime::Toolkit {

  class BasemapGalleryItem;

  class BasemapGalleryImageProvider : public QQuickAsyncImageProvider
  {
  public:
    static const QString PROVIDER_ID;

    static BasemapGalleryImageProvider* instance();

    ~BasemapGalleryImageProvider() override;

    QQuickImageResponse* requestImageResponse(const QString& id, const QSize& requestedSize) override;

    bool registerItem(BasemapGalleryItem* item);
    bool deregisterItem(BasemapGalleryItem* item);

  private:
    BasemapGalleryImageProvider();
    QObject* m_internalObject;
    QMap<QUuid, BasemapGalleryItem*> m_itemMap;
  };
} // Esri::ArcGISRuntime::Toolkit

#endif // CPP_ARCGISRUNTIME_TOOLKIT
#endif // ESRI_ARCGISRUNTIME_TOOLKIT_INTERNAL_BASEMAPGALLERYIMAGEPROVIDER_H
