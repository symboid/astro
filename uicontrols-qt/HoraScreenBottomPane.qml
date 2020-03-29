
import QtQuick 2.12
import QtQuick.Controls 2.5
import Symboid.Sdk.Controls 1.0

Pane {
    property Item referenceItem: Item {}
    property Item controlItem: Item {}
    property bool landscape: true

//background: Rectangle { anchors.fill:parent; border.width:3; border.color:"red" }

    readonly property int itemHeight: itemSlot.height + 2*padding
    readonly property int landscapeSpace: mandalaSize - (referenceItem.y + referenceItem.height)

    height:                  !landscape ? itemHeight :
            landscapeSpace < itemHeight ? mandalaSize :
                                          landscapeSpace
    width: itemSlot.width

    ItemSlot {
        id: itemSlot
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        item: controlItem
    }
}
