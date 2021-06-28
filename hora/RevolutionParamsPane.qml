
import QtQuick 2.15
import QtQuick.Controls 2.15
import Symboid.Sdk.Controls 1.0
import Symboid.Astro.Hora 1.0

MainScreenParamCalcPane {
    title: qsTr("Revolution of %1").arg(revolution.planetName)

    autocalc: true
    buttonVisible: false

    property alias planetIndex: revolution.planetIndex

    property alias planetModel: revolution.planetModel
    property alias planetLont: revolution.planetLont
    property alias hora: revolution.hora
    property alias defaultRevCount: revolution.defaultRevCount
    property alias year: revolution.year
    property alias revCount: revolution.revCount
    property alias tzDiff: revolution.revTzDiff
    property alias revCoords: revolutionList.currentValue

    calcable: Revolution {
        id: revolution
    }

    Pane {
        id: pane
        Row {
            ComboBox {
                id: revolutionList
                anchors.verticalCenter: parent.verticalCenter
                width: metrics.paramSectionWidth - 5*pane.padding - (buttonVisible ? placeHolder.width : 0)
                model: RevolutionListModel {
                    revolutionCalc: revolution
                    onModelReset: {
                        // emitting change of currentValue
                        revolutionList.currentIndex = -1
                        revolutionList.currentIndex = 0
                    }
                }
                textRole: "display"
                valueRole: "datetime"
            }
            // placeholder for "recalc" button
            RoundButton {
                id: placeHolder
                visible: buttonVisible
                anchors.verticalCenter: parent.verticalCenter
                opacity: 0.0
                enabled: false
            }
        }
    }
}
