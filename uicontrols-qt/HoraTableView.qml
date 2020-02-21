
import QtQuick 2.12
import QtQuick.Controls 2.5
import Symboid.Astro.Controls 1.0
import QtQuick.Controls.Material 2.3

Item {

    property var headerModel: null
    property var tableModel: null

    property var columnWidths: []
    property int tableWidth: parent.width//tableView.contentWidth

    property list<Component> columnComponents: [ Component { Item { } } ]

    readonly property int rowHeight: 40
    readonly property int headerHeight: rowHeight
    readonly property int colSpacing: 30

    Rectangle {
        id: tableHeader
        anchors {
            top: parent.top
            topMargin: rowHeight / 2
            left: parent.left
            right: parent.right
        }
        height: headerHeight
        width: parent.width
        color: "#95B2A0"
        ListView {
            anchors {
                top: parent.top
                bottom: parent.bottom
                horizontalCenter: parent.horizontalCenter
            }
            width: Math.min(tableWidth, parent.width)

            clip: true
            contentX: tableView.contentX
            model: headerModel
            orientation: Qt.Horizontal
            spacing: colSpacing
            delegate: Label {
                height: headerHeight
                width: columnWidths[index]
                text: modelData
                wrapMode: Label.Wrap
                horizontalAlignment: Label.AlignHCenter
                verticalAlignment: Label.AlignVCenter
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
        rowHeightProvider: function (row) { return rowHeight }
        clip: true

        model: tableModel
        flickableDirection: Flickable.VerticalFlick | Flickable.HorizontalFlick
        columnSpacing: colSpacing

        delegate: Loader {
            property var cellData: display
            sourceComponent: columnComponents[column + 1]

            Component.onCompleted: {
                while (columnWidths.length <= column)
                {
                    columnWidths.push(0)
                }
                if (columnWidths[column] < width)
                {
                    columnWidths[column] = width
                    columnWidthsChanged()
                }
            }
        }

    }
}
