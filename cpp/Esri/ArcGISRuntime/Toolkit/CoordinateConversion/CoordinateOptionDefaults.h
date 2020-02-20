#ifndef COORDIANTEOPTIONDEFAULTS_H
#define COORDIANTEOPTIONDEFAULTS_H

#include "CoordinateConversionOption.h"

namespace Esri
{
namespace ArcGISRuntime
{
namespace Toolkit
{

CoordinateConversionOption* createDecimalDegrees(QObject* parent = nullptr);

CoordinateConversionOption* createDegreesDecimalMinutes(QObject* parent = nullptr);

CoordinateConversionOption* createDegreesMinutesSeconds(QObject* parent = nullptr);

CoordinateConversionOption* createMgrs(MgrsConversionMode mode = MgrsConversionMode::Automatic, QObject* parent = nullptr);

CoordinateConversionOption* createUsgs(int precision = 7, bool useSpaces = true, QObject* parent = nullptr);

CoordinateConversionOption* createUtm(UtmConversionMode mode = UtmConversionMode::NorthSouthIndicators, bool useSpaces = true, QObject* parent = nullptr);

CoordinateConversionOption* createGars(GarsConversionMode mode = GarsConversionMode::Center, QObject* parent = nullptr);

} // Toolkit
} // ArcGISRuntime
} // Esri

#endif // COORDIANTEOPTIONDEFAULTS_H