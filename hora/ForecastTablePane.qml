
import QtQuick 2.12
import QtQuick.Controls 2.5
import Symboid.Sdk.Controls 1.0
import Symboid.Astro.Hora 1.0

CalcPane {

    property alias periodBegin: forecastItemModel.periodBegin
    property alias periodEnd: forecastItemModel.periodEnd
    property alias hora: forecastItemModel.hora
    property alias forecastModel: forecastItemModel.forecastModel

    calculating: forecastItemModel.calculating
    progressPos: forecastItemModel.progress
    contentValid: forecastItemModel.valid

    onStartCalc: forecastItemModel.startRecalc()
    onAbortCalc: forecastItemModel.abortRecalc()

    Row {
        id: periodRow
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
        autoRecalc: autocalc
    }

    ForecastTableView {
        id: forecastTableView
        anchors.top: periodRow.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        tableModel: forecastItemModel
        opacity: forecastItemModel.valid ? 1.0 : 0.5
    }
    Label {
        anchors.centerIn: parent
        visible: !forecastItemModel.valid && !calculating
        text: qsTr("Parameters has changed.\nTable must be recalculated!")
        horizontalAlignment: Label.AlignHCenter
        font.italic: true
        color: "red"
    }
    Component.onCompleted: {
        periodBeginDate.setCurrent()
        periodEndDate.setCurrent()
    }
}
