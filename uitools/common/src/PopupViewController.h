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
#ifndef ESRI_ARCGISRUNTIME_TOOLKIT_POPUPVIEWCONTROLLER_H
#define ESRI_ARCGISRUNTIME_TOOLKIT_POPUPVIEWCONTROLLER_H

// Qt headers
#include <QAbstractListModel>
#include <QMetaObject>
#include <QObject>
#include <QPointer>

// STL headers
#include <Deprecated.h>
#include <Popup.h>
#include <PopupElement.h>

// Other headers
#include "GenericListModel.h"

namespace Esri::ArcGISRuntime
{

  namespace Toolkit
  {

    class PopupViewController : public QObject
    {
      Q_OBJECT
      Q_PROPERTY(Popup* popup READ popup WRITE setPopup NOTIFY popupChanged)
      Q_PROPERTY(QString title READ title NOTIFY titleChanged)
      Q_PROPERTY(QString editSummary READ editSummary NOTIFY editSummaryChanged)
      Q_PROPERTY(QAbstractListModel* popupElementControllers READ popupElementControllers NOTIFY popupChanged)

    public:
      Q_INVOKABLE explicit PopupViewController(QObject* parent = nullptr);

      ~PopupViewController() override;

      Popup* popup() const;

      void setPopup(Popup* popup);

      GenericListModel* popupElementControllers() const;

      QString title() const;
      QString editSummary() const;

    signals:

      void popupChanged();

      void titleChanged();

      void editSummaryChanged();

      void attachmentDataFetched(const QByteArray& attachmentData, const QString& name);

      void clickedUrl(const QUrl& url);

      void imageClicked(const QUrl& sourceUrl, const QUrl& linkUrl);

    private:
      void refreshPopupContent_();
      void disconnectAttributeModelSignal_();

      QPointer<Popup> m_popup;
      GenericListModel* m_popupElementControllerModel = nullptr;
      QMetaObject::Connection m_attributeModelConnection;
    };

  } // namespace Toolkit
} // namespace Esri::ArcGISRuntime

#endif // ESRI_ARCGISRUNTIME_TOOLKIT_POPUPVIEWCONTROLLER_H
