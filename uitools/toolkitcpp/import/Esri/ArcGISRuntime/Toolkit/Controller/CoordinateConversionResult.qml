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
import QtQml

/*!
   \qmltype CoordinateConversionResult
   \inqmlmodule Esri.ArcGISRuntime.Toolkit
   \ingroup ArcGISQtToolkitUiQmlControllers
   \since Esri.ArcGISRutime 100.10
   \brief a CoordinateConversionResult stores the textual representation of a
   a point converted to a string using the formatting given in a
   CoordinateConversionOption.
 */

QtObject {
   /*!
      \brief The the name of this result. The name is the name as given
      by the associated CoordinateConversionOption.
      \sa type
    */
    readonly property alias name: internal.name;

   /*!
      \brief The CoordinateConversionOption used to format points passed into
      this object.
      Returns the current associated CoordinateConversionOption.
    */
    property CoordinateConversionOption type;

   /*!
      \brief The textual representation of a point.
    */
    property string notation: "";

    /*!
       \qmlmethod CoordinateConversionResult::updateCoordinatePoint(Point point)
       \brief Given a point, updates the notation of this object to the textual
       representation of the point as dictated by the formatting options given
       in type.

       \list
       \li \a point to store in notation field.
       \endlist
     */
    function updateCoordinatePoint(point) {
      if (type)
        notation = type.prettyPrint(point);
      else
        notation = "";
    }

    onTypeChanged: {
      notation = "";
      internal.name = type ?  type.name : "";
    }

    /*! \internal */
    property QtObject internal: QtObject {
        id: internal
        property string name: "";
    }

}
