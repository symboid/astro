
import QtQuick 2.12
import QtQuick.Controls 2.5
import Symboid.Astro.Controls 1.0
import Symboid.Sdk.Controls 1.0

MultiNumberBox {
    id: arcCoordBox

    property alias sectionCalc: arcCoord.sectionCalc

    ArcCoord {
        id: arcCoord
        section: box(0).value
        degree: box(1).value
        minute: box(2).value
        second: box(3).value

        Component.onCompleted: {
            box(0).value = Qt.binding(function(){return arcCoord.section})
            box(1).value = Qt.binding(function(){return arcCoord.degree})
            box(2).value = Qt.binding(function(){return arcCoord.minute})
            box(3).value = Qt.binding(function(){return arcCoord.second})
        }
    }
    property alias arcDegree: arcCoord.arcDegree

    boxes: Row {
        EnumBox {
            id: sectionBox
            from: 0
            to: sectionCalc.sectionCount() - 1
            valueTexts: sectionCalc.values()
            circular: true
            visible: sectionCalc.sectionCount() > 1
            font: sectionCalc.sectionFont()
        }
        NumberBox {
            id: degreeBox
            from: sectionCalc.sectionMin()
            to: sectionCalc.sectionMax() - 1
            displaySuffix: "°"
        }
        NumberBox {
            id: minuteBox
            from: 0
            to: 59
            digitCount: 2
            displaySuffix: "'"
            circularLink: degreeBox
        }
        NumberBox {
            id: secondBox
            from: 0
            to: 59
            digitCount: 2
            displaySuffix: "\""
            circularLink: minuteBox
        }
    }
}
