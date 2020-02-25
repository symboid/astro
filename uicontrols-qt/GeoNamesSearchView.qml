
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
                   "&lang="+Qt.locale().name.substring(0,2)+
                   "&maxRows="+maxRows+
                   "&username="+apiUser
        columnNames: ["name", "countryName", "adminName1", "population", "lng", "lat"]

        onModelAboutToBeReset: busyIndicator.running = true
        onModelReset: busyIndicator.running = false
        onNetworkError: {
            networkStatusDialog.message = qsTr("Netwok error!")
            networkStatusDialog.open()
        }
    }
    function update()
    {
        currentIndex = -1
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

            geoName: name
            adminName: adminName1
            country: countryName
            Component.onCompleted: {
                lattArcDegree = lat
                lontArcDegree = lng
            }
        }
        MouseArea {
            anchors.fill: parent
            onClicked: currentIndex = index
        }
    }

    focus: true
    clip: true

    BusyIndicator {
        id: busyIndicator
        anchors.centerIn: parent
        running: false
    }

    Dialog {
        id: networkStatusDialog
        anchors.centerIn: parent
        width: 150
        onOpened: networkStatusTimer.start()
        property alias message: messageLabel.text
        Label {
            anchors.centerIn: parent
            id: messageLabel
            font.bold: true
            color: "red"
        }
        Timer {
            id: networkStatusTimer
            interval: 2000
            onTriggered: networkStatusDialog.close()
        }
    }
}
