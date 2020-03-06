
import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQml 2.12

Pane {
    id: timerPane
    property string text: ""
    property alias checked: switchButton.checked
    property int periodMsec: 1000
    signal triggered()

    Grid {
        verticalItemAlignment: Grid.AlignVCenter
        rows: 1
        spacing: 10
        Label {
            text: timerPane.text
            horizontalAlignment: Label.AlignRight
        }

        RoundButton {
            id: switchButton
            checkable: true
            onCheckedChanged: timerPane.checked = checked
            icon.source: checked
                       ? "/icons/playback_stop_icon&24.png"
                       : "/icons/playback_rec_icon&24.png"
        }
    }

    Timer {
        triggeredOnStart: true
        repeat: true
        interval: periodMsec
        running: timerPane.checked
        onTriggered: timerPane.triggered()
    }
}
