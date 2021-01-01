
import QtQuick 2.12
import QtQuick.Controls 2.5
import Symboid.Sdk.Controls 1.0
import Symboid.Sdk.Network 1.0
import Symboid.Astro.Db 1.0

LoadListView {
    id: geoListView
    property string queryOperation: ""
    signal loadButtonClicked(string geoName, real geoLat, real geoLng)
    function runQuery()
    {
        restTableModel.runOperation()
    }

    model: RestTableModel {
        id: restTableModel
        restClient: GeoNamesRestClient
        operation: queryOperation+
                   "&lang="+Qt.locale().name.substring(0,2)+
                   "&maxRows=10"+
                   "&username=symboid"
        columnNames: ["name", "countryName", "adminName1", "population", "lng", "lat"]

        onModelAboutToBeReset: busyPopup.show(qsTr("Querying geographic data..."))
        onModelReset: busyPopup.close()

        onNetworkError: errorPopup.show(qsTr("Network error!"))
    }

    delegate: LoadListItem {
        itemTitle: name +
                   (adminName1 !== "" ? " (" +adminName1 + ")" : "") +
                   "\n" + countryName
        anchors.left: parent !== null ? parent.left : undefined
        anchors.right: parent !== null ? parent.right : undefined
        itemWidth: rowWidth
        editable: false
        revertedLayout: true
        selectable: index === geoListView.currentIndex
        selectIndicator: Image {
            anchors.verticalCenter: parent.verticalCenter
            source: parent.checked ? "/icons/pin_map_down_icon&16.png" : "/icons/pin_map_icon&16.png"
            verticalAlignment: Image.AlignVCenter
        }
        onItemClicked: geoListView.currentIndex = index
        onButtonClicked: loadButtonClicked(name, lat, lng)
    }
}
