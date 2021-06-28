
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

    function refresh()
    {
        tableModel.update()
    }

}
