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
#ifndef ESRI_ARCGISRUNTIME_TOOLKIT_BOOKMARKLISTITEM_H
#define ESRI_ARCGISRUNTIME_TOOLKIT_BOOKMARKLISTITEM_H

// Qt headers
#include <QObject>
#include <QPointer>

namespace Esri {
  namespace ArcGISRuntime {
    class Bookmark;

    namespace Toolkit {
      class BookmarkListItem : public QObject
      {
        Q_OBJECT
        Q_PROPERTY(QString name READ name NOTIFY nameChanged)

      public:
        Q_INVOKABLE explicit BookmarkListItem(QObject* parent = nullptr);
        Q_INVOKABLE explicit BookmarkListItem(Bookmark* bookmark, QObject* parent = nullptr);
        ~BookmarkListItem() override;

        void setBookmark(Bookmark* bookmark);
        Bookmark* bookmark() const;

        QString name() const;
        void setName(const QString& name);

      signals:
        void bookmarkChanged();
        void nameChanged();

      private:
        QPointer<Bookmark> m_bookmark;
      };

    } // Toolkit
  } // ArcGISRuntime
} // Esri

#endif // ESRI_ARCGISRUNTIME_TOOLKIT_BOOKMARKLISTITEM_H
