
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
    indeterminateCalc: false

    onStartCalc: forecastItemModel.startRecalc()
    onAbortCalc: forecastItemModel.abortRecalc()

    parameters: Row {
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
        anchors.fill: parent
        tableModel: forecastItemModel
    }
    Component.onCompleted: {
        periodBeginDate.setCurrent()
        periodEndDate.setCurrent()
    }
}
