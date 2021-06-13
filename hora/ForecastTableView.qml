
import QtQuick 2.12
import QtQuick.Controls 2.5
import Symboid.Astro.Controls 1.0
import Symboid.Astro.Hora 1.0

HoraTableView {
    columns: [
        Component {
            Label {
                text: cellData
                font.family: "Symboid"
                horizontalAlignment: Label.AlignHCenter
                width: 30
            }
        },
        Component {
            Label {
                text: cellData
                font.family: "Symboid"
                horizontalAlignment: Label.AlignHCenter
                width: 30
            }
        },
        Component {
            Label {
                text: cellData
                font.family: "Symboid"
                horizontalAlignment: Label.AlignHCenter
                width: 30
            }
        }
    ]
}
