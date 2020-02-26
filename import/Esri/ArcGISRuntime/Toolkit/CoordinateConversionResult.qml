import QtQml 2.12

QtObject {
    readonly property alias name: internal.name;

    property CoordinateConversionOption type;

    property string notation: "";

    property QtObject internal: QtObject {
        id: internal
        property string name: "";
    }

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
}
