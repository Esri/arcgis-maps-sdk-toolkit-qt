/*******************************************************************************
 *  Copyright 2012-2021 Esri
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

import QtQml 2.12

/*!
   \internal
   \qmltype ScalineLineController
   \inqmlmodule Esri.ArcGISRuntime.Toolkit
   \since Esri.ArcGISRuntime 100.13
   \ingroup ArcGISQtToolkitUiQmlControllers
   \brief In MVC architecture, this is the controller for the corresponding
    \c Scalebar view.

    This controller object handles the Scalebar calculations for a Scalebar's width
    and display units, based on a given mapview and owning scalebar's bounds.
    \note This controller has not been implemented yet for QML.
 */
//todo: once implemented, add this file as a \l to the docs on scalebar.qml controller property. remove the \note above
QtObject {

  property var mapView;

  property var unitSystem;

  Component.onCompleted: {
    console.error("Scalebar has not been implemented for QML.")
  }
}
