import QtQuick 2.3
import ArcGIS.Runtime.Toolkit.Controls 1.0
import QtQuick.Window 2.0
import QtQuick.Controls 1.2
import QtQuick.Dialogs 1.2
import ArcGIS.Runtime 10.25

Rectangle {
    id: searchBoxExample
    clip: true

    property real displayScaleFactor: (Screen.logicalPixelDensity * 25.4) / (Qt.platform.os === "windows" ? 96 : 72)
    property string errorMsg

    width: 500 * displayScaleFactor
    height: 400 * displayScaleFactor

    Map {
        id: mainMap
        anchors.fill: parent
        wrapAroundEnabled: true
        extent: usExtent
        focus: true

        ArcGISTiledMapServiceLayer {
            url: "http://server.arcgisonline.com/ArcGIS/rest/services/World_Street_Map/MapServer"
        }
    }

    ServiceLocator {
        id: locator
        url: "http://geocode.arcgis.com/arcgis/rest/services/World/GeocodeServer"

        onFindStatusChanged: {
            if (findStatus === Locator.FindComplete) {
                progressBar.visible = false;
                if (findResults.length < 1) {
                    showError("No address Found");
                } else {
                    for (var i = 0; i < findResults.length; i++) {
                        var result = findResults[i];
                    }
                    mainMap.zoomTo(result.location);
                }
            } else if (findStatus === Locator.FindError) {
                progressBar.visible = false;
                showError(findError.message + "\nNo Address Found");
            }
        }
    }

    function showError(errorString) {
        errorMsg = errorString;
        messageDialog.visible = true;
    }

    LocatorFindParameters {
        id: findTextParams
        text: searchBox.searchTextInput
        outSR: mainMap.spatialReference
        maxLocations: 1
        searchExtent: usExtent
        sourceCountry: "US"
    }

    Envelope {
        id: usExtent
        xMax: -15000000
        yMax: 2000000
        xMin: -7000000
        yMin: 8000000
    }

    SearchBox {
        id: searchBox
        anchors {
            left:parent.left
            top: parent.top
            margins: 20 * displayScaleFactor
        }

        onSearch: {
            findTextParams.text = searchBox.searchTextInput
            locator.find(findTextParams);
            progressBar.visible = true;
        }

        onClear: {
            mainMap.extent = usExtent;
            mainMap.mapRotation = 0;
            searchBox.searchTextInput = "";
        }

        Keys.onPressed: {
            findTextParams.text = searchBox.searchTextInput
            locator.find(findTextParams);
            progressBar.visible = true;
            Qt.inputMethod.hide();
        }
    }

    Row {
        anchors {
            horizontalCenter: parent.horizontalCenter
            bottom: mainMap.bottom
            bottomMargin: 5 * displayScaleFactor
        }

        ProgressBar {
            id: progressBar
            indeterminate: true
            visible: false
        }
    }

    MessageDialog {
        id: messageDialog
        title: "Error"
        icon: StandardIcon.Warning
        modality: Qt.WindowModal
        standardButtons: StandardButton.Ok
        text: errorMsg
    }
}
