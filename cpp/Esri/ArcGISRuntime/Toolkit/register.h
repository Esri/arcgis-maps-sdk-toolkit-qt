#ifndef ESRI_ARCGISRUNTIME_TOOLKIT_REGISTER_H
#define ESRI_ARCGISRUNTIME_TOOLKIT_REGISTER_H

class QQmlEngine;

namespace Esri {
namespace ArcGISRuntime {
namespace Toolkit {

extern const char* const Namespace;
extern const int VersionMajor;
extern const int VersionMinor;

void registerComponents(QQmlEngine* engine);

} // Toolkit
} // ArcGISRuntime
} // Esri

#endif // ESRI_ARCGISRUNTIME_TOOLKIT_REGISTER_H