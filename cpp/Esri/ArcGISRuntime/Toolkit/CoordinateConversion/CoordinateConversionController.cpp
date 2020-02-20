#include "CoordinateConversionController.h"

// ArcGISRuntime headers
#include <CoordinateFormatter.h>

#include "Esri/ArcGISRuntime/Toolkit/GenericListModel.h"
#include "CoordinateConversionOption.h"
#include "CoordinateConversionResult.h"
#include "CoordinateOptionDefaults.h"

namespace Esri
{
namespace ArcGISRuntime
{
namespace Toolkit
{

CoordinateConversionController::CoordinateConversionController(QObject* parent):
  QObject(parent)
{
  {
    auto formatsListModel = new GenericListModel(&CoordinateConversionOption::staticMetaObject, this);
    formatsListModel->setDisplayPropertyName("name");
    formatsListModel->append(QList<QObject*>()
      << createDecimalDegrees()
      << createDegreesDecimalMinutes()
      << createMgrs()
      << createUsgs()
      << createUtm()
      << createGars()
    );
    m_coordinateFormats = formatsListModel;
  }

  {
    auto resultListsModel = new GenericListModel(&CoordinateConversionResult::staticMetaObject, this);
    resultListsModel->setDisplayPropertyName("name");
    m_conversionResults = resultListsModel;
  }
}

CoordinateConversionController::~CoordinateConversionController()
{
}

QObject* CoordinateConversionController::geoView() const
{
  return m_geoView;
}

void CoordinateConversionController::setGeoView(QObject* geoView)
{
  if (geoView == m_geoView)
    return;

  disconnect(this, nullptr, m_geoView, nullptr);

  m_geoView = geoView;
}

QAbstractListModel* CoordinateConversionController::coordinateFormats() const
{
  return m_coordinateFormats;
}

QAbstractListModel* CoordinateConversionController::conversionResults() const
{
  return m_conversionResults;
}

void CoordinateConversionController::zoomTo(Point p)
{
}

//QString CoordinateConversionController::prettyPrint(Point p, Type type)
//{
//  switch(type)
//  {

//  }
//}

} // Toolkit
} // ArcGISRuntime
} // Esri
