
import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import Symboid.Sdk.Controls 1.0
import Symboid.Astro.Controls 1.0
import QtPositioning 5.12
import Symboid.Astro.Db 1.0
import Symboid.Sdk.Network 1.0
import QtQuick.Controls.Material 2.3

Drawer {
    property TextField geoNameBox: null
    property ArcCoordBox geoLattBox: null
    property ArcCoordBox geoLontBox: null

    signal geoNameChanged

    property int rowWidth: width

    LoadListToolBar {
        id: toolBar
        listView: geoListView
        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
        }

        toolModel: ListModel {
            ListElement {
                iconSource: "/icons/zoom_icon&32.png"
                withTextInput: true
            }
        }
        onTextInputClicked: {
            restTableModel.runOperation()
        }
    }

    LoadListView {
        id: geoListView
        anchors {
            top: toolBar.bottom
            left: parent.left
            right: parent.right
            bottom: parent.bottom
        }

        model: restTableModel

        delegate: LoadListItem {
            itemTitle: name +
                       (adminName1 !== "" ? " (" +adminName1 + ")" : "") +
                       "\n" + countryName
            anchors.left: parent.left
            anchors.right: parent.right
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
            onButtonClicked: {
                geoNameBox.text = name
                geoLattBox.arcDegree = lat
                geoLontBox.arcDegree = lng
                geoNameChanged()
                close()
            }
        }

        BusyIndicator {
            id: busyIndicator
            anchors.centerIn: parent
            running: false
        }
    }

    property int maxRows: 10
    readonly property string apiUser: "symboid"
    RestTableModel {
        id: restTableModel
        restClient: GeoNamesRestClient
        operation: "searchJSON?q="+toolBar.textInput+
                   "&lang="+Qt.locale().name.substring(0,2)+
                   "&maxRows="+maxRows+
                   "&username="+apiUser
        columnNames: ["name", "countryName", "adminName1", "population", "lng", "lat"]

        onModelAboutToBeReset: busyIndicator.running = true
        onModelReset: busyIndicator.running = false
        onSuccessfullyFinished: {
            toolBar.textInputClose()
        }

        onNetworkError: {
            networkStatusDialog.message = qsTr("Netwok error!")
            networkStatusDialog.open()
        }
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
