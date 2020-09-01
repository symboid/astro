
import QtQuick 2.12
import QtQuick.Controls 2.5
import Symboid.Astro.Hora 1.0

Flickable {

    property int minHoraSize: 100
    property int horaSize: minHoraSize

    property alias year: horaView.year
    property alias month: horaView.month
    property alias day: horaView.day
    property alias hour: horaView.hour
    property alias minute: horaView.minute
    property alias second: horaView.second

    property alias geoLatt: horaView.geoLatt
    property alias geoLont: horaView.geoLont
    property alias tzDiff: horaView.tzDiff

    property alias housesType: horaView.housesType
    property alias withJulianCalendar: horaView.withJulianCalendar

    property alias planetsModel: horaView.planetsModel
    property alias housesModel: horaView.housesModel
    property alias interactive: horaView.interactive

    contentWidth: horaView.width
    contentHeight: horaView.height
    clip: true

    function zoomToMinimum()
    {
        horaSize = minHoraSize
        contentX = 0
        contentY = 0
    }
    function zoomToDefault()
    {
        horaSize = minHoraSize / horaView.defaultZoom
        contentX = (horaSize - minHoraSize) / 2
        contentY = contentX
    }
    function zoomTo(zoomPointX,zoomPointY,zoomDelta)
    {
        var zoomRatio = (horaSize + zoomDelta) / horaSize
        if (horaSize + zoomDelta >= minHoraSize)
        {
            horaSize += zoomDelta
            var newContentX = contentX + (zoomRatio - 1) * zoomPointX
            var newContentY = contentY + (zoomRatio - 1) * zoomPointY
            contentX = newContentX < 0.0 ? 0.0 : newContentX
            contentY = newContentY < 0.0 ? 0.0 : newContentY
//                            console.log("contentCorner = ("+contentX+","+contentY+"), zoom=("+zoomDelta+","+zoomRatio+")")
        }
        else
        {
            zoomToMinimum()
        }
    }

    onWidthChanged:  zoomToDefault()
    onHeightChanged: zoomToDefault()

    MouseArea {
        anchors.fill: parent
        onWheel: {
            var zoomDelta = (wheel.angleDelta.y/500.0) * horaSize
            zoomTo(wheel.x, wheel.y, zoomDelta)
        }
        onClicked: {
//                            console.log("contentCorner = ("+contentX+","+contentY+")")
        }

        onDoubleClicked: zoomToDefault()
    }
    PinchArea {
        anchors.fill: parent
        onPinchUpdated: {
            var zoomDelta = (pinch.scale > 1 ? 1 : -1) * horaSize / 30
            zoomTo(pinch.center.x, pinch.center.y, zoomDelta)
        }
    }

    HoraView {
        id: horaView
        width: horaSize
        height: horaSize

        displayFlags: HoraView.SHOW_FIXSTARS
        fontPointSize: mainWindow.font.pointSize

        BusyIndicator {
            id: horaCalcIndicator
            width: 100
            height: 100
            anchors.centerIn: parent
            running: false
        }
        onStartCalc: horaCalcIndicator.running = true
        onStopCalc: horaCalcIndicator.running = false
    }
}