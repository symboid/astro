
import QtQuick 2.12
import QtQuick.Controls 2.5
import Symboid.Sdk.Controls 1.0
import Symboid.Astro.Hora 1.0

MainScreenParamCalcPane {
    title: qsTr("Revolution of %1").arg(revolution.planetName)

    property alias planetIndex: revolution.planetIndex
    property alias planetModel: revolution.planetModel
    property alias planetLont: revolution.planetLont
    property alias hora: revolution.hora
    property alias defaultRevCount: revolution.defaultRevCount

    calcable: Revolution {
        id: revolution
    }

    Pane {
        id: pane
        Row {
            ComboBox {
                id: revolutionList
                width: metrics.paramSectionWidth - 5*pane.padding - placeHolder.width
                model: revolution.list
            }
            // placeholder for "recalc" button
            RoundButton {
                id: placeHolder
                opacity: 0.0
                enabled: false
            }
        }
    }
}
