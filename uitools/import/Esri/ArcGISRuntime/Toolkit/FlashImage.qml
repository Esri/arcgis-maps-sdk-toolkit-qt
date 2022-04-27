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
import QtQuick 2.12

/*!
 * \internal
 * \qmltype FlashImage
 * \inqmlmodule Esri.ArcGISRuntime.Toolkit
 * \since Esri.ArcGISRuntime 100.10
 * \brief a FlashImage just exists to display a flashing blue dot on a map
 * for the CoordinateConversion tool, then delete itself after its animation
 * is complete.
 */

Item {
    id: flashImage

    /*!
      \qmlproperty bool running
      \brief Read-only flag stating if the flashing animation is running or not.
     */
    property alias running: animation.running

    /*!
      \brief Color of the flashing image.
     */
    property color color;

    /*!
      \brief This signal is called when the animation completes.
     */
    signal finished();

    opacity: 0

    Rectangle {
      height: 16
      width: height
      radius: height
      color: flashImage.color
      x: -width/2
      y: -height/2
      border {
          color: "transparent"
          width: 1
      }
    }

    SequentialAnimation {
        id: animation
        running: true
        loops: 1
        NumberAnimation {
            target: flashImage
            property: "opacity"
            to: 1.0
            duration: 500
            easing.type: Easing.InOutQuad
        }
        NumberAnimation {
            target: flashImage
            property: "opacity"
            to: 0.0
            duration: 500
            easing.type: Easing.InOutQuad
        }

        onFinished: {
            flashImage.finished()
            flashImage.destroy();
        }
    }
}
