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
#ifndef ESRI_ARCGISRUNTIME_TOOLKIT_BASEMAPGALLERY_H
#define ESRI_ARCGISRUNTIME_TOOLKIT_BASEMAPGALLERY_H

#include <QFrame>

namespace Ui {
  class BasemapGallery;
}

namespace Esri {
namespace ArcGISRuntime {
  class GeoModel;

  namespace Toolkit {
    class BasemapGalleryController;

    class BasemapGallery : public QFrame
    {
      Q_OBJECT
    public:
      Q_INVOKABLE explicit BasemapGallery(QWidget* parent = nullptr);
      Q_INVOKABLE explicit BasemapGallery(GeoModel* geoModel, QWidget* parent = nullptr);

      ~BasemapGallery() override;

      BasemapGalleryController* controller() const;
      void setGeoModel(GeoModel* geomodel);
      GeoModel* geoModel();

    private slots:
      void onItemSelected(const QModelIndex& index);
      void onCurrentBasemapChanged();

    private:
      BasemapGalleryController* m_controller = nullptr;
      Ui::BasemapGallery* m_ui;
    };

  } // Toolkit
} // ArcGISRuntime
} // Esri

#endif // ESRI_ARCGISRUNTIME_TOOLKIT_BASEMAPGALLERY_H
