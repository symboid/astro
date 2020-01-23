
import QtQuick 2.12
import QtQuick.Controls 2.5

Column {
    property alias selectedItem: searchView.selectedItem

    spacing: 20
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
            anchors.verticalCenter: parent.verticalCenter
            onEditingFinished: parent.search()
        }
        Button {
            id: searchButton
            enabled: geoName.text.length
            text: qsTr("Search")
            onClicked: parent.search()
        }
        function search()
        {
            searchView.geoName = geoName.text
            searchView.update()
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
