
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
    property HoraScreenTimeZoneBox tzBox: null
    property int currentUnixTime: 0

    Material.background: "#DFEEE5"

    function setPosition(geoName,tzDiff)
    {
        geoNameBox.text = geoName.geoName
        geoLattBox.arcDegree = geoName.lattArcDegree
        geoLontBox.arcDegree = geoName.lontArcDegree
        tzBox.setHour(tzDiff / 3600)
    }

    BusyIndicator {
        id: busyIndicator
        anchors.centerIn: parent
        running: false
    }

    RestClient {
        id: timeZoneDbRestClient
        apiAddress: "http://api.timezonedb.com"
        authUser: "symboid"
    }

    property GeoNamesSearchItem selectedGeoName: null
    property GeoNamesSearchItem queryGeoName: null

    RestObjectModel {
        id: selectedTimeZone
        restClient: timeZoneDbRestClient
        operation: "v2.1/get-time-zone"+
                   "?key=7JKCP2G245UG"+
                   "&format=json"+
                   "&by=position"+
                   "&time="+currentUnixTime+
                   "&lat="+(selectedGeoName !== null ? selectedGeoName.lattArcDegree : 0)+
                   "&lng="+(selectedGeoName !== null ? selectedGeoName.lontArcDegree : 0)
        onModelAboutToBeReset: busyIndicator.running = true
        onModelReset: busyIndicator.running = false
        onSuccessfullyFinished: {
            setPosition(selectedGeoName, restObject.gmtOffset)
            close()
        }
        onNetworkError: {
            setPosition(selectedGeoName, 0)
            close()
        }
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
                        queryGeoName = selectedItem
                    }
                }
                canExec: queryGeoName !== null
                onExec: {
                    selectedGeoName = queryGeoName
                    selectedTimeZone.runOperation()
                }
            },
            InputOperation {
                title: qsTr("Current location")
                control: GeoNamesSearchItem {
                    geoName: currentGeoName.geoName
                    country: currentLoc.objectCount > 0 ? currentLoc.firstObject.countryName : ""
                    adminName: currentLoc.objectCount > 0 ? currentLoc.firstObject.adminName1 : ""
                    lattArcDegree: currentGeoName.lattArcDegree
                    lontArcDegree: currentGeoName.lontArcDegree
                }
                canExec: currentSource.valid
                onExec: {
                    setCurrent()
                    close()
                }
            }
        ]
    }

    property GeoNamesSearchItem currentGeoName: GeoNamesSearchItem {
        geoName: currentLoc.objectCount > 0 ? currentLoc.firstObject.name : ""
        country: currentLoc.objectCount > 0 ? currentLoc.firstObject.countryName : ""
        adminName: currentLoc.objectCount > 0 ? currentLoc.firstObject.adminName1 : ""
        lattArcDegree: currentSource.position.coordinate.latitude
        lontArcDegree: currentSource.position.coordinate.longitude
    }

    PositionSource {
        id: currentSource
        active: true
    }

    RestTableModel {
        id: currentLoc
        restClient: RestClient {
            apiAddress: "http://api.geonames.org"
            authUser: "symboid"
        }
        interactive: currentIsValid
        operation: "findNearbyPlaceNameJSON?"+
                   "lat="+currentSource.position.coordinate.latitude+
                   "&lng="+currentSource.position.coordinate.longitude+
                   "&cities=cities1000"+
                   "&lang="+Qt.locale().name.substring(0,2)+
                   "&username="+"symboid"
    }

    readonly property bool currentIsValid: currentSource.valid &&
            (currentSource.position.latitudeValid || currentSource.position.longitudeValid)
    function setCurrent()
    {
        selectedGeoName = currentGeoName
        selectedTimeZone.runOperation()
    }
}
