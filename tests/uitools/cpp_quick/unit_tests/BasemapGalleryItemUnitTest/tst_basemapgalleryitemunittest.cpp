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
#include "tst_basemapgalleryitemunittest.h"

#include "ArcGISRuntimeEnvironment.h"
#include "AutoDisconnector.h"
#include "Basemap.h"
#include "BasemapGalleryItem.h"
#include "Map.h"

using namespace Esri::ArcGISRuntime;
using namespace Esri::ArcGISRuntime::Toolkit;

BasemapGalleryItemUnitTest::BasemapGalleryItemUnitTest()
{

}

BasemapGalleryItemUnitTest::~BasemapGalleryItemUnitTest()
{

}

void BasemapGalleryItemUnitTest::initTestCase()
{
  //setting API key
  const QString apiKey = qgetenv("ARCGIS_RUNTIME_API_KEY");
  Esri::ArcGISRuntime::ArcGISRuntimeEnvironment::setApiKey(apiKey);

  m_basemapLightGray = Basemap::lightGrayCanvas(this);
  m_map = new Map(m_basemapLightGray);

  QSignalSpy mapLoaded(m_map, &Esri::ArcGISRuntime::GeoModel::doneLoading);
  m_map->load();
  AutoDisconnector ad1(connect(m_map, &Esri::ArcGISRuntime::Map::doneLoading, this, [](Esri::ArcGISRuntime::Error loadError)
                               {
                                 QVERIFY2(loadError.isEmpty(), qPrintable(loadError.message()));
                               }));

  QVERIFY2(mapLoaded.wait(), "problem in loading the map.");
}

void BasemapGalleryItemUnitTest::cleanupTestCase()
{
  delete m_map;
  delete m_basemapLightGray;
}

void BasemapGalleryItemUnitTest::ctor_Basemap()
{
  BasemapGalleryItem item(m_basemapLightGray, this);
  QCOMPARE(item.basemap(), m_basemapLightGray);
  QCOMPARE(item.name(), "Light Gray Canvas");
  QCOMPARE(item.tooltip(), "");
  //default thumbnail image
  QVERIFY(!item.thumbnail().isNull());
  QImage defaultThumbnail(QStringLiteral(":/esri.com/imports/Esri/ArcGISRuntime/Toolkit/images/generic_basemap_thumbnail.png"));
  QCOMPARE(item.thumbnail(), defaultThumbnail);
}

void BasemapGalleryItemUnitTest::ctor_Overrides()
{
  QImage thumbnail(":/esri.com/imports/Esri/ArcGISRuntime/Toolkit/images/generic_basemap_thumbnail.png");
  QString tooltip("Basemap description");
  QVERIFY(!thumbnail.isNull());
  BasemapGalleryItem item(m_basemapLightGray, thumbnail, tooltip, this);
  QCOMPARE(item.basemap(), m_basemapLightGray);
  QCOMPARE(item.tooltip(), tooltip);
  QCOMPARE(item.thumbnail(), thumbnail);
}

void BasemapGalleryItemUnitTest::ctor_Portalitem()
{
  QString name("OpenStreetMap Blueprint");
  //not using this tooltip at the moment
  //QString tooltip(QStringLiteral("<div><div style='margin-bottom:3rem;'><div><div style='max-width:100%; display:inherit;'><p style='margin-top:0px; margin-bottom:1.5rem;'><span style='font-family:&quot;Avenir Next W01&quot;, &quot;Avenir Next W00&quot;, &quot;Avenir Next&quot;, Avenir, &quot;Helvetica Neue&quot;, sans-serif; font-size:16px;'>This web map presents a vector basemap of OpenStreetMap (OSM) data hosted by Esri. Esri created this vector tile basemap from the </span><a href='https://daylightmap.org/' rel='nofollow ugc' style='color:rgb(0, 121, 193); text-decoration-line:none; font-family:&quot;Avenir Next W01&quot;, &quot;Avenir Next W00&quot;, &quot;Avenir Next&quot;, Avenir, &quot;Helvetica Neue&quot;, sans-serif; font-size:16px;'>Daylight map distribution</a><span style='font-family:&quot;Avenir Next W01&quot;, &quot;Avenir Next W00&quot;, &quot;Avenir Next&quot;, Avenir, &quot;Helvetica Neue&quot;, sans-serif; font-size:16px;'> of OSM data, which is supported by </span><b><font style='font-family:inherit;'><span style='font-family:inherit;'>Facebook</span></font> </b><span style='font-family:&quot;Avenir Next W01&quot;, &quot;Avenir Next W00&quot;, &quot;Avenir Next&quot;, Avenir, &quot;Helvetica Neue&quot;, sans-serif; font-size:16px;'>and supplemented with additional data from </span><font style='font-family:&quot;Avenir Next W01&quot;, &quot;Avenir Next W00&quot;, &quot;Avenir Next&quot;, Avenir, &quot;Helvetica Neue&quot;, sans-serif; font-size:16px;'><b>Microsoft</b>. It presents the map in a cartographic style is like a blueprint technical drawing. The OSM Daylight map will be updated every month with the latest version of OSM Daylight data. </font></p><div style='font-family:&quot;Avenir Next W01&quot;, &quot;Avenir Next W00&quot;, &quot;Avenir Next&quot;, Avenir, &quot;Helvetica Neue&quot;, sans-serif; font-size:16px;'>OpenStreetMap is an open collaborative project to create a free editable map of the world. Volunteers gather location data using GPS, local knowledge, and other free sources of information and upload it. The resulting free map can be viewed and downloaded from the OpenStreetMap site: <a href='https://www.openstreetmap.org/' rel='nofollow ugc' style='color:rgb(0, 121, 193); text-decoration-line:none; font-family:inherit;' target='_blank'>www.OpenStreetMap.org</a>. Esri is a supporter of the OSM project and is excited to make this enhanced vector basemap available to the ArcGIS user and developer communities.</div></div></div></div></div><div style='margin-bottom:3rem; display:inherit; font-family:&quot;Avenir Next W01&quot;, &quot;Avenir Next W00&quot;, &quot;Avenir Next&quot;, Avenir, &quot;Helvetica Neue&quot;, sans-serif; font-size:16px;'><div style='display:inherit;'></div></div>"));
  PortalItem portalItem(QUrl("https://runtime.maps.arcgis.com/home/item.html?id=46a87c20f09e4fc48fa3c38081e0cae6"));
  Basemap b(&portalItem, this);
  QSignalSpy basemapLoaded(&b, &Basemap::loadStatusChanged);
  // basemap loaded by the item constructor-call stack.
  BasemapGalleryItem item(&b, this);
  QVERIFY(basemapLoaded.wait() || basemapLoaded.count() > 1);
  QCOMPARE(item.basemap(), &b);
  QCOMPARE(item.name(), name);
  QVERIFY(!item.tooltip().isNull());
}

QTEST_MAIN(BasemapGalleryItemUnitTest)
