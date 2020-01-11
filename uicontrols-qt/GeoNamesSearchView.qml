
import QtQuick 2.12
import QtQuick.Controls 2.5
import Symboid.Astro.Controls 1.0
import Symboid.Sdk.Network 1.0

ListView {

    property string geoName: ""
    property int maxRows: 10
    readonly property string apiUser: "symboid"

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

    delegate: Rectangle {
        width: parent.width
        height: itemFlow.height
        color: index == currentIndex ? "lightgray" : "white"

        Flow {
            id: itemFlow
            width: parent.width
            spacing: 10
            padding: 10

            Label {
                height: coordsBox.height
                verticalAlignment: Label.AlignVCenter
                text: name
            }
            Label {
                height: coordsBox.height
                verticalAlignment: Label.AlignVCenter
                text: "("+countryName+"/"+adminName1+")"
            }
            Row {
                id: coordsBox
                spacing: 10
                GeoCoordBox {
                    value: lng
                }
                GeoCoordBox {
                    isLattitude: true
                    value: lat
                }
            }
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
