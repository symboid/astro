
import QtQuick 2.12
import QtQuick.Controls 2.5
import Symboid.Astro.Controls 1.0
import QtQuick.Controls.Material 2.3

Item {

    property var headerModel: null
    property var tableModel: null

    property var columnWidths: []
    property int tableWidth: parent.width

    property list<Component> columnComponents: [ Component { Item { } } ]

    readonly property int headerHeight: 60
    readonly property int colSpacing: 25

    Pane {
        id: tableHeader
        leftInset: 0
        leftPadding: 0
        anchors {
            top: parent.top
            topMargin: headerHeight / 2
            left: parent.left
            right: parent.right
        }
        height: headerHeight
        Material.background: "#95B2A0"
        ListView {
            id: headerView
            anchors {
                top: parent.top
                bottom: parent.bottom
            }
            width: Math.min(tableWidth, parent.width)

            clip: true
            contentX: tableView.contentX
            model: headerModel
            orientation: Qt.Horizontal
            spacing: colSpacing
            delegate: Label {
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
        clip: true

        model: tableModel
        flickableDirection: Flickable.VerticalFlick | Flickable.HorizontalFlick
        columnSpacing: colSpacing

        delegate: Loader {
            property var cellData: display
            sourceComponent: columnComponents[column + 1]

            onWidthChanged: correctColumnWidth()
            Component.onCompleted: correctColumnWidth()

            function correctColumnWidth(){
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

    function forceLayout()
    {
        tableView.forceLayout()
        headerView.forceLayout()
    }
}
