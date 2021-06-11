
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
                height: 20
                verticalAlignment: Label.AlignVCenter
                horizontalAlignment: Label.AlignRight
                width: 30
            }
        },
        Component {
            Label {
                text: cellData
                font.family: "Symboid"
                verticalAlignment: Label.AlignVCenter
                horizontalAlignment: Label.AlignHCenter
                width: 30
            }
        },
        Component {
            Label {
                text: cellData
                font.family: "Symboid"
                verticalAlignment: Label.AlignVCenter
                horizontalAlignment: Label.AlignLeft
                width: 30
            }
        }
    ]
}
