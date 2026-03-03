/*******************************************************************************
 *  Copyright 2012-2022 Esri
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

#include "CoordinateConversionDemo.h"

#include "CoordinateConversionOption.h"
#include "CoordinateConversionResult.h"

CoordinateConversionDemo::CoordinateConversionDemo(QObject* parent) :
  BaseDemo(parent)
{
}

void CoordinateConversionDemo::setConversionType(Esri::ArcGISRuntime::Toolkit::CoordinateConversionResult* result,
                                                 Esri::ArcGISRuntime::Toolkit::CoordinateConversionOption* type)
{
  if (!result || !type)
  {
    return;
  }
  qDebug() << "set type called = " << type->name();

  result->setType(type);
}

CoordinateConversionDemo::~CoordinateConversionDemo()
{
}
