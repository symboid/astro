
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
                editable: true
            }
            TimeCoordBox {
                id: timeBox
                editable: true
                circularLink: dateBox.dayLink
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

            Switch {
                id: details
                anchors {
                    bottom: parent.bottom
                    right: parent.right
                }
                text: qsTr("Details")
            }

            RoundButton {
                anchors.left: parent.left
                anchors.bottom: parent.bottom
                text: qsTr("Tabular")
                onClicked: horaTableDialog.open()
            }

            Dialog {
                id: horaTableDialog
                title: qsTr("Horoscope items")
                standardButtons: Dialog.Close
                anchors.centerIn: parent

                HoraTableView {
                    anchors.fill: parent
                    horaModel: horaTableDialog.opened ? horaView : null
                }

                height: parent.height - 50
                width: parent.width - 50
            }
        }

        HoraScreenParams {
            id: locationParams
            title: qsTr("Location")

            TextField {
                id: geoName
                width: parent.defaultItemWidth

                RoundButton {
                    height: geoLatt.height * 1.33
                    width: height
                    icon.source: "/icons/globe_3_icon&48.png"
                    icon.width: 48
                    icon.height: 48
                    icon.color: "darkblue"
                    anchors.right: parent.right
                    anchors.bottom: parent.top
                    anchors.bottomMargin: -20
                    onClicked: geoNameDialog.open()
                }
            }
            ArcCoordBox {
                id: geoLatt
                visible: details.checked
                sectionCalc: GeoLattSectionCalc {}
                editable: true
            }
            ArcCoordBox {
                id: geoLont
                visible: details.checked
                sectionCalc: GeoLontSectionCalc {}
                editable: true
            }

            Row {
                visible: details.checked
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
    }

    GeoNameDialog {
        id: geoNameDialog
        width: Math.min(400,parent.width)
        height: parent.height
        edge: Qt.RightEdge
        geoNameBox: geoName
        geoLattBox: geoLatt
        geoLontBox: geoLont
    }
}
