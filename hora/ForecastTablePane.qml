
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
    signal startCalc
    signal stopCalc

    ForecastItemModel {
        id: forecastItemModel
        autoRecalc: autoRecalcButton.checked
        onModelAboutToBeReset: startCalc()
        onModelReset: stopCalc()
    }
    ForecastTableView {
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: buttonRow.top
        tableModel: forecastItemModel
        opacity: forecastItemModel.valid ? 1.0 : 0.5
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
}
