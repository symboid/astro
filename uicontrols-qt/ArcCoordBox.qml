import QtQuick 2.0
import QtQuick.Controls 2.5
import Symboid.Astro.Controls 1.0
import Symboid.Sdk.Controls 1.0

MultiNumberBox {
    id: arcCoordBox

    property alias sectionCalc: arcCoord.sectionCalc

    ArcCoord {
        id: arcCoord
        section: numberBox(0).value
        onSectionChanged: {
            numberBox(0).value = Qt.binding(function(){return arcCoord.section})
        }

        degree: numberBox(1).value
        onDegreeChanged: {
            numberBox(1).value = Qt.binding(function(){return arcCoord.degree})
        }

        minute: numberBox(2).value
        onMinuteChanged: {
            numberBox(2).value = Qt.binding(function(){return arcCoord.minute})
        }

        second: numberBox(3).value
        onSecondChanged: {
            numberBox(3).value = Qt.binding(function(){return arcCoord.second})
        }

    }
    property alias arcDegree: arcCoord.arcDegree

    boxes: ListModel {
        ListElement {
            is_enum: true
            number_from: 0
            number_to: 11
        }
        ListElement {
            is_enum: false
            number_from: 0
            number_to: 359
            number_suffix: "°"
        }
        ListElement {
            is_enum: false
            number_from: 0
            number_to: 59
            number_suffix: "'"
        }
        ListElement {
            is_enum: false
            number_from: 0
            number_to: 59
            number_suffix: "\""
        }
    }

    Component.onCompleted: {
        numberBox(0).valueTexts = [ "K", "Ny" ]
        numberBox(0).to   = Qt.binding(function(){return sectionCalc.sectionCount() - 1})
        numberBox(1).to   = Qt.binding(function(){return sectionCalc.sectionMax() - 1})
    }
}
