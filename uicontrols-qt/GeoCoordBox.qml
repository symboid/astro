import QtQuick 2.6
import QtQuick.Controls 2.5
import Symboid.Sdk.Controls 1.0
import Symboid.Astro.Controls 1.0

Row {

    property bool isLattitude: false
    readonly property real signValue: 2 * signBox.value - 1
    readonly property real arcDegree: signValue * coordBox.arcDegree

    function setArcDegree(signedArcDegree) {
        signBox.value = signedArcDegree < 0.0 ? 0 : 1
        coordBox.arcDegree = Math.abs(signedArcDegree)
    }

    property bool editable: false

    EnumBox {
        id: signBox
        from: 0
        to: 1
        valueTexts: isLattitude ? [ "D", "É" ] : [ "Ny", "K" ]
        editable: parent.editable
    }

    ArcCoordBox {
        id: coordBox
        from: 0
        to: isLattitude ? 89 : 179
        editable: parent.editable
        circular: true
    }
}
