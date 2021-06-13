
import QtQuick 2.15
import QtQuick.Controls 2.5
import Symboid.Sdk.Controls 1.0

Item {
    anchors.margins: 20

    property var tableModel: null

    property font horzHeaderFont: Qt.font({bold:true})

    property list<Component> columns: [
        Component {
            Label {
                text: cellData
                font: horzHeaderFont
            }
        }
    ]

    readonly property int horzDataSpace: width - verticalHeader.width - 1 - 2
    readonly property int vertDataSpace: height - horizontalHeader.height - 1 - 2

    Frame {
        anchors.centerIn: parent

    Grid {
        columns: 3

        onWidthChanged: dataView.forceLayout()
        onHeightChanged: dataView.forceLayout()

        Item {
            height: 1; width: 1
        }
        Frame {
            padding: 0
            height: horizontalHeader.height
            width: 1
        }
        TableView {
            id: horizontalHeader
            width: dataView.width
            height: contentHeight
            model: tableModel.horzHeaderModel
            clip: true
            delegate: Pane {
                Label {
                    text: model.display
                    anchors.right: parent.right
                }
            }
            syncView: dataView
            syncDirection: Qt.Horizontal
        }

        Frame {
            padding: 0
            height: 1
            width: verticalHeader.width
        }
        Frame {
            padding: 0
            height: 1; width: 1
        }
        Frame {
            padding: 0
            height: 1
            width: dataView.width
        }

        TableView {
            id: verticalHeader
            width: contentWidth
            height: dataView.height
            model: tableModel.vertHeaderModel
            clip: true
            delegate: Pane {
                Loader {
                    property var cellData: display
                    sourceComponent: columns[0]
                }
            }
            syncView: dataView
            syncDirection: Qt.Vertical
        }
        Frame {
            padding: 0
            height: dataView.height
            width: 1
        }
        TableView {
            id: dataView
            model: tableModel
            flickableDirection: Flickable.AutoFlickIfNeeded
            width: Math.min(contentWidth, horzDataSpace)
            height: Math.min(contentHeight, vertDataSpace)
            clip: true
            boundsBehavior: Flickable.StopAtBounds
            delegate: Pane {
                Loader {
                    anchors.right: parent.right
                    property var cellData: display
                    sourceComponent: columns[column + 1]
                }
            }
        }
    }
    }

    /*
    property var columnWidths: []
    readonly property int lineWidth: 1

    readonly property int tableWidth: {
        var w = 0
        for (var c = 0; c < columnWidths.length; ++c)
        {
            w += columnWidths[c]
        }
        w += colSpacing*(columnWidths.length - 1)
        return w
    }
    readonly property int displayWidth: Math.min(tableWidth + rowHeight, width)
    readonly property int verticalHeaderWidth: columnWidths.length ? columnWidths[0] : 0
    readonly property int horizontalHeaderWidth: displayWidth - verticalHeaderWidth - lineWidth

    property int rowHeight: 50
    readonly property int tableHeight: rowHeight * (verticalHeader.count + 1) + 2 * lineWidth
    readonly property int displayHeight: Math.min(tableHeight, height)
    readonly property int verticalHeaderHeight: displayHeight - horizontalHeader.height - 2 * lineWidth
    readonly property int horizontalHeaderHeight: rowHeight

    readonly property int colSpacing: 25

    Grid {
        columns: 3
        anchors.centerIn: parent

        HoraTableLine { length: verticalHeaderWidth; w: lineWidth }
        HoraTableLine { length: lineWidth; w: lineWidth }
        HoraTableLine { length: displayWidth - verticalHeaderWidth; w: lineWidth }

        Item { height: 1; width: 1 }
        HoraTableLine { vertical: true; length: rowHeight; w: lineWidth }
        ListView {
            id: horizontalHeader
            width: horizontalHeaderWidth
            height: horizontalHeaderHeight

            clip: true
            contentX: tableView.contentX
            contentWidth: tableWidth
            model: horzHeaderModel
            orientation: Qt.Horizontal
            spacing: colSpacing
            delegate: Label {
                width: index !== 0 ? columnWidths[index] : 1
                height: parent.height
                text: modelData
                wrapMode: Label.Wrap
                horizontalAlignment: Label.AlignHCenter
                verticalAlignment: Label.AlignVCenter
            }
        }

        HoraTableLine { length: verticalHeaderWidth; w: lineWidth }
        HoraTableLine { length: lineWidth; w: lineWidth}
        HoraTableLine { length: horizontalHeaderWidth; w: lineWidth }

        ListView {
            id: verticalHeader
            width: verticalHeaderWidth
            height: verticalHeaderHeight

            clip: true
            contentY: tableView.contentY
            model: tableModel
            delegate: Item {
                height: rowHeight
                width: columnWidths[0]

                Loader {
                    anchors.centerIn: parent
                    property var cellData: display
                    sourceComponent: columnComponents[1]

                    onWidthChanged: correctColumnWidth()
                    Component.onCompleted: correctColumnWidth()

                    function correctColumnWidth(){
                        if (index == 0) {
                            columnWidths = []
                        }
                        while (columnWidths.length <= 0)
                        {
                            columnWidths.push(0)
                        }
                        if (columnWidths[0] < width)
                        {
                            columnWidths[0] = width
                            columnWidthsChanged()
                        }
                    }
                }
            }
        }
        HoraTableLine { vertical: true; length: verticalHeaderHeight; w: lineWidth }
        TableView {
            id: tableView
            width: horizontalHeaderWidth
            height: verticalHeaderHeight
            clip: true

            model: tableModel
            flickableDirection: Flickable.VerticalFlick | Flickable.HorizontalFlick
            columnSpacing: colSpacing

            contentWidth: tableWidth

            delegate: Item {
                implicitHeight: rowHeight
                implicitWidth: column !== 0 && columnWidths.length > column ? columnWidths[column] : 1
                Loader {
                    anchors.centerIn: parent
                    property var cellData: display
                    Component {
                        id: nullItem
                        Item { implicitWidth:1;implicitHeight:1 }
                    }

                    sourceComponent: column ? columnComponents[column + 1] : nullItem

                    onWidthChanged: correctColumnWidth()
                    Component.onCompleted: correctColumnWidth()

                    function correctColumnWidth() {
                        while (columnWidths.length <= column)
                        {
                            columnWidths.push(0)
                        }
                        if (column == 0) {
//                            visible = false
                        }
                        else if (columnWidths[column] < item.width)
                        {
                            columnWidths[column] = item.width
                            columnWidthsChanged()
                        }
                    }
                }
            }
        }
    }
    */
}
