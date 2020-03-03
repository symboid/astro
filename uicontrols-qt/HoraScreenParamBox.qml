
import QtQuick 2.12
import QtQuick.Controls 2.5

Grid {
    property string title: ""

    width: parent.paramSectionWidth
    padding: parent.paramSectionPadding
    columns: 1
    horizontalItemAlignment: Grid.AlignRight

    readonly property int defaultItemWidth: width - 3*padding

    Label {
        id: titleLabel
        width: parent.width - 2*parent.padding
        text: title + ":"
        font.italic: true
        font.bold: true
    }
}
