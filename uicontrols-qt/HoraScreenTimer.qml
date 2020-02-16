
import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQml 2.12

Pane {
    id: timerPane
    property alias text: switchLabel.text
    property alias checked: switchButton.checked
    property int periodMsec: 1000
    signal triggered()
    Row {
        spacing: 10

        Label {
            id: switchLabel
            anchors.verticalCenter: parent.verticalCenter
        }

        RoundButton {
            id: switchButton
            anchors.verticalCenter: parent.verticalCenter
            checkable: true
            icon.source: checked
                       ? "/icons/playback_stop_icon&24.png"
                       : "/icons/playback_rec_icon&24.png"
        }

        Timer {
            triggeredOnStart: true
            repeat: true
            interval: periodMsec
            running: switchButton.checked
            onTriggered: timerPane.triggered()
        }
    }
}
