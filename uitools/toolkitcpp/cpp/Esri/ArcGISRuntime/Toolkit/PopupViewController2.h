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
#include <PopupElement.h>

// Other headers
#include "Internal/GenericListModel.h"

namespace Esri::ArcGISRuntime {

class Popup;
class PopupElement;

namespace Toolkit
{

class PopupViewController2 : public QObject
{
  Q_OBJECT
  Q_PROPERTY(Popup* popup READ popup WRITE setPopup NOTIFY popupChanged)
  Q_PROPERTY(QPointer<PopupElement> popupElement READ popupElement NOTIFY popupElementChanged)
//  Q_PROPERTY(int popupElementType READ popupElementType NOTIFY popupElementTypeChanged)
  Q_PROPERTY(QString title READ title NOTIFY popupChanged)
  Q_PROPERTY(QAbstractListModel* popupElements READ popupElements NOTIFY popupElementChanged)

public:
  Q_INVOKABLE explicit PopupViewController2(QObject* parent = nullptr);
  ~PopupViewController2() = default;

  Popup* popup() const;
  QPointer<PopupElement> popupElement() const;
  void setPopup(Popup* popup);
  GenericListModel* popupElements() const;

  QString title() const;

signals:
  void popupChanged();
  void popupElementChanged();

private:
  QPointer<Popup> m_popup;
  QPointer<PopupElement> m_popupElement;
  GenericListModel* m_popupElementsModel = nullptr;
};

} // Toolkit
} // Esri::ArcGISRuntime
#endif // ESRI_ARCGISRUNTIME_TOOLKIT_POPUPVIEWCONTROLLER2_H
