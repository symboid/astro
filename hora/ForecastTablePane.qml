
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

    Grid {
        id: periodRow
        rows: 1
        columns: 2
        anchors.top: parent.top
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
        onModelAboutToBeReset: calcIndicator.running = true
        onModelReset: calcIndicator.running = false
    }
    ForecastTableView {
        anchors.top: periodRow.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: buttonRow.top
        tableModel: forecastItemModel
        opacity: forecastItemModel.valid && !calcIndicator.running ? 1.0 : 0.5
    }
    BusyIndicator {
        id: calcIndicator
        anchors.centerIn: parent
        running: false
    }

    Label {
        anchors.centerIn: parent
        visible: !forecastItemModel.valid
        text: qsTr("Parameters has changed.\nTable must be recalculated!")
        horizontalAlignment: Label.AlignHCenter
        font.italic: true
        color: "red"
    }
    Pane {
        id: buttonRow
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        contentItem: Row {
            spacing: 10
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
    Component.onCompleted: {
        periodBeginDate.setCurrent()
        periodEndDate.setCurrent()
    }
}
