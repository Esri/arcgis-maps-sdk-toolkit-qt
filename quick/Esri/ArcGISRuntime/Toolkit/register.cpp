#include "register.h"

// Toolkit Includes
#include <Esri/ArcGISRuntime/Toolkit/NorthArrowController.h>

// Qt Includes
#include <QQmlEngine>
#include <QQmlFileSelector>

// std includes
#include <type_traits>

namespace Esri {
namespace ArcGISRuntime {
namespace Toolkit {

const char* Namespace= "Esri.ArcGISRuntime.Toolkit";
int VersionMajor = 100;
int VersionMinor = 7;

namespace {

template <typename T>
void registerComponent()
{
  static_assert(std::is_base_of<QObject, T>::value, "Must inherit QObject");
  auto name = QString("%1CPP").arg(T::staticMetaObject.className());
  name.remove("Esri::ArcGISRuntime::Toolkit::");
  qmlRegisterType<NorthArrowController>(Namespace, VersionMajor, VersionMinor, name.toLatin1());
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
}

} // Toolkit
} // ArcGISRuntime
} // Esri
