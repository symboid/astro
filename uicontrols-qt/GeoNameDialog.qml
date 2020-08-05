
import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
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

    Material.background: "#DFEEE5"

    function setPosition(geoName)
    {
        geoNameBox.text = geoName.geoName
        geoLattBox.arcDegree = geoName.lattArcDegree
        geoLontBox.arcDegree = geoName.lontArcDegree
        tzBox.search()
    }

    property GeoNamesSearchItem queryGeoName: null
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
                    setPosition(queryGeoName)
                    close()
                }
            },
            InputOperation {
                title: qsTr("Current location")
                control: StackLayout {
                    currentIndex: currentIsValid ? 0 : 1
                    GeoNamesSearchItem {
                        geoName: currentGeoName.geoName
                        country: currentLoc.objectCount > 0 ? currentLoc.firstObject.countryName : ""
                        adminName: currentLoc.objectCount > 0 ? currentLoc.firstObject.adminName1 : ""
                        lattArcDegree: currentGeoName.lattArcDegree
                        lontArcDegree: currentGeoName.lontArcDegree
                    }
                    Label {
                        text: qsTr("Not available!")
                        color: "red"
                        font.bold: true
                    }
                }
                canExec: currentIsValid
                onExec: {
                    setCurrent()
                    close()
                }
            },
            RecentGeoNamesOperation {
                title: qsTr("Recent locations")
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
        if (currentIsValid)
        {
            setPosition(currentGeoName)
        }
    }
}
