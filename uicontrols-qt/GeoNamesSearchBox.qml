
import QtQuick 2.12
import QtQuick.Controls 2.5

Column {
    padding: 20
    spacing: padding
    Row {
        id: searchRow
        anchors {
            left: parent.left
            right: parent.right
        }

        spacing: parent.spacing
        TextField {
            id: geoName
            width: parent.width - searchButton.width - parent.spacing
            verticalAlignment: TextField.AlignVCenter
        }
        Button {
            id: searchButton
            text: qsTr("Search")
            onClicked: {
                searchView.geoName = geoName.text
                searchView.update()
            }
        }
    }
    Rectangle {
        anchors {
            left: parent.left
            right: parent.right
        }
        height: parent.height - searchRow.height - parent.spacing - 2 * parent.padding
        GeoNamesSearchView {
            id: searchView
            anchors.fill: parent
        }
    }
}
