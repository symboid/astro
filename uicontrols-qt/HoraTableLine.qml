
import QtQuick 2.12

Rectangle {
    property bool vertical: false
    property int length: 1
    property int w: 1
    color: "black"
    height: vertical ? length : w
    width: vertical ? w : length
}
