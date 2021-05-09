
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
    Label {
        anchors.centerIn: parent
        visible: !forecastItemModel.valid && !progressRunning
        text: qsTr("Parameters has changed.\nTable must be recalculated!")
        horizontalAlignment: Label.AlignHCenter
        font.italic: true
        color: "red"
    }
    property bool progressRunning: forecastItemModel.calculating;
    Column {
        id: buttonRow
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        ProgressBar {
            id: calcProgress
            width: forecastTableView.width
            visible: progressRunning
            anchors.horizontalCenter: parent.horizontalCenter
            value: forecastItemModel.progress
        }
        Pane {
            anchors.horizontalCenter: parent.horizontalCenter
            contentItem: Row {
                spacing: parent.padding
                RoundButton {
                    radius: 5
                    icon.source: "/icons/refresh_icon&24.png"
                    enabled: !autoRecalcButton.checked || !forecastItemModel.valid
                    visible: !progressRunning
                    highlighted: !forecastItemModel.valid
                    onClicked: forecastItemModel.startRecalc()
                }
                RoundButton {
                    radius: 5
                    icon.source: "/icons/delete_icon&24.png"
                    visible: progressRunning
                    onClicked: forecastItemModel.abortRecalc()
                }
                RoundButton {
                    id: autoRecalcButton
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
