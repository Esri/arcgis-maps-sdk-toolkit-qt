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
#ifndef ESRI_ARCGISRUNTIME_TOOLKIT_BASEMAPGALLERYITEM_H
#define ESRI_ARCGISRUNTIME_TOOLKIT_BASEMAPGALLERYITEM_H

// ArcGISRuntime headers
#include <Basemap.h>

// Qt headers
#include <QImage>
#include <QObject>
#include <QPointer>
#include <QString>
#include <QUrl>

namespace Esri {
namespace ArcGISRuntime {
namespace Toolkit {

  class BasemapGalleryItem : public QObject
  {
    Q_OBJECT
    Q_PROPERTY(Basemap* basemap READ basemap WRITE setBasemap NOTIFY basemapChanged)
    Q_PROPERTY(QString name READ name NOTIFY basemapChanged)
    Q_PROPERTY(QString tooltip READ tooltip NOTIFY tooltipChanged)
    Q_PROPERTY(QString tooltipOverride READ tooltipOverride WRITE setTooltipOverride NOTIFY tooltipOverrideChanged)
    Q_PROPERTY(QImage thumbnail READ thumbnail NOTIFY thumbnailChanged)
    Q_PROPERTY(QImage thumbnailOverride READ thumbnailOverride WRITE setThumbnailOverride NOTIFY thumbnailOverrideChanged)
#ifdef CPP_ARCGISRUNTIME_TOOLKIT
    Q_PROPERTY(QUrl thumbnailUrl READ thumbnailUrl NOTIFY thumbnailChanged)
#endif // CPP_ARCGISRUNTIME_TOOLKIT
  public:
    Q_INVOKABLE BasemapGalleryItem(QObject* parent = nullptr);
    Q_INVOKABLE BasemapGalleryItem(Basemap* basemap, QObject* parent = nullptr);
    Q_INVOKABLE BasemapGalleryItem(Basemap* basemap, QImage thumbnail, QString tooltip, QObject* parent = nullptr);

    ~BasemapGalleryItem() override;

    Basemap* basemap() const;
    void setBasemap(Basemap* basemap);

    QString name() const;

    QImage thumbnail() const;
    QImage thumbnailOverride() const;
    void setThumbnailOverride(QImage thumbnail);

    QString tooltip() const;
    QString tooltipOverride() const;
    void setTooltipOverride(QString tooltip);

#ifdef CPP_ARCGISRUNTIME_TOOLKIT
        QUrl thumbnailUrl() const;
#endif // CPP_ARCGISRUNTIME_TOOLKIT

  QUuid id() const;

  signals:
    void basemapChanged();
    void tooltipChanged();
    void tooltipOverrideChanged();
    void thumbnailChanged();
    void thumbnailOverrideChanged();

  private:
    QPointer<Basemap> m_basemap;
    QImage m_thumbnail;
    QString m_tooltip;
    QUuid m_id;
    Qt::ItemFlags m_flags;
  };

} // Toolkit
} // ArcGISRuntime
} // Esri

#endif // ESRI_ARCGISRUNTIME_TOOLKIT_BASEMAPGALLERYCONTROLLER_H
