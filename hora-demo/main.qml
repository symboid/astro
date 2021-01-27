
import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Window 2.12
import Symboid.Sdk.Controls 1.0
import Symboid.Astro.Controls 1.0
import Symboid.Astro.Hora 1.0

ApplicationWindow {
    id: mainWindow
    visible: true
    visibility: Window.Maximized
    width: 900
    height: 600

    readonly property QtObject metrics: QtObject {
        readonly property int paramSectionWidth: 300
    }

    Column {
        Row {
            id: horaParams
            anchors.horizontalCenter: parent.horizontalCenter
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
            anchors.horizontalCenter: parent.horizontalCenter
            width: height
            height: mainWindow.height-horaParams.height
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
    }
}
