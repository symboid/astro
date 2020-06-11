
import QtQuick 2.12
import QtQuick.Controls 2.5

Pane {
    property alias item: textField
    property alias text: textField.text
    property Component button: null
    readonly property int internalWidth: parent.defaultItemWidth - leftPadding - rightMargin

    Grid {
        verticalItemAlignment: Grid.AlignVCenter
        rows: 1
        spacing: 10

        TextField {
            id: textField
            width: internalWidth - (helperButton.active ? helperButton.width + parent.spacing : 0)
        }
        Loader {
            id: helperButton
            sourceComponent: button
        }
    }
}
