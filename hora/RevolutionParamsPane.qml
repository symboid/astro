
import QtQuick 2.15
import QtQuick.Controls 2.15
import Symboid.Sdk.Controls 1.0
import Symboid.Astro.Hora 1.0

MainScreenParamBox {
    title: qsTr("Revolution of %1").arg(revolution.planetName)

    property alias planetIndex: revolution.planetIndex

    property alias planetModel: revolution.planetModel
    property alias planetLont: revolution.planetLont
    property alias hora: revolution.hora
    property alias defaultRevCount: revolution.defaultRevCount
    property alias year: revolution.year
    property alias revCount: revolution.revCount
    property alias tzDiff: revolution.revTzDiff
    property alias revCoords: revolutionList.currentValue

    CalcPane {
        id: calcPane

        autocalc: true
        showButton: false

        calcable: Revolution {
            id: revolution
        }

        width: metrics.paramSectionWidth - 3*pane.padding
        height: pane.height

        Pane {
            id: pane
            background: null
            rightPadding: leftPadding + (calcPane.buttonVisible ? padding + calcPane.buttonSize : 0)
            height: calcPane.buttonSize + topPadding + bottomPadding
            ComboBox {
                id: revolutionList
                anchors.verticalCenter: parent.verticalCenter
                width: metrics.paramSectionWidth - 3*pane.padding - (pane.leftPadding + pane.rightPadding)
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
        }
    }
}
