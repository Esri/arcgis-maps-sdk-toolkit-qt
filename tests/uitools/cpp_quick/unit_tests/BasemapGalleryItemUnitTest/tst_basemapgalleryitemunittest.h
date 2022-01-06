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
