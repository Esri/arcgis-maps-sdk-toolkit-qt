/*******************************************************************************
 *  Copyright 2012-2024 Esri
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

#ifndef ARCGIS_RUNTIME_TOOLKIT_CPP_QMLENUMS_H
#define ARCGIS_RUNTIME_TOOLKIT_CPP_QMLENUMS_H

// Qt headers
#include <QObject>

// Maps SDK headers
#include <PopupTypes.h>

class QmlEnums : public QObject
{
  Q_OBJECT
public:
  Q_INVOKABLE explicit QmlEnums(QObject* parent = nullptr);
  ~QmlEnums();

  enum class PopupElementType
  {
    PopupElementTypeUnknown = static_cast<int>(Esri::ArcGISRuntime::PopupElementType::Unknown),
    PopupElementTypeTextPopupElement = static_cast<int>(Esri::ArcGISRuntime::PopupElementType::TextPopupElement),
    PopupElementTypeFieldsPopupElement = static_cast<int>(Esri::ArcGISRuntime::PopupElementType::FieldsPopupElement),
    PopupElementTypeMediaPopupElement = static_cast<int>(Esri::ArcGISRuntime::PopupElementType::MediaPopupElement),
    PopupElementTypeAttachmentsPopupElement = static_cast<int>(Esri::ArcGISRuntime::PopupElementType::AttachmentsPopupElement),
    PopupElementTypeExpressionPopupElement = static_cast<int>(Esri::ArcGISRuntime::PopupElementType::ExpressionPopupElement),
    PopupElementTypeRelationshipPopupElement = static_cast<int>(Esri::ArcGISRuntime::PopupElementType::RelationshipPopupElement),
    PopupElementTypeUnsupportedPopupElement = static_cast<int>(Esri::ArcGISRuntime::PopupElementType::UnsupportedPopupElement)
  };
  Q_ENUM(PopupElementType)
};

#endif // ARCGIS_RUNTIME_TOOLKIT_CPP_QMLENUMS_H