
import QtQuick 2.12
import QtQuick.Controls 2.5
import Symboid.Sdk.Controls 1.0
import Symboid.Sdk.Hosting 1.0
import Symboid.Astro.Controls 1.0
import Symboid.Astro.Hora 1.0

MainScreen {

    readonly property int horzParamSpace: metrics.screenWidth - metrics.screenHeight
    metrics.isTransLandscape: metrics.isLandscape && horzParamSpace - separator.width < 2*metrics.minParamSectionWidth

    Grid {
        id: horaParams
        readonly property bool isRow: metrics.isPortrait || (metrics.isTransLandscape && horzParamSpace >= metrics.minParamSectionWidth)
        columns: isRow ? 2 : 1
        rows: isRow ? 1 : 2
        MainScreenDateTimeBox {
            id: dateTimeBox
            showCurrentTimer: false
            showSeconds: false
            Component.onCompleted: setCurrent()
        }

        MainScreenParamBox {
            title: qsTr("Geographic position")
            ArcCoordBox {
                id: geoLatt
                sectionCalc: GeoLattSectionCalc {}
                editable: true
            }
            ArcCoordBox {
                id: geoLont
                sectionCalc: GeoLontSectionCalc {}
                editable: true
            }
        }
    }

    HoraPanel {
        id: horaPanel
        width: height
//        width:        metrics.isPortrait ? metrics.screenWidth :
//                metrics.isTransLandscape ? metrics.screenWidth - horaParams.width :
//                                           metrics.screenHeight

        height:       metrics.isPortrait ? metrics.screenWidth :
                metrics.isTransLandscape ? metrics.screenHeight - horaParams.height :
                                           metrics.screenHeight
        minHoraSize: width
        horaSize: minHoraSize

        year: dateTimeBox.year
        month: dateTimeBox.month
        day: dateTimeBox.day

        hour: dateTimeBox.hour
        minute: dateTimeBox.minute
        second: 0

        geoLont: geoLont.arcDegree
        geoLatt: geoLatt.arcDegree
        tzDiff: 0

        withJulianCalendar: false
        interactive: true
    }

    VerticalLine {
        id: separator
        height: mainWindow.height
    }

    DemoSettings {
        id: settingsPane
        height: mainWindow.height
        width:        metrics.isPortrait ? metrics.screenWidth :
                metrics.isTransLandscape ? metrics.screenWidth - Math.max(horaParams.width, horaPanel.width) :
                                           horzParamSpace - separator.width - horaParams.width
    }
}
