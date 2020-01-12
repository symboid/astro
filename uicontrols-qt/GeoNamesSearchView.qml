
import QtQuick 2.12
import QtQuick.Controls 2.5
import Symboid.Astro.Controls 1.0
import Symboid.Sdk.Network 1.0

ListView {

    property string geoName: ""
    property int maxRows: 10
    readonly property string apiUser: "symboid"
    readonly property GeoNamesSearchItem selectedItem: currentIndex !== -1 ? currentItem.geoItem : null

    model: restTableModel

    RestTableModel {
        id: restTableModel
        restClient: GeoNamesRestClient
        operation: "searchJSON?q="+geoName+
                   "&lang="+Qt.locale()+
                   "&maxRows="+maxRows+
                   "&username="+apiUser
        columnNames: ["name", "countryName", "adminName1", "population", "lng", "lat"]

        onModelAboutToBeReset: busyPopup.open()
        onModelReset: busyPopup.close()
    }
    function update()
    {
        restTableModel.runOperation()
    }

    delegate: Rectangle {
        width: parent.width
        height: searchItem.height
        color: index == currentIndex ? "lightgray" : "white"
        property GeoNamesSearchItem geoItem: searchItem

        Rectangle {
            height: 1
            width: parent.width
            color: "black"
            visible: index > 0
        }
        GeoNamesSearchItem {
            id: searchItem
            width: parent.width

//            lattArcDegree: lat
//            lontArcDegree: lng
            geoName: name
            adminName: adminName1
            country: countryName
        }
        MouseArea {
            anchors.fill: parent
            onClicked: currentIndex = index
        }
    }

    focus: true
    clip: true

    Dialog {
        id: busyPopup
        width: 100
        height: width
        anchors.centerIn: parent
        BusyIndicator {
            anchors.centerIn: parent
            running: true
        }
    }
}
