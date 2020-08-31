
import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Window 2.5
import Symboid.Sdk.Controls 1.0
import Symboid.Astro.Controls 1.0
import Symboid.Astro.Db 1.0

MainScreenParamBox {

    property alias geoName: geoName.text
    property alias geoLatt: geoLatt.arcDegree
    property alias geoLont: geoLont.arcDegree
    property alias geoTzDiff: timeZoneBox.diffHours
    property bool showDetails: false
    property alias lockedToCurrent: currentLocTimer.checked
    property alias showCurrentTimer: currentLocTimer.visible

    title: qsTr("Location")

    MainScreenTextField {
        id: geoName
        enabled: !lockedToCurrent
        button: RoundButton {
            display: RoundButton.IconOnly
            padding: 0
            icon.source: "/icons/globe_3_icon&48.png"
            icon.color: "darkblue"
            icon.width: width - leftInset - rightInset
            icon.height: height - topInset - bottomInset
            onClicked: geoNameDialog.open()
        }
    }
    ArcCoordBox {
        id: geoLatt
        visible: showDetails
        sectionCalc: GeoLattSectionCalc {}
        editable: true
        enabled: !lockedToCurrent
    }
    ArcCoordBox {
        id: geoLont
        visible: showDetails
        sectionCalc: GeoLontSectionCalc {}
        editable: true
        enabled: !lockedToCurrent
    }
    GeoTimeZoneBox {
        id: timeZoneBox
        visible: showDetails
        enabled: !lockedToCurrent
        geoNameLatt: geoLatt.arcDegree
        geoNameLont: geoLont.arcDegree
        currentUnixTime: dateTimeParams.unixTime
    }
    MainScreenTimer {
        id: currentLocTimer
        text: qsTr("Current")
        visible: showDetails
        onTriggered: geoNameDialog.setCurrent()
        enabled: geoNameDialog.currentIsValid
    }
    GeoNameDialog {
        id: geoNameDialog
        width: Math.min(400,metrics.screenWidth)
        height: Screen.height
        edge: Qt.RightEdge
        geoNameBox: geoName.item
        geoLattBox: geoLatt
        geoLontBox: geoLont
        opacity: 0.875
        onGeoNameChanged: timeZoneBox.search()
    }
}
