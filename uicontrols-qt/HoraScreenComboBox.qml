
import QtQuick 2.12
import QtQuick.Controls 2.5

Pane {
    property alias model: comboBox.model
    property alias textRole: comboBox.textRole
    property alias currentIndex: comboBox.currentIndex

    width: parent.defaultItemWidth

    ComboBox {
        id: comboBox
        anchors.left: parent.left
        anchors.right: parent.right
    }
}
