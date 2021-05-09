
import QtQuick 2.12
import QtQuick.Controls 2.5
import Symboid.Sdk.Controls 1.0

IndirectContainer {

    property bool calculating: false
    property real progressPos: 0.0
    property alias autocalc: autoCalcButton.checked
    property bool contentValid: false

    signal startCalc()
    signal abortCalc()

    container: resultItem
    Item {
        id: resultItem
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: calcProgress.top
    }
    reparentFrom: 3

    ProgressBar {
        id: calcProgress
        visible: calculating
        anchors.bottom: buttonPane.top
        anchors.left: parent.left
        anchors.right: parent.right
        value: progressPos
    }
    Pane {
        id: buttonPane
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        contentItem: Row {
            spacing: parent.padding
            RoundButton {
                radius: 5
                icon.source: "/icons/refresh_icon&24.png"
                enabled: !autocalc || !contentValid
                visible: !calculating
                highlighted: !contentValid
                onClicked: startCalc()
            }
            RoundButton {
                radius: 5
                icon.source: "/icons/delete_icon&24.png"
                visible: calculating
                onClicked: abortCalc()
            }
            RoundButton {
                id: autoCalcButton
                checkable: true
                radius: 5
                display: RoundButton.IconOnly
                icon.source: checked ? "/icons/connect_icon&24.png" : "/icons/not_connected_icon&24.png"
            }
        }
    }
}
