
import QtQuick 2.12
import QtQuick.Controls 2.5
import Symboid.Sdk.Controls 1.0
import Symboid.Astro.Controls 1.0
import QtPositioning 5.12
import QtQuick.Controls.Material 2.3

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
            TextField {
                id: radixName
                width: parent.defaultItemWidth
                leftPadding: 10
                rightPadding: leftPadding
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
        }

        HoraScreenParams {
            title: qsTr("Calendar")
            visible: details.checked
            ComboBox {
                id: calendarType
                width: parent.defaultItemWidth
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
            tzDiff: tzDiff.hour
            housesType: housesType.currentToken()
            withJulianCalendar: calendarType.currentIndex !== 0
        }

        HoraScreenParams {
            id: locationParams
            title: qsTr("Location")

            Row {
                padding: geoLatt.padding
                spacing: 10
                enabled: !currentLocTimer.checked
                TextField {
                    id: geoName
                    width: locationParams.defaultItemWidth - geoNameDialogButton.width - parent.spacing - 2*parent.padding
                    anchors.verticalCenter: parent.verticalCenter
                    onTextChanged: {
                        console.log("geoname changed")
                    }
                }
                RoundButton {
                    id: geoNameDialogButton
                    display: RoundButton.IconOnly
                    padding: 0
                    icon.source: "/icons/globe_3_icon&48.png"
                    icon.color: "darkblue"
                    icon.width: width
                    icon.height: height
                    anchors.verticalCenter: parent.verticalCenter
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

            Row {
                visible: details.checked
                enabled: !currentLocTimer.checked
                spacing: 10
                Label {
                    anchors.verticalCenter: parent.verticalCenter
                    text: qsTr("Time zone diff:")
                }
                MultiNumberBox {
                    id: tzDiff
                    editable: true
                    property int hour: box(0).value
                    boxes: Row {
                        NumberBox {
                            from: -12
                            to: 12
                            displaySuffix: qsTr("h")
                            circular: true
                        }
                    }
                }
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
            title: qsTr("House system")
            visible: details.checked

            ComboBox {
                id: housesType
                width: parent.defaultItemWidth
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
//                color: "#95B2A0"
                color: "transparent"
                border.color: "black"
                border.width: 1
                radius: 10
                readonly property int buttonPadding: 5
                /*RoundButton {
                    id: geoNameDialogButton
                    anchors.margins: parent.buttonPadding
                    height: parent.height - 2*parent.buttonPadding
                    width: height
                    icon.source: "/icons/globe_3_icon&48.png"
//                    icon.width: 48
//                    icon.height: 48
                    icon.color: "darkblue"
                    anchors.left: parent.left
                    anchors.verticalCenter: parent.verticalCenter
                    onClicked: geoNameDialog.open()
                }*/
                Switch {
                    id: details
                    anchors.margins: parent.buttonPadding
                    anchors {
                        right: parent.right
                        verticalCenter: parent.verticalCenter
                    }
                    text: qsTr("Details")
                }

                Rectangle {
                    anchors.left: parent.left//geoNameDialogButton.right
                    anchors.leftMargin: parent.buttonPadding
                    anchors.right: details.left
                    anchors.rightMargin: parent.buttonPadding
                    height: parent.height
                    color: parent.color
                    border.color: parent.border.color
                    border.width: 0//parent.border.width
                    RoundButton {
                        anchors.fill: parent
                        anchors.margins: parent.parent.buttonPadding
                        radius: 0
                        text: qsTr("Tabular")
                        onClicked: horaTableDialog.open()
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
        geoNameBox: geoName
        geoLattBox: geoLatt
        geoLontBox: geoLont
        Material.background: "#DFEEE5"
        opacity: 0.875
    }

    Dialog {
        id: horaTableDialog
        title: qsTr("Horoscope items")
        standardButtons: Dialog.Close
        anchors.centerIn: parent

        HoraTablesLayout {
            anchors.fill: parent
            horaModel: horaTableDialog.opened ? horaView : null
        }

        height: parent.height - 50
        width: Math.min(parent.width - 50, 700)
    }
}
