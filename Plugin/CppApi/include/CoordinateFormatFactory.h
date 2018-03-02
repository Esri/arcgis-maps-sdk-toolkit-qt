// Copyright 2017 ESRI
//
// All rights reserved under the copyright laws of the United States
// and applicable international laws, treaties, and conventions.
//
// You may freely redistribute and use this sample code, with or
// without modification, provided you include the original copyright
// notice and use restrictions.
//
// See the Sample code usage restrictions document for further information.
//

#ifndef COORDINATEFORMATFACTORY_H
#define COORDINATEFORMATFACTORY_H

class QObject;

namespace Esri
{
namespace ArcGISRuntime
{
namespace Toolkit
{

class CoordinateConversionOptions;

class TOOLKIT_EXPORT CoordinateFormatFactory
{
public:
  static CoordinateConversionOptions* createFormat(const QString& formatName, QObject* parent);
};

} // Toolkit
} // ArcGISRuntime
} // Esri

#endif // COORDINATEFORMATFACTORY_H
