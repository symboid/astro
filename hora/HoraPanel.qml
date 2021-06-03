
import QtQuick 2.12
import QtQuick.Controls 2.5
import Symboid.Sdk.Controls 1.0
import Symboid.Astro.Hora 1.0

CalcPane {
    calcable: hora

    property bool isLandscape: true
    property alias view: horaView
    property bool withSeparator: false

    Rectangle {
        height: isLandscape ? parent.height : 1
        width: isLandscape ? 1 : parent.width
        color: "lightgray"
        visible: withSeparator && minHoraSize !== horaSize
    }

    Rectangle {
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        height: isLandscape ? parent.height : 1
        width: isLandscape ? 1 : parent.width
        color: "lightgray"
        visible: withSeparator && minHoraSize !== horaSize
    }

    readonly property int minHoraSize: Math.min(width,height)
    property int horaSizeDelta: 0
    readonly property int horaSize: minHoraSize / horaView.defaultZoom + horaSizeDelta

    property alias housesType: horaView.housesType

    property alias hora: horaView.hora

    Flickable {
        id: horaFlickable

        anchors.fill: parent

        contentWidth: horaView.width
        contentHeight: horaView.height
        clip: true

        function zoomToMinimum()
        {
            horaSizeDelta = minHoraSize * (horaView.defaultZoom - 1) / horaView.defaultZoom
            contentX = 0
            contentY = 0
        }
        function zoomToDefault()
        {
            horaSizeDelta = 0
            contentX = (horaSize - minHoraSize) / 2
            contentY = contentX
        }
        function zoomTo(zoomPointX,zoomPointY,zoomDelta)
        {
            var zoomRatio = (horaSize + zoomDelta) / horaSize
            if (horaSize + zoomDelta >= minHoraSize)
            {
                horaSizeDelta += zoomDelta
                var newContentX = contentX + (zoomRatio - 1) * zoomPointX
                var newContentY = contentY + (zoomRatio - 1) * zoomPointY
                contentX = newContentX < 0.0 ? 0.0 : newContentX
                contentY = newContentY < 0.0 ? 0.0 : newContentY
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
                horaFlickable.zoomTo(wheel.x, wheel.y, zoomDelta)
            }

            onDoubleClicked: horaFlickable.zoomToDefault()
        }
        PinchArea {
            anchors.fill: parent
            onPinchUpdated: {
                var zoomDelta = (pinch.scale > 1 ? 1 : -1) * horaSize / 30
                horaFlickable.zoomTo(pinch.center.x, pinch.center.y, zoomDelta)
            }
        }

        SingleHoraView {
            id: horaView
            width: horaSize * Math.max(horaFlickable.width / horaFlickable.height, 1.0)
            height: horaSize * Math.max(horaFlickable.height / horaFlickable.width, 1.0)
        }

        Connections {
            target: HoraConfig.fixstars
            function onIncludedChanged() { horaFlickable.zoomToDefault() }
        }

        Component.onCompleted: zoomToDefault()
    }
}
