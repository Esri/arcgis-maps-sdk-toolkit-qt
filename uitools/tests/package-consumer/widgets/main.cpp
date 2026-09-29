// Qt headers
#include <QApplication>

// STL headers
#include <Esri/ArcGISRuntime/Toolkit/NorthArrow.h>

int main(int argc, char* argv[])
{
  QApplication app(argc, argv);
  Esri::ArcGISRuntime::Toolkit::NorthArrow northArrow;
  return 0;
}
