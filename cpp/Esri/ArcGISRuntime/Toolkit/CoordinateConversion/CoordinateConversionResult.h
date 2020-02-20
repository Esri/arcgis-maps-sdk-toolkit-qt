/*******************************************************************************
 *  Copyright 2012-2018 Esri
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

#ifndef COORDINATECONVERSIONRESULT_H
#define COORDINATECONVERSIONRESULT_H

#include <QObject>

namespace Esri
{
namespace ArcGISRuntime
{
namespace Toolkit
{

class CoordinateConversionResult : public QObject
{
  Q_OBJECT
  Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
  Q_PROPERTY(QString notation READ notation WRITE setNotation NOTIFY notationChanged)
  Q_PROPERTY(QString type READ type WRITE setType NOTIFY typeChanged)
public:
  Q_INVOKABLE CoordinateConversionResult(QObject* parent = nullptr);
  ~CoordinateConversionResult() override;

  QString name() const;
  void setName(const QString& name);
  Q_SIGNAL void nameChanged();

  QString notation() const;
  void setNotation(const QString& name);
  Q_SIGNAL void notationChanged();

  int type() const;
  void setType(int type);
  Q_SIGNAL void typeChanged();

private:
  QString name;
  QString notation;
  int type = 0;
};

} // Toolkit
} // ArcGISRuntime
} // Esri

#endif // COORDINATECONVERSIONRESULT_H
