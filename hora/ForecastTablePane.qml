
import QtQuick 2.12
import QtQuick.Controls 2.5
import Symboid.Sdk.Controls 1.0
import Symboid.Astro.Hora 1.0

Item {
    property alias periodBegin: forecastItemModel.periodBegin
    property alias periodEnd: forecastItemModel.periodEnd
    property alias hora: forecastItemModel.hora
    property alias forecastModel: forecastItemModel.forecastModel
    property alias autoRecalc: autoRecalcButton.checked

    Row {
        id: periodRow
        anchors.horizontalCenter: parent.horizontalCenter
        DateCoordBox {
            id: periodBeginDate
            editable: true
        }
        DateCoordBox {
            id: periodEndDate
            editable: true
        }
    }

    ForecastItemModel {
        id: forecastItemModel
        periodBegin: HoraCoords {
            year: periodBeginDate.year
            month: periodBeginDate.month
            day: periodBeginDate.day
        }
        periodEnd: HoraCoords {
            year: periodEndDate.year
            month: periodEndDate.month
            day: periodEndDate.day
        }
        autoRecalc: autoRecalcButton.checked
        onModelAboutToBeReset: {
            progressPopup.visible = true
        }
        onModelReset: {
            progressPopup.visible = false
        }
    }
    ForecastTableView {
        id: forecastTableView
        anchors.top: periodRow.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: buttonRow.top
        tableModel: forecastItemModel
        opacity: forecastItemModel.valid ? 1.0 : 0.5
    }
    /*
    Pane {
        id: progressPopup
        anchors.centerIn: parent
        contentItem: Frame {
            contentItem: Pane {
                contentItem: Column {
                    spacing: progressPopup.padding
                    ProgressBar {
                        id: calcProgress
                        width: 300
                        anchors.horizontalCenter: parent.horizontalCenter
                        value: 0.0
                    }
                    RoundButton {
                        anchors.horizontalCenter: parent.horizontalCenter
                        icon.source: "file:/Users/robert/Munka/icons/black/png/delete_icon&24.png"
                    }
                }
            }
        }
    }
    */

    Label {
        anchors.centerIn: parent
        visible: !forecastItemModel.valid && !progressPopup.visible
        text: qsTr("Parameters has changed.\nTable must be recalculated!")
        horizontalAlignment: Label.AlignHCenter
        font.italic: true
        color: "red"
    }
    Pane {
        id: buttonRow
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        contentItem: Column {
            Row {
                id: progressPopup
                anchors.horizontalCenter: parent.horizontalCenter
                spacing: buttonRow.padding
                visible: false
                ProgressBar {
                    id: calcProgress
                    width: forecastTableView.width - cancelCalc.width - 2 * progressPopup.padding
                    anchors.verticalCenter: parent.verticalCenter
                    value: forecastItemModel.progress
                }
                RoundButton {
                    id: cancelCalc
                    anchors.verticalCenter: parent.verticalCenter
                    icon.source: "file:/Users/robert/Munka/icons/black/png/delete_icon&24.png"
                }
            }
            Row {
                anchors.horizontalCenter: parent.horizontalCenter
                spacing: buttonRow.padding
                RoundButton {
                    radius: 5
                    icon.source: "/icons/refresh_icon&24.png"
                    anchors.verticalCenter: parent.verticalCenter
                    enabled: !autoRecalcButton.checked
                    highlighted: !forecastItemModel.valid
                    onClicked: forecastItemModel.recalc()
                }
                RoundButton {
                    id: autoRecalcButton
                    anchors.verticalCenter: parent.verticalCenter
                    checkable: true
                    radius: 5
                    display: RoundButton.IconOnly
                    icon.source: checked ? "/icons/connect_icon&24.png" : "/icons/not_connected_icon&24.png"
                }
            }
        }
    }
    Component.onCompleted: {
        periodBeginDate.setCurrent()
        periodEndDate.setCurrent()
    }
}
