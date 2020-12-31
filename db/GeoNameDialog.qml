
import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import Symboid.Sdk.Controls 1.0
import Symboid.Astro.Controls 1.0
import QtPositioning 5.12
import Symboid.Astro.Db 1.0
import Symboid.Sdk.Network 1.0

Drawer {
    property TextField geoNameBox: null
    property ArcCoordBox geoLattBox: null
    property ArcCoordBox geoLontBox: null

    signal geoNameChanged

    property int rowWidth: width

    interactive: opened

    ToolBar {
        id: pageBar
        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
        }
        Column {
            anchors.horizontalCenter: parent.horizontalCenter
            PageIndicator {
                id: pageSelector
                anchors.horizontalCenter: parent.horizontalCenter
                interactive: true
                count: 3
                delegate: ToolButton {
                    checkable: pageSelector.currentIndex !== index
                    checked: pageSelector.currentIndex === index
                    icon.source: {
                        switch (index)
                        {
                        case 0: return "/icons/db_icon&32.png"
                        case 1: return "/icons/server_icon&32.png"
                        case 2: return "/icons/home_icon&32.png"
                        default: return ""
                        }
                    }
                    onClicked: pageSelector.currentIndex = index
                }
            }
            Pane {
                anchors.horizontalCenter: parent.horizontalCenter
                background: null
                Label {
                    text: {
                        switch(pageSelector.currentIndex)
                        {
                        case 0: return qsTr("Database search")
                        case 1: return qsTr("Query via internet")
                        case 2: return qsTr("Surrounding locations")
                        default: return ""
                        }
                    }
                    font.italic: true
                }
            }
        }
   }

    StackLayout {
        anchors {
            top: pageBar.bottom
            left: parent.left
            right: parent.right
            bottom: parent.bottom
        }
        currentIndex: pageSelector.currentIndex
        Item {

        }
        Item {
            LoadListItem {
                id: onlineSearchInput
                anchors {
                    top: parent.top
                    left: parent.left
                    right: parent.right
                }
                itemWidth: width
                loadIconSource: "/icons/zoom_icon&32.png"
                lineColor: pageBar.background.color
                onEditAccepted: onlineSearchView.runQuery()
                onButtonClicked: onlineSearchView.runQuery()
            }
            GeoListView {
                id: onlineSearchView
                anchors {
                    top: onlineSearchInput.bottom
                    left: parent.left
                    right: parent.right
                    bottom: parent.bottom
                }
                queryOperation: "searchJSON?q="+onlineSearchInput.itemTitle
                onLoadButtonClicked: {
                    geoNameBox.text = geoName
                    geoLattBox.arcDegree = geoLat
                    geoLontBox.arcDegree = geoLng
                    geoNameChanged()
                    close()
                }
            }
        }
        Item {
            Pane {
                id: currentLocationsPane
                anchors {
                    top: parent.top
                    left: parent.left
                    right: parent.right
                }
                background: null
                Button {
                    anchors.top: parent.top
                    anchors.right: parent.right
                    text: qsTr("Execute query")
                    onClicked: {
                        currentLocationsView.runQuery()
                    }
                }
            }
            GeoListView {
                id: currentLocationsView
                anchors {
                    top: currentLocationsPane.bottom
                    left: parent.left
                    right: parent.right
                    bottom: parent.bottom
                }
                queryOperation: "findNearbyJSON"+
                           "?lat="+pos.lat+
                           "&lng="+pos.lng+
                           "&featureCode=PPL"+
                           "&radius=10"

                onLoadButtonClicked: {
                    geoNameBox.text = geoName
                    geoLattBox.arcDegree = geoLat
                    geoLontBox.arcDegree = geoLng
                    geoNameChanged()
                    close()
                }
                PositionSource {
                    id: pos
                    updateInterval: 1000
                    active: true
                    readonly property real lng: position.coordinate.longitude
                    readonly property real lat: position.coordinate.latitude
                    onPositionChanged: {
                        active = false
                    }
                }
            }
        }
    }
}
