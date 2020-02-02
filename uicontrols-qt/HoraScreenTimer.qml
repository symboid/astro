
import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQml 2.12

Row {
    property alias text: switchItem.text
    property alias checked: switchItem.checked
    property int periodMsec: 1000
    signal triggered()
    spacing: 10

    Switch {
        id: switchItem
        anchors.verticalCenter: parent.verticalCenter
    }
    Item {
        height: parent.height
        width: parent.height
    }
    Timer {
        triggeredOnStart: true
        repeat: true
        interval: periodMsec
        running: switchItem.checked
        onTriggered: parent.triggered()
    }
}
