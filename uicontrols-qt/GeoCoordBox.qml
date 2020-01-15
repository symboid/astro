import QtQuick 2.6
import QtQuick.Controls 2.5
import Symboid.Sdk.Controls 1.0
import Symboid.Astro.Controls 1.0

Row {

    property bool isLattitude: false
    property bool editable: false
    property alias arcDegree: coordBox.arcDegree

    ArcCoordBox {
        id: coordBox
        editable: parent.editable
        circular: true
        sectionCalc: isLattitude ? geoLattSectionCalc : geoLontSectionCalc
        sectionValues: isLattitude ? [ qsTr("N"), qsTr("S") ] : [ qsTr("E"), qsTr("W") ]
        GeoLontSectionCalc {
            id: geoLontSectionCalc
        }

        GeoLattSectionCalc {
            id: geoLattSectionCalc
        }
    }
}
