import QtQuick 2.12

QtObject {
  property double heading: NaN;
  property var geoView;

  Binding {
      when: geoView !== null && ("mapRotation" in geoView)
      target: this
      property: "heading"
      value: geoView.mapRotation
  }

  Binding {
      when: geoView !== null && ("viewPointCamera" in geoView)
      target: this
      property: "heading"
      value: geoView.currentViewpointCamera.heading
  }
}