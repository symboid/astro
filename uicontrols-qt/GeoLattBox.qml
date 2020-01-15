
import QtQuick 2.12
import QtQuick.Controls 2.5
import Symboid.Astro.Controls 1.0
import Symboid.Sdk.Controls 1.0

MultiNumberBox {

    property alias sectionCalc: arcCoord.sectionCalc

    ArcCoord {
        id: arcCoord
        sectionCalc: GeoLattSectionCalc {}
        section: box(0).value
        degree: box(1).value
        minute: box(2).value
        second: box(3).value
        Component.onCompleted: {
            box(0).value = Qt.binding(function(){return section})
            box(1).value = Qt.binding(function(){return degree})
            box(2).value = Qt.binding(function(){return minute})
            box(3).value = Qt.binding(function(){return second})
        }
    }
    property alias arcDegree: arcCoord.arcDegree

    boxes: Row {
        EnumBox {
            id: cardinalBox
            from: 0
            to: 1
            valueTexts: [ qsTr("N"), qsTr("S") ]
            circular: true
        }
        NumberBox {
            id: degreeBox
            from: 0
            to: 89
            displaySuffix: "°"
        }
        NumberBox {
            id: minuteBox
            from: 0
            to: 59
            displaySuffix: "'"
            circularLink: degreeBox
        }
        NumberBox {
            id: secondBox
            from: 0
            to: 59
            displaySuffix: "\""
            circularLink: minuteBox
        }
    }
}
