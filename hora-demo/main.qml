
import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Window 2.12
import Symboid.Astro.Hora 1.0

ApplicationWindow {

    visible: true
    visibility: Window.Maximized
    width: 900
    height: 600

    HoraPanel {
        id: horaPanel
        width: parent.height
        height: width
        minHoraSize: Math.min(parent.width,parent.height)
        horaSize: minHoraSize

        year: 1977
        month: 9
        day: 10

        hour: 12
        minute: 47
        second: 0

        geoLont: 20.1
        geoLatt: 47.1
        tzDiff: 1

        withJulianCalendar: false
        interactive: true
    }
//    Component.onCompleted: horaPanel.interactive = true
}
