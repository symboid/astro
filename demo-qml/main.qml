
import QtQuick 2.6
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.12

ApplicationWindow {

    id: appWindow
    visible: true
    color: "#E8E8FF"

    width: 1200
    height: 800

    Rectangle {
        id: menuFrame
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.margins: 20
        color: appWindow.color
        border.width: 1
        border.color: "lightgrey"
        width: 250
        ListView {
            id: menu
            anchors.fill: parent
            anchors.margins: parent.border.width

            model: [ "Coordinate Items Demo", "GeoNames Demo", "HoraView Demo" ]
            delegate: Rectangle {
                width: menu.width
                height: 40
                border.width: 0
                color: index === menu.currentIndex ? menuFrame.border.color : appWindow.color
                Text {
                    anchors.leftMargin: 10
                    verticalAlignment: Text.AlignVCenter
                    anchors.fill: parent
                    text: modelData
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: menu.currentIndex = index
                }
            }
        }
    }

    Rectangle {
        anchors {
            top: parent.top
            bottom: parent.bottom
            left: menuFrame.right
            right: parent.right
            margins: menuFrame.anchors.margins
        }
        border.color: menuFrame.border.color
        border.width: menuFrame.border.width
        height: parent.height
        StackLayout {
            anchors.fill: parent
            currentIndex: menu.currentIndex
            CoordsDemoPage {}
            GeoNamesDemoPage {}
            HoraViewDemoPage {}
        }
    }
}
