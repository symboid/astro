
import QtQuick 2.12
import QtQuick.Controls 2.5
import Symboid.Astro.Controls 1.0
import QtQuick.Controls.Material 2.3

Item {

    property var headerModel: null
    property var tableModel: null

    property var columnWidths: []
    property int tableWidth: 500

    property list<Component> columnComponents: [ Component { Item { } } ]

    Rectangle {
        id: tableHeader
        anchors {
            top: parent.top
            horizontalCenter: parent.horizontalCenter
        }
        height: 40
        width: Math.min(tableWidth, parent.width)
        color: "#95B2A0"
        ListView {
            anchors.fill: parent
            clip: true
            contentX: tableView.contentX
            model: headerModel
            orientation: Qt.Horizontal
            spacing: tableView.columnSpacing
            delegate: Pane {
                height: 40
                width: columnWidths[index]
                Material.background: "#95B2A0"
                Label {
                    text: modelData
                    wrapMode: Label.Wrap
                }
                Component.onCompleted: {
                    columnWidths.push(0)
                }
            }
        }
    }

    TableView {
        id: tableView
        anchors {
            top: tableHeader.bottom
            horizontalCenter: parent.horizontalCenter
            bottom: parent.bottom
        }
        width: Math.min(tableWidth, parent.width)
        rowHeightProvider: function (row) { return 40 }
        clip: true

        model: tableModel
        flickableDirection: Flickable.VerticalFlick | Flickable.HorizontalFlick
        columnSpacing: 30

        delegate: Loader {
            property var cellData: display
            sourceComponent: columnComponents[column + 1]

            Component.onCompleted: {
                if (columnWidths[column] < width)
                {
                    columnWidths[column] = width
                }
            }
        }
    }
}
