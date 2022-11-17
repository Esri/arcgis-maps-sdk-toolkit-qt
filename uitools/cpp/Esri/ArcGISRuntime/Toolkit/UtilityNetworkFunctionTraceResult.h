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

#ifndef UTILITYNETWORKFUNCTIONTRACERESULT_H
#define UTILITYNETWORKFUNCTIONTRACERESULT_H

// Toolkit headers
#include "Internal/GenericListModel.h"

// Qt headers
#include <QObject>

namespace Esri::ArcGISRuntime::Toolkit {

class UtilityNetworkFunctionTraceResult : public QObject
{
  Q_OBJECT

public:
  Q_INVOKABLE explicit UtilityNetworkFunctionTraceResult(QObject* parent = nullptr);
  Q_INVOKABLE UtilityNetworkFunctionTraceResult(const QString& name,
                                                int type,
                                                double value,
                                                QObject* parent = nullptr);
  ~UtilityNetworkFunctionTraceResult() override;

  QString name() const;
  int type() const;
  double value() const;

private:
  QString m_name;
  int m_type;
  double m_value;
};

} // Esri::ArcGISRuntime::Toolkit

#endif // UTILITYNETWORKFUNCTIONTRACERESULT_H
