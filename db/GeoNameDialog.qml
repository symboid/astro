
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

    interactive: opened

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
                toolAction: function() {}
            }
        }
        onTextInputClicked: {
            restTableModel.runOperation()
        }
    }
    onOpened: {
        toolBar.textInputIndex = 0
        toolBar.textInputShow(true)
    }

    LoadListView {
        id: geoListView
        anchors {
            top: toolBar.bottom
            left: parent.left
            right: parent.right
            bottom: labelPane.top
        }

        model: RestTableModel {
            id: restTableModel
            restClient: GeoNamesRestClient
            operation: "searchJSON?q="+toolBar.textInput+
                       "&lang="+Qt.locale().name.substring(0,2)+
                       "&maxRows=10"+
                       "&username=symboid"
            columnNames: ["name", "countryName", "adminName1", "population", "lng", "lat"]

            onModelAboutToBeReset: busyPopup.show(qsTr("Querying geographic data..."))
            onModelReset: busyPopup.close()

            onSuccessfullyFinished: toolBar.textInputShow(false)
            onNetworkError: errorPopup.show(qsTr("Netrwork error!"))
        }

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
    }

    Pane {
        id: labelPane
        anchors {
            left: parent.left
            right: parent.right
            bottom: currentLocation.top
        }
        Label {
            text: qsTr("Current location:")
            font.italic: true
        }
    }

    LoadListItem {
        id: currentLocation
        itemTitle: findLocation.name + " (" + findLocation.adminName +")\n" + findLocation.countryName
        itemWidth: rowWidth
        editable: false
        revertedLayout: true
        anchors {
            left: parent.left
            right: parent.right
            bottom: parent.bottom
        }
        PositionSource {
            id: pos
            updateInterval: 1000
            active: true
            readonly property real lng: position.coordinate.longitude
            readonly property real lat: position.coordinate.latitude
            onPositionChanged: {
                findLocation.runOperation()
                active = false
            }
        }
        RestTableModel {
            id: findLocation
            restClient: GeoNamesRestClient
            operation: "findNearbyJSON"+
                       "?lat="+pos.lat+
                       "&lng="+pos.lng+
                       "&featureClass=P"+
                       "&lang="+Qt.locale().name.substring(0,2)+
                       "&maxRows=10"+
                       "&username=symboid"
            columnNames: ["name", "countryName", "adminName1"]
            readonly property string name: objectCount > 0 ? firstObject.name : "???"
            readonly property string adminName: objectCount > 0 ? firstObject.adminName1 : "???"
            readonly property string countryName: objectCount > 0 ? firstObject.countryName : "???"
        }
        onButtonClicked: {
            geoNameBox.text = findLocation.name
            geoLattBox.arcDegree = pos.lat
            geoLontBox.arcDegree = pos.lng
            geoNameChanged()
            close()
        }
    }
}
