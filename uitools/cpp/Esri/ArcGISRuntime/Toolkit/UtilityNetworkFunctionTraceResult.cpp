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

#include "UtilityNetworkFunctionTraceResult.h"

namespace Esri::ArcGISRuntime::Toolkit {

UtilityNetworkFunctionTraceResult::UtilityNetworkFunctionTraceResult(QObject* parent) :
  QObject(parent)
{
  //
}

UtilityNetworkFunctionTraceResult::UtilityNetworkFunctionTraceResult(const QString& name,
                                                                     UtilityTraceFunctionType type,
                                                                     double value,
                                                                     QObject* parent) :
  QObject(parent),
  m_name(name),
  m_type(type),
  m_value(value)
{
  //
}

UtilityNetworkFunctionTraceResult::~UtilityNetworkFunctionTraceResult() = default;

QString UtilityNetworkFunctionTraceResult::name() const
{
  return m_name;
}

UtilityTraceFunctionType UtilityNetworkFunctionTraceResult::type() const
{
  return m_type;
}

QString UtilityNetworkFunctionTraceResult::typeAsLabel() const
{
  switch(m_type)
  {
    case UtilityTraceFunctionType::Add:
      return "Add";
    case UtilityTraceFunctionType::Average:
      return "Average";
    case UtilityTraceFunctionType::Count:
      return "Count";
    case UtilityTraceFunctionType::Max:
      return "Max";
    case UtilityTraceFunctionType::Min:
      return "Min";
    case UtilityTraceFunctionType::Subtract:
      return "Subtract";
    default:
      return "N/a";
  }
}

double UtilityNetworkFunctionTraceResult::value() const
{
  return m_value;
}

} // Esri::ArcGISRuntime::Toolkit
