
import QtQuick 2.12
import QtQuick.Controls 2.5

Column {
    padding: 20
    spacing: padding
    Row {
        id: searchRow
        width: parent.width
        spacing: parent.spacing
        TextField {
            id: geoName
            width: parent.width - searchButton.width - parent.spacing
            verticalAlignment: TextField.AlignVCenter
        }
        Button {
            id: searchButton
            text: qsTr("Search")
            onClicked: searchView.geoName = geoName.text
        }
    }
    Rectangle {
        width: parent.width
        height: parent.height - searchRow.height - parent.spacing
        border.width: 1
        GeoNamesSearchView {
            id: searchView
            anchors.fill: parent
            anchors.margins: 1
        }
    }
}
