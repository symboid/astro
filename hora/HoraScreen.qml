
import QtQuick 2.12
import QtQuick.Controls 2.5
import Symboid.Sdk.Controls 1.0
import Symboid.Astro.Controls 1.0
import Symboid.Astro.Db 1.0
import Symboid.Astro.Hora 1.0
import QtPositioning 5.12
import QtQuick.Layouts 1.12

MainScreen {

    property alias fontPointSize: horaView.fontPointSize

    // parameters:
    property alias title: horaName.text
    property alias interactive: horaView.interactive
    // date and time
    property alias year: dateTimeParams.year
    property alias month: dateTimeParams.month
    property alias day: dateTimeParams.day
    property alias hour: dateTimeParams.hour
    property alias minute: dateTimeParams.minute
    property alias second: dateTimeParams.second
    // geo loacation attributes:
    property alias geoName: geoName.text
    property alias geoLatt: geoLatt.arcDegree
    property alias geoLont: geoLont.arcDegree
    property alias geoTzDiff: timeZoneBox.diffHours
    // other details:
    property alias calendarType: calendarType.currentIndex

    function setCurrent()
    {
        dateTimeParams.currentTimerOn = true
        currentLocTimer.checked = true
    }
    function unsetCurrent()
    {
        dateTimeParams.currentTimerOn = false
        currentLocTimer.checked = false
    }
    function zoomToDefault()
    {
        horaFlickable.zoomToDefault()
    }

    MainScreenParamBox {
        title: qsTr("Horoscope name")
        MainScreenTextField {
            id: horaName
        }
    }

    MainScreenDateTimeBox {
        id: dateTimeParams
        showDetails: details.checked

    }

    MainScreenParamBox {
        id: calendarParam
        title: qsTr("Calendar")
        visible: details.checked
        MainScreenComboBox {
            id: calendarType
            model: [ "Gregorian", "Julian" ]
        }
    }

    MainScreenViewSelector {
        id: viewSelector
        viewNames: [ qsTr("Chart"), qsTr("Planet positions"), qsTr("House cusps") ]
        referenceItem: details.checked ? calendarParam : dateTimeParams
    }

    StackLayout {
        width: metrics.mandalaSize
        height: metrics.mandalaSize
        currentIndex: viewSelector.currentIndex
        Rectangle {
            border.width: horaFlickable.minHoraSize != horaFlickable.horaSize ? 1 : 0
            border.color: "lightgray"
            color: "transparent"
            Flickable {
                id: horaFlickable
                anchors.fill: parent
                contentWidth: horaView.width
                contentHeight: horaView.height
                clip: true

                function zoomToMinimum()
                {
                    horaSize = minHoraSize
                    contentX = 0
                    contentY = 0
                }
                function zoomToDefault()
                {
                    horaSize = minHoraSize / horaView.defaultZoom
                    contentX = (horaSize - minHoraSize) / 2
                    contentY = contentX
                }
                function zoomTo(zoomPointX,zoomPointY,zoomDelta)
                {
                    var zoomRatio = (horaSize + zoomDelta) / horaSize
                    if (horaSize + zoomDelta >= minHoraSize)
                    {
                        horaSize += zoomDelta
                        var newContentX = contentX + (zoomRatio - 1) * zoomPointX
                        var newContentY = contentY + (zoomRatio - 1) * zoomPointY
                        contentX = newContentX < 0.0 ? 0.0 : newContentX
                        contentY = newContentY < 0.0 ? 0.0 : newContentY
//                            console.log("contentCorner = ("+contentX+","+contentY+"), zoom=("+zoomDelta+","+zoomRatio+")")
                    }
                    else
                    {
                        zoomToMinimum()
                    }
                }

                onWidthChanged:  zoomToDefault()
                onHeightChanged: zoomToDefault()

                readonly property int minHoraSize: metrics.mandalaSize
                property int horaSize: metrics.mandalaSize
                MouseArea {
                    anchors.fill: parent
                    onWheel: {
                        var zoomDelta = (wheel.angleDelta.y/500.0) * horaFlickable.horaSize
                        horaFlickable.zoomTo(wheel.x, wheel.y, zoomDelta)
                    }
                    onClicked: {
//                            console.log("contentCorner = ("+contentX+","+contentY+")")
                    }

                    onDoubleClicked: horaFlickable.zoomToDefault()
                }
                PinchArea {
                    anchors.fill: parent
                    onPinchUpdated: {
                        var zoomDelta = (pinch.scale > 1 ? 1 : -1) * horaFlickable.horaSize / 30
                        horaFlickable.zoomTo(pinch.center.x, pinch.center.y, zoomDelta)
                    }
                }

                HoraView {
                    id: horaView
                    width: horaFlickable.horaSize
                    height: horaFlickable.horaSize

                    year: dateTimeParams.year
                    month: dateTimeParams.month
                    day: dateTimeParams.day
                    hour: dateTimeParams.hour
                    minute: dateTimeParams.minute
                    second: dateTimeParams.second
                    geoLatt: geoLatt.arcDegree
                    geoLont: geoLont.arcDegree
                    tzDiff: timeZoneBox.diffHours
                    housesType: housesType.currentToken()
                    withJulianCalendar: calendarType.currentIndex !== 0

                    displayFlags: HoraView.SHOW_FIXSTARS

                    BusyIndicator {
                        id: horaCalcIndicator
                        width: 100
                        height: 100
                        anchors.centerIn: parent
                        running: false
                    }
                    onStartCalc: horaCalcIndicator.running = true
                    onStopCalc: horaCalcIndicator.running = false
                }
            }
        }
        Item {
            HoraTableView {
                id: planetTableView
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.bottom: showPlanetSeconds.top
                anchors.margins: 20
                tableModel: horaView.planetsModel
                headerModel: tableModel.headerModel
                columnComponents: [
                    Component {
                        Pane {
                            Label {
                                text: cellData
                                font.family: "Symboid"
                                width: 40
                            }
                        }
                    },
                    Component {
                        ArcCoordLabel {
                            arcDegree: cellData
                            sectionCalc: ZodiacSectionCalc {}
                            sectionFont.family: "Symboid"
                            showSecond: showPlanetSeconds.checked
                        }
                    },
                    Component {
                        ArcCoordLabel {
                            arcDegree: cellData
                            sectionCalc: SignumSectionCalc {}
                            showSecond: showPlanetSeconds.checked
                        }
                    },
                    Component {
                        ArcCoordLabel {
                            arcDegree: cellData
                            sectionCalc: SignumSectionCalc {}
                            showSecond: showPlanetSeconds.checked
                        }
                    }
                ]
            }
            CheckBox {
                id: showPlanetSeconds
                anchors {
                    horizontalCenter: parent.horizontalCenter
                    bottom: parent.bottom
                    bottomMargin: 20
                }
                text: qsTr("Show seconds")
                onCheckedChanged: {
                    planetTableView.tableModel.update()
                }
            }
        }
        Item {
            HoraTableView {
                id: houseTableView
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.bottom: showHousesSeconds.top
                anchors.margins: 20
                tableModel: horaView.housesModel
                headerModel: tableModel.headerModel
                columnComponents: [
                    Component {
                        Pane {
                            Label {
                                text: cellData
                                anchors.horizontalCenter: parent.horizontalCenter
                            }
                        }
                    },
                    Component {
                        ArcCoordLabel {
                            arcDegree: cellData
                            sectionCalc: ZodiacSectionCalc {}
                            sectionFont.family: "Symboid"
                            showSecond: showHousesSeconds.checked
                        }
                    },
                    Component {
                        ArcCoordLabel {
                            arcDegree: cellData
                            sectionCalc: SignumSectionCalc {}
                            showSecond: showHousesSeconds.checked
                        }
                    }
                ]
            }
            CheckBox {
                id: showHousesSeconds
                anchors {
                    horizontalCenter: parent.horizontalCenter
                    bottom: parent.bottom
                    bottomMargin: 20
                }
                text: qsTr("Show seconds")
                onCheckedChanged: {
                    houseTableView.tableModel.update()
                }
            }
        }
    }

    MainScreenParamBox {
        id: locationParams
        title: qsTr("Location")

        MainScreenTextField {
            id: geoName
            enabled: !currentLocTimer.checked
            button: RoundButton {
                display: RoundButton.IconOnly
                padding: 0
                icon.source: "/icons/globe_3_icon&48.png"
                icon.color: "darkblue"
                icon.width: width - leftInset - rightInset
                icon.height: height - topInset - bottomInset
                onClicked: geoNameDialog.open()
            }
        }
        ArcCoordBox {
            id: geoLatt
            visible: details.checked
            sectionCalc: GeoLattSectionCalc {}
            editable: true
            enabled: !currentLocTimer.checked
        }
        ArcCoordBox {
            id: geoLont
            visible: details.checked
            sectionCalc: GeoLontSectionCalc {}
            editable: true
            enabled: !currentLocTimer.checked
        }
        GeoTimeZoneBox {
            id: timeZoneBox
            visible: details.checked
            enabled: !currentLocTimer.checked
            geoNameLatt: geoLatt.arcDegree
            geoNameLont: geoLont.arcDegree
            currentUnixTime: dateTimeParams.unixTime
        }
        MainScreenTimer {
            id: currentLocTimer
            text: qsTr("Current")
            visible: details.checked
            onTriggered: geoNameDialog.setCurrent()
            enabled: geoNameDialog.currentIsValid
        }
    }

    MainScreenParamBox {
        id: houseSystemParams
        title: qsTr("House system")
        visible: details.checked

        MainScreenComboBox {
            id: housesType
            textRole: "name"
            model: ListModel {
                ListElement {
                    name: qsTr("Placidus")
                    token: "placidus"
                }
                ListElement {
                    name: qsTr("Koch")
                    token: "koch"
                }
                ListElement {
                    name: qsTr("Regiomontanus")
                    token: "regiomontanus"
                }
                ListElement {
                    name: qsTr("Campanus")
                    token: "campanus"
                }
                ListElement {
                    name: qsTr("Equal")
                    token: "equal"
                }
            }
            function currentToken()
            {
                return model.data(model.index(currentIndex, 0))
            }
        }
    }

    MainScreenBottomPane {
        referenceItem: details.checked ? houseSystemParams : locationParams
        controlItem: Frame {
            Switch {
                id: details
                anchors.centerIn: parent
                text: qsTr("Details")
            }
        }
    }

    GeoNameDialog {
        id: geoNameDialog
        width: Math.min(400,parent.width)
        height: parent.height
        edge: Qt.RightEdge
        geoNameBox: geoName.item
        geoLattBox: geoLatt
        geoLontBox: geoLont
        opacity: 0.875
        onGeoNameChanged: timeZoneBox.search()
    }

    Component.onCompleted: interactive = true
}
