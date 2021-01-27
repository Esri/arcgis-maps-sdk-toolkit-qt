/*******************************************************************************
 *  Copyright 2012-2020 Esri
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
#ifndef ESRI_ARCGISRUNTIME_TOOLKIT_COORDIANTEOPTIONDEFAULTS_H
#define ESRI_ARCGISRUNTIME_TOOLKIT_COORDIANTEOPTIONDEFAULTS_H

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

CoordinateConversionOption* createUsng(int precision = 7, bool useSpaces = true, QObject* parent = nullptr);

CoordinateConversionOption* createUtm(UtmConversionMode mode = UtmConversionMode::NorthSouthIndicators, bool useSpaces = true, QObject* parent = nullptr);

CoordinateConversionOption* createGars(GarsConversionMode mode = GarsConversionMode::Center, QObject* parent = nullptr);

} // Toolkit
} // ArcGISRuntime
} // Esri

#endif // ESRI_ARCGISRUNTIME_TOOLKIT_COORDIANTEOPTIONDEFAULTS_H
