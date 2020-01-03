import QtQuick 2.0
import QtQuick.Controls 2.5
import Symboid.Astro.Controls 1.0
import Symboid.Sdk.Controls 1.0

MultiNumberBox {
    id: arcCoordBox

    ArcCoord {
        id: arcCoord
        degree: numberBox(0).value
        minute: numberBox(1).value
        second: numberBox(2).value
    }
    property alias arcDegree: arcCoord.arcDegree

    property int from: 0
    property int to: 359

    boxes: ListModel {
        ListElement {
            number_from: 0
            number_to: 359
            number_suffix: "°"
        }
        ListElement {
            number_from: 0
            number_to: 59
            number_suffix: "'"
        }
        ListElement {
            number_from: 0
            number_to: 59
            number_suffix: "\""
        }
    }

    Component.onCompleted: {
        numberBox(0).from =  Qt.binding(function(){return arcCoordBox.from})
        numberBox(0).to =    Qt.binding(function(){return arcCoordBox.to})
        numberBox(0).value = Qt.binding(function(){return arcCoord.degree})
        numberBox(1).value = Qt.binding(function(){return arcCoord.minute})
        numberBox(2).value = Qt.binding(function(){return arcCoord.second})
    }
}
