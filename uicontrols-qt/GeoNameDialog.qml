
import QtQuick 2.12
import QtQuick.Controls 2.12
import Symboid.Sdk.Controls 1.0
import Symboid.Astro.Controls 1.0
import QtPositioning 5.12
import Symboid.Sdk.Network 1.0
import QtQuick.Controls.Material 2.3

Drawer {
    property TextField geoNameBox: null
    property ArcCoordBox geoLattBox: null
    property ArcCoordBox geoLontBox: null

    Material.background: "#DFEEE5"

    QtObject {
        id: temp
        property GeoNamesSearchItem selectedGeoName: null
        property GeoNamesSearchItem currentGeoName: null
    }

    InputOperationsView {
        anchors.fill: parent
        leftAligned: edge === Qt.LeftEdge
        operations: [
            InputOperation {
                title: qsTr("Geographic name lookup")

                control: GeoNamesSearchBox {
                    height: 500
                    onSelectedItemChanged: {
                        temp.selectedGeoName = selectedItem
                    }
                }
                canExec: temp.selectedGeoName !== null
                onExec: {
                    geoNameBox.text = temp.selectedGeoName.geoName
                    geoLattBox.arcDegree = temp.selectedGeoName.lattArcDegree
                    geoLontBox.arcDegree = temp.selectedGeoName.lontArcDegree
                    close()
                }
            },
            InputOperation {
                title: qsTr("Current location")
                control: GeoNamesSearchItem {
                    id: currentLocation
                    geoName: currentLoc.firstObject.name
                    country: currentLoc.firstObject.countryName
                    adminName: currentLoc.firstObject.adminName1
                    lattArcDegree: currentSource.position.coordinate.latitude
                    lontArcDegree: currentSource.position.coordinate.longitude
                }
                canExec: currentSource.valid
                onExec: {
                    setCurrent()
                    close()
                }
            }
        ]
    }
    PositionSource {
        id: currentSource
        active: true
    }
    RestTableModel {
        id: currentLoc
        restClient: GeoNamesRestClient
        interactive: true
        operation: "findNearbyPlaceNameJSON?"+
                   "lat="+currentSource.position.coordinate.latitude+
                   "&lng="+currentSource.position.coordinate.longitude+
                   "&cities=cities1000"+
                   "&lang="+Qt.locale().name.substring(0,2)+
                   "&username="+"symboid"
        onNetworkError: {
        }
    }

    readonly property bool currentIsValid: currentSource.valid &&
            (currentSource.position.latitudeValid || currentSource.position.longitudeValid)
    function setCurrent()
    {
        if (currentSource.valid)
        {
            geoNameBox.text = currentLoc.firstObject.name
            if (currentSource.position.latitudeValid)
            {
                geoLattBox.arcDegree = currentSource.position.coordinate.latitude
            }
            if (currentSource.position.longitudeValid)
            {
                geoLontBox.arcDegree = currentSource.position.coordinate.longitude
            }
        }
    }
}
