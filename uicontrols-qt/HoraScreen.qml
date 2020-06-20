
import QtQuick 2.12
import QtQuick.Controls 2.5
import Symboid.Sdk.Controls 1.0
import Symboid.Astro.Controls 1.0
import QtPositioning 5.12
import QtQuick.Layouts 1.12

Flickable {

    // metrics:
    readonly property bool isLandscape: width > height
    readonly property int mandalaSize: isLandscape ? height : width
    readonly property int screenSize: isLandscape ? width : height
    readonly property int restSize: screenSize - mandalaSize

    property alias fontPointSize: horaView.fontPointSize

    // parameters:
    property alias title: horaName.text
    property alias interactive: horaView.interactive
    // date and time
    property alias year: dateBox.year
    property alias month: dateBox.month
    property alias day: dateBox.day
    property alias hour: timeBox.hour
    property alias minute: timeBox.minute
    property alias second: timeBox.second
    // geo loacation attributes:
    property alias geoName: geoName.text
    property alias geoLatt: geoLatt.arcDegree
    property alias geoLont: geoLont.arcDegree
    property alias geoTzDiff: timeZoneBox.diffHours
    // other details:
    property alias calendarType: calendarType.currentIndex

    flickableDirection: isLandscape ? Flickable.HorizontalFlick : Flickable.VerticalFlick
    contentWidth: horaScreen.width
    contentHeight: horaScreen.height
    clip: true

    function setCurrent()
    {
        currentTimeTimer.checked = true
        currentLocTimer.checked = true
    }
    function unsetCurrent()
    {
        currentTimeTimer.checked = false
        currentLocTimer.checked = false
    }

    Flow {
        id: horaScreen

        width: isLandscape ? childrenRect.width : mandalaSize
        height: isLandscape ? mandalaSize : childrenRect.height

        readonly property int minParamSectionWidth: 300
        readonly property int paramSectionWidth:
            isLandscape ? ((restSize / 2) < minParamSectionWidth ? minParamSectionWidth : restSize / 2)
                        : ((mandalaSize / 2) < minParamSectionWidth ? mandalaSize : mandalaSize / 2)
        readonly property int paramSectionPadding: 20

        flow: isLandscape ? Flow.TopToBottom : Flow.LeftToRight

        HoraScreenParamBox {
            title: qsTr("Horoscope name")
            HoraScreenTextField {
                id: horaName
            }
        }

        HoraScreenParamBox {
            id: dateTimeParams
            title: qsTr("Date and time")

            DateCoordBox {
                id: dateBox
                enabled: !currentTimeTimer.checked
                editable: true
            }
            TimeCoordBox {
                id: timeBox
                enabled: !currentTimeTimer.checked
                editable: true
                circularLink: dateBox.dayLink
            }
            HoraScreenTimer {
                id: currentTimeTimer
                text: qsTr("Current")
                visible: details.checked
                onTriggered: {
                    dateBox.setCurrent()
                    timeBox.setCurrent()
                }
            }
            UnixTimeConverter {
                id: unixTimeConverter
                year: dateBox.year
                month: dateBox.month
                day: dateBox.day
                hour: timeBox.hour
                minute: timeBox.minute
                second: timeBox.second
            }
        }

        HoraScreenParamBox {
            id: calendarParam
            title: qsTr("Calendar")
            visible: details.checked
            HoraScreenComboBox {
                id: calendarType
                model: [ "Gregorian", "Julian" ]
            }
        }

        HoraScreenBottomPane {
            id: viewSelector
            property int currentIndex: 0
            referenceItem: details.checked ? calendarParam : dateTimeParams
            landscape: isLandscape
            width: parent.paramSectionWidth
            padding: parent.paramSectionPadding
            controlItem: SpinBox {
                id: viewSpin
                to: viewNames.length - 1
                onValueChanged: viewSelector.currentIndex = value
                property var viewNames: [ qsTr("Chart"), qsTr("Planet positions"), qsTr("House cusps") ]
                textFromValue: function(value, locale)
                {
                    return viewNames[value]
                }

                down.indicator: Image {
                    anchors.left: parent.left
                    anchors.verticalCenter: parent.verticalCenter
                    opacity: viewSpin.value > viewSpin.from ? 1.0 : 0.1
                    source: "/icons/br_prev_icon&24.png"
                }
                up.indicator: Image {
                    anchors.right: parent.right
                    anchors.verticalCenter: parent.verticalCenter
                    opacity: viewSpin.value < viewSpin.to ? 1.0 : 0.1
                    source: "/icons/br_next_icon&24.png"
                }
            }
        }

        StackLayout {
            width: mandalaSize
            height: mandalaSize
            currentIndex: viewSelector.currentIndex
            HoraView {
                id: horaView

                year: dateBox.year
                month: dateBox.month
                day: dateBox.day
                hour: timeBox.hour
                minute: timeBox.minute
                second: timeBox.second
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

        HoraScreenParamBox {
            id: locationParams
            title: qsTr("Location")

            HoraScreenTextField {
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
            HoraScreenTimeZoneBox {
                id: timeZoneBox
                visible: details.checked
                enabled: !currentLocTimer.checked
                geoNameLatt: geoLatt.arcDegree
                geoNameLont: geoLont.arcDegree
                currentUnixTime: unixTimeConverter.unixTime
            }
            HoraScreenTimer {
                id: currentLocTimer
                text: qsTr("Current")
                visible: details.checked
                onTriggered: geoNameDialog.setCurrent()
                enabled: geoNameDialog.currentIsValid
            }
        }
        HoraScreenParamBox {
            id: houseSystemParams
            title: qsTr("House system")
            visible: details.checked

            HoraScreenComboBox {
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

        HoraScreenBottomPane {
            referenceItem: details.checked ? houseSystemParams : locationParams
            landscape: isLandscape
            width: parent.paramSectionWidth
            padding: parent.paramSectionPadding
            controlItem: Frame {
                height: 60
                Item {
                    anchors {
                        left: parent.left
                        top: parent.top
                        bottom: parent.bottom
                        right: parent.horizontalCenter
                    }
                    RoundButton {
                        anchors.left: parent.left
                        anchors.centerIn: parent
                        icon.source: "/icons/cog_icon&32.png"
                        text: qsTr("Settings")
                    }
                }
                Item {
                    anchors {
                        left: parent.horizontalCenter
                        top: parent.top
                        bottom: parent.bottom
                        right: parent.right
                    }
                    Switch {
                        id: details
                        anchors.right: parent.right
                        anchors.centerIn: parent
                        text: qsTr("Details")
                    }
                }
            }
        }
    }

    Component.onCompleted: interactive = true

    GeoNameDialog {
        id: geoNameDialog
        width: Math.min(400,parent.width)
        height: parent.height
        edge: Qt.RightEdge
        geoNameBox: geoName.item
        geoLattBox: geoLatt
        geoLontBox: geoLont
        tzBox: timeZoneBox
        opacity: 0.875
    }
}
