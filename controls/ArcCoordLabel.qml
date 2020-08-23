
import QtQuick 2.12
import QtQuick.Controls 2.5
import Symboid.Astro.Controls 1.0

Row {

    property alias arcDegree: arcCoord.arcDegree
    property alias sectionCalc: arcCoord.sectionCalc
    property alias sectionFont: sectionLabel.font
    property bool showSecond: true

    ArcCoord {
        id: arcCoord
    }

    TextMetrics {
        id: metrics
        text: "0000"
    }

    function twoDigit(number)
    {
        return "" + Math.floor(number / 10) + (number % 10)
    }

    Label {
        id: sectionLabel
        property var valueTexts: sectionCalc.values()
        text: valueTexts[arcCoord.section]
        horizontalAlignment: Label.AlignRight
        anchors.verticalCenter: parent.verticalCenter
    }
    Label {
        id: degreeLabel
        text: arcCoord.degree + "°"
        width: metrics.width
        horizontalAlignment: Label.AlignRight
        anchors.verticalCenter: parent.verticalCenter
    }
    Label {
        id: minuteLabel
        text: twoDigit(arcCoord.minute) + "'"
        width: metrics.width
        horizontalAlignment: Label.AlignRight
        anchors.verticalCenter: parent.verticalCenter
    }
    Label {
        id: secondLabel
        text: twoDigit(arcCoord.second) + "''"
        width: metrics.width
        horizontalAlignment: Label.AlignRight
        anchors.verticalCenter: parent.verticalCenter
        visible: showSecond
    }
}
