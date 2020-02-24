import QtQml 2.12

QtObject {
    id: controller
    readonly property alias heading: internal.heading;
    property var geoView;

    // Keep the heading read only for external consumers.
    property QtObject internal: QtObject {
        id: internal
        property double heading: NaN;

        property Binding mapBinding: Binding {
            when: geoView !== null && geoView.mapRotation !== undefined
            target: internal
            property: "heading"
            value: geoView.mapRotation
        }

        property Binding sceneBinding: Binding {
            when: geoView !== null && geoView.currentViewpointCamera !== undefined
            target: internal
            property: "heading"
            value: geoView.currentViewpointCamera ? geoView.currentViewpointCamera.heading : NaN
        }

        property Binding nullBinding: Binding {
            when: geoView === null
            target: internal
            property: "heading"
            value: NaN
        }
    }

    function setHeading(newHeading) {
        if (geoView === null)
            return;

        if (geoView.mapRotation !== undefined) {
            // This is a MapView.
            geoView.setViewpointRotation(newHeading);
        }
        else if (geoView.currentViewpointCamera !== undefined) {
            // This is a SceneView.
            let camera = geoView.currentViewpointCamera;
            let newCamera = camera.rotateTo(newHeading, camera.pitch, camera.roll);
            geoView.setViewpointCamera(newCamera);
        }
    }
}
