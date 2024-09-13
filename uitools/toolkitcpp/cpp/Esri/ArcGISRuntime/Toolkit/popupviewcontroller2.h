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
#ifndef ESRI_ARCGISRUNTIME_TOOLKIT_POPUPVIEWCONTROLLER2_H
#define ESRI_ARCGISRUNTIME_TOOLKIT_POPUPVIEWCONTROLLER2_H

// Qt headers
#include <QObject>
#include <QPointer>

// STL headers
#include <Popup.h>

namespace Esri::ArcGISRuntime {

class Popup;

namespace Toolkit
{

class PopupViewController2 : public QObject
{
  Q_OBJECT
  Q_PROPERTY(Popup* popup READ popup WRITE setPopup NOTIFY popupChanged)
  Q_PROPERTY(QString title READ title NOTIFY popupChanged)

public:
  Q_INVOKABLE explicit PopupViewController2(QObject* parent = nullptr);

  Popup* popup() const;
  void setPopup(Popup* popup);

  QString title() const;

signals:
  void popupChanged();

private:
  QPointer<Popup> m_popup;
};

} // Toolkit
} // Esri::ArcGISRuntime
#endif // ESRI_ARCGISRUNTIME_TOOLKIT_POPUPVIEWCONTROLLER2_H
