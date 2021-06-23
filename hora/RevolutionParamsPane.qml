
import QtQuick 2.12
import QtQuick.Controls 2.5
import Symboid.Sdk.Controls 1.0
import Symboid.Astro.Hora 1.0

MainScreenParamCalcPane {
    title: qsTr("Revolution of %1").arg(revolution.planetName)

//    autocalc: true

    property alias planetIndex: revolution.planetIndex
    property alias planetModel: revolution.planetModel
    property alias planetLont: revolution.planetLont
    property alias hora: revolution.hora
    property alias defaultRevCount: revolution.defaultRevCount
    property alias year: revolution.year
    property alias revCount: revolution.revCount

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
                model: revolution.list
                textRole: "display"
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
