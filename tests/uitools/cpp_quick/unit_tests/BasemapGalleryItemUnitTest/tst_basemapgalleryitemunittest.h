/*******************************************************************************
 *  Copyright 2012-2022 Esri
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
#ifndef TST_BASEMAPGALLERYITEMUNITTEST_H
#define TST_BASEMAPGALLERYITEMUNITTEST_H

#include <QtTest>

namespace Esri {
namespace ArcGISRuntime {
  class Basemap;
  class Map;
  namespace Toolkit {

  }
}
}

class BasemapGalleryItemUnitTest : public QObject
{
  Q_OBJECT

public:
  BasemapGalleryItemUnitTest();
  ~BasemapGalleryItemUnitTest();

private slots:
  void initTestCase();
  void cleanupTestCase();
  void ctor_Basemap();
  void ctor_Ovverides();
  void ctor_Portalitem();

private:
  Esri::ArcGISRuntime::Basemap* m_basemapLightGray;
  Esri::ArcGISRuntime::Map* m_map;
};

#endif // TST_BASEMAPGALLERYITEMUNITTEST_H
