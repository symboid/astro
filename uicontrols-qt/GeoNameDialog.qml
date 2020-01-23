
import QtQuick 2.12
import QtQuick.Controls 2.12
import Symboid.Sdk.Controls 1.0
import Symboid.Astro.Controls 1.0
import QtPositioning 5.12

Drawer {
    property TextField geoNameBox: null
    property ArcCoordBox geoLattBox: null
    property ArcCoordBox geoLontBox: null

    QtObject {
        id: temp
        property GeoNamesSearchItem selectedGeoName: null
        property GeoNamesSearchItem currentGeoName: null
    }

    DocItemOpsView {
        anchors.fill: parent
        leftAligned: edge === Qt.LeftEdge
        operations: Container {
            DocItemOp {
                title: qsTr("Geographic name lookup:")

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
            }
            DocItemOp {
                title: qsTr("Current location")
                control: GeoNamesSearchItem {
                    id: currentLocation
                    PositionSource {
                        id: positionSrc
                        updateInterval: 1000
                        active: true
                        onPositionChanged: {
                            if (valid)
                            {
                                if (position.latitudeValid)
                                {
                                    currentLocation.lattArcDegree = position.coordinate.latitude
                                }
                                if (position.longitudeValid)
                                {
                                    currentLocation.lontArcDegree = position.coordinate.longitude
                                }
                                temp.currentGeoName = currentLocation
                            }
                        }
                    }
                }
                canExec: temp.currentGeoName !== null
                onExec: {
                    geoNameBox.text = temp.currentGeoName.geoName
                    geoLattBox.arcDegree = temp.currentGeoName.lattArcDegree
                    geoLontBox.arcDegree = temp.currentGeoName.lontArcDegree
                    close()
                }
            }
        }
    }
}
/*/
Drawer {
    property TextField geoNameBox: null
    property ArcCoordBox geoLattBox: null
    property ArcCoordBox geoLontBox: null

    opacity: 0.875

    Flickable {
        anchors.fill: parent
        flickableDirection: Flickable.VerticalFlick
        contentHeight: searchGeoName.height
        Flow {
            id: searchGeoName
            anchors.left: parent.left
            anchors.right: parent.right
            Rectangle {
                width: parent.width
                height: 1
                color: "black"
            }
            Label {
                padding: 15
                width: parent.width
                font.italic: true
                font.bold: true
                text: qsTr("Geographic name lookup:")
            }
            Item {
                height: searchBox.height
                width: searchButton.width
                RoundButton {
                    id: searchButton
                    y: searchBox.height / 2
                    icon.source: "file:///home/robert/Munka/icons/black/png/br_down_icon&16.png"
                    enabled: searchBox.selectedItem !== null
                    onClicked: {
                        if (searchBox.selectedItem)
                        {
                            if (geoNameBox)
                            {
                                geoNameBox.text = searchBox.selectedItem.geoName
                            }
                            if (geoLattBox)
                            {
                                geoLattBox.arcDegree = searchBox.selectedItem.lattArcDegree
                            }
                            if (geoLontBox)
                            {
                                geoLontBox.arcDegree = searchBox.selectedItem.lontArcDegree
                            }
                            close()
                        }
                    }
                }
            }

            GeoNamesSearchBox {
                id: searchBox
                width: parent.width - searchButton.width - 15
                height: 500
            }
            Rectangle {
                width: parent.width
                height: 1
                color: "black"
            }
            Label {
                padding: 15
                width: parent.width
                font.italic: true
                font.bold: true
                text: qsTr("Current geographic location:")
            }
            Item {
                height: currLoc.height
                width: currButton.width
                RoundButton {
                    id: currButton
                    icon.source: "file:///home/robert/Munka/icons/black/png/br_down_icon&16.png"
                    enabled: positionSrc.sourceError === PositionSource.NoError
                    onClicked: {
                        if (geoNameBox)
                        {
                            geoNameBox.text = currLoc.geoName
                        }
                        if (geoLattBox)
                        {
                            geoLattBox.arcDegree = currLoc.lattArcDegree
                        }
                        if (geoLontBox)
                        {
                            geoLontBox.arcDegree = currLoc.lontArcDegree
                        }
                        close()
                    }
                }
            }
            GeoNamesSearchItem {
                id: currLoc

                width: parent.width - currButton.width - 15
                PositionSource {
                    id: positionSrc
                    updateInterval: 1000
                    active: true
                    onPositionChanged: {
                        if (valid)
                        {
                            if (position.latitudeValid)
                            {
                                currLoc.lattArcDegree = position.coordinate.latitude
                            }
                            if (position.longitudeValid)
                            {
                                currLoc.lontArcDegree = position.coordinate.longitude
                            }
                        }
                    }
                }
            }
            Rectangle {
                width: parent.width
                height: 1
                color: "black"
            }
        }
    }
}
/*/
