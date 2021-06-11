
import QtQuick 2.12
import QtQuick.Controls 2.5
import Symboid.Astro.Controls 1.0
import Symboid.Astro.Hora 1.0

HoraTableView {
    anchors.margins: 20
    vertHeaderWidth: 100
    columns: [
        Component {
            Label {
                text: cellData
                font.family: "Symboid"
//                width: 100
            }
        },
        Component {
            Label {
                text: cellData
                font.family: "Symboid"
//                width: 100
            }
        },
        Component {
            Label {
                text: cellData
                font.family: "Symboid"
//                width: 100
            }
        }
    ]
}
