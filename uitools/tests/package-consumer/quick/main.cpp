// Qt headers
#include <QGuiApplication>
#include <QQmlEngine>
#include <QtWebView>

// STL headers
#include <Esri/ArcGISRuntime/Toolkit/register.h>

int main(int argc, char* argv[])
{
  QtWebView::initialize();
  QGuiApplication app(argc, argv);
  QQmlEngine engine;
  Esri::ArcGISRuntime::Toolkit::registerComponents(engine);
  return 0;
}
