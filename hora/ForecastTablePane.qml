
import QtQuick 2.12
import QtQuick.Controls 2.5
import Symboid.Sdk.Controls 1.0
import Symboid.Astro.Hora 1.0

CalcPane {

    property alias periodBegin: forecastItemModel.periodBegin
    property alias periodEnd: forecastItemModel.periodEnd
    property alias hora: forecastItemModel.hora
    property alias forecastModel: forecastItemModel.forecastModel

    calcable: forecastItemModel.calcable

    indeterminateCalc: false

    ForecastItemModel {
        id: forecastItemModel
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
