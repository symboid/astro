
import QtQuick 2.15
import QtQuick.Controls 2.15
import Symboid.Sdk.Controls 1.0
import Symboid.Astro.Hora 1.0

MainScreenParamCalcPane {
    title: qsTr("Revolution of %1").arg(revolution.planetName)

//    autocalc: true

    property alias planetIndex: revolution.planetIndex
    onPlanetIndexChanged: revolutionList.currentIndex = -1

    property alias planetModel: revolution.planetModel
    property alias planetLont: revolution.planetLont
    property alias hora: revolution.hora
    property alias defaultRevCount: revolution.defaultRevCount
    property alias year: revolution.year
    property alias revCount: revolution.revCount

    property HoraCoords nullCoords: HoraCoords {
        year: 0; month: 0; day: 0
    }
    property HoraCoords revCoords: revolutionList.currentValue === undefined ? nullCoords : revolutionList.currentValue

    onCalcTaskFinished: revolutionList.currentIndex = 0

    calcable: Revolution {
        id: revolution
    }

    Pane {
        id: pane
        Row {
            ComboBox {
                id: revolutionList
                anchors.verticalCenter: parent.verticalCenter
                width: metrics.paramSectionWidth - 5*pane.padding - placeHolder.width
                model: RevolutionListModel {
                    revolutionCalc: revolution
                }
                textRole: "display"
                valueRole: "datetime"
            }
            // placeholder for "recalc" button
            RoundButton {
                id: placeHolder
                anchors.verticalCenter: parent.verticalCenter
                opacity: 0.0
                enabled: false
            }
        }
    }
}
