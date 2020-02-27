
import QtQuick 2.12
import QtQuick.Controls 2.5
import Symboid.Sdk.Controls 1.0
import Symboid.Astro.Controls 1.0
import QtPositioning 5.12

Flickable {

    readonly property bool isLandscape: width > height

    readonly property int mandalaSize: isLandscape ? height : width
    readonly property int screenSize: isLandscape ? width : height
    readonly property int restSize: screenSize - mandalaSize

    property alias fontPointSize: horaView.fontPointSize

    flickableDirection: isLandscape ? Flickable.HorizontalFlick : Flickable.VerticalFlick
    contentWidth: horaScreen.width
    contentHeight: horaScreen.height

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

        HoraScreenParams {
            title: qsTr("Horoscope name")
            HoraScreenTextField {
                id: horaName
            }
        }

        HoraScreenParams {
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

        HoraScreenParams {
            title: qsTr("Calendar")
            visible: details.checked
            HoraScreenComboBox {
                id: calendarType
                model: [ "Gregorian", "Julian" ]
            }
        }

        HoraView {
            id: horaView
            width: mandalaSize
            height: mandalaSize

            year: dateBox.year
            month: dateBox.month
            day: dateBox.day
            hour: timeBox.hour
            minute: timeBox.minute
            second: timeBox.second
            geoLatt: geoLatt.arcDegree
            geoLont: geoLont.arcDegree
            tzDiff: timeZoneBox.hour
            housesType: housesType.currentToken()
            withJulianCalendar: calendarType.currentIndex !== 0
        }

        HoraScreenParams {
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
            }
            HoraScreenTimer {
                id: currentLocTimer
                text: qsTr("Current")
                visible: details.checked
                onTriggered: geoNameDialog.setCurrent()
                enabled: geoNameDialog.currentIsValid
            }
        }
        HoraScreenParams {
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

        Item {
            id: referencePanel
            width: isLandscape ? parent.paramSectionWidth : mandalaSize
            readonly property int contentHeight: 80
            height: isLandscape ? contentHeight + 2 * parent.paramSectionPadding : 0
        }

        Pane {
            id: controlPanel
            width: referencePanel.width
            padding: parent.paramSectionPadding
            topPadding: isLandscape ? mandalaSize - (referencePanel.y + referencePanel.height) - referencePanel.contentHeight
                                    : parent.paramSectionPadding
            leftPadding: isLandscape ? parent.paramSectionPadding
                                     : parent.paramSectionPadding + mandalaSize - parent.paramSectionWidth
            height: referencePanel.contentHeight + topPadding

            Rectangle {
                anchors.fill: parent
                color: "transparent"
                border.width: 1
                radius: 10
                Item {
                    anchors {
                        left: parent.left
                        top: parent.top
                        bottom: parent.bottom
                        right: parent.horizontalCenter
                    }
                    Button {
                        anchors.left: parent.left
                        anchors.centerIn: parent
                        icon.source: "/icons/3x3_grid_icon&32.png"
                        text: qsTr("Tabulars")
                        onClicked: horaTableDialog.open()
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
        currentUnixTime: unixTimeConverter.unixTime
    }

    Dialog {
        id: horaTableDialog
        title: qsTr("Horoscope items")
        standardButtons: Dialog.Close
        anchors.centerIn: parent

        HoraTablesLayout {
            anchors.fill: parent
            horaModel: horaView
        }

        height: parent.height - 50
        width: Math.min(parent.width - 50, 700)
    }
}
