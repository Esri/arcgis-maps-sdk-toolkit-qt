#include "register.h"

// Toolkit Includes
#include "CoordinateConversion/CoordinateConversionController.h"
#include "CoordinateConversion/CoordinateConversionOption.h"
#include "CoordinateConversion/CoordinateConversionResult.h"
#include "NorthArrowController.h"

// Qt Includes
#include <QQmlEngine>
#include <QQmlFileSelector>

// std includes
#include <type_traits>

namespace Esri
{
namespace ArcGISRuntime
{
namespace Toolkit
{

const char* const Namespace= "Esri.ArcGISRuntime.Toolkit";
const int VersionMajor = 100;
const int VersionMinor = 7;

namespace
{

template <typename T>
void registerComponent()
{
  static_assert(std::is_base_of<QObject, T>::value, "Must inherit QObject");
  auto name = QString("%1CPP").arg(T::staticMetaObject.className());
  name.remove("Esri::ArcGISRuntime::Toolkit::");
  qmlRegisterType<T>(Namespace, VersionMajor, VersionMinor, name.toLatin1());
}

void addFileSelector(QQmlEngine* engine)
{
  auto fileSelector = QQmlFileSelector::get(engine);
  if (!fileSelector)
    fileSelector = new QQmlFileSelector(engine, engine);
  fileSelector->setExtraSelectors({"cpp_api"});
}

}

void registerComponents(QQmlEngine* engine)
{
  addFileSelector(engine);
  registerComponent<NorthArrowController>();
  registerComponent<CoordinateConversionController>();
  registerComponent<CoordinateConversionResult>();
  registerComponent<CoordinateConversionOption>();
}

} // Toolkit
} // ArcGISRuntime
} // Esri
