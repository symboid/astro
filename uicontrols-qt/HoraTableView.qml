
import QtQuick 2.12
import QtQuick.Controls 2.5
import Symboid.Astro.Controls 1.0
import QtQuick.Controls.Material 2.3

Item {

    property var headerModel: null
    property var tableModel: null

    property var columnWidths: []

    property list<Component> columnComponents: [ Component { Item { } } ]

    ListView {
        id: tableHeader
        clip: true
        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
        }
        contentX: tableView.contentX
        Material.background: "#95B2A0"
        height: 40
        model: headerModel
        orientation: Qt.Horizontal
        spacing: tableView.columnSpacing
        delegate: Pane {
            height: 40
            width: columnWidths[index]
            Label {
                text: modelData
                wrapMode: Label.Wrap
            }
            Component.onCompleted: {
                columnWidths.push(0)
            }
        }
    }

    TableView {
        id: tableView
        anchors {
            top: tableHeader.bottom
            left: parent.left
            right: parent.right
            bottom: parent.bottom
        }

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
                    columnWidthsChanged()
                }
            }
        }
    }
}
