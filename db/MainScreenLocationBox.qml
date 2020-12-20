
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

    title: qsTr("Location")

    MainScreenTextField {
        id: geoName
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
    }
    ArcCoordBox {
        id: geoLont
        visible: showDetails
        sectionCalc: GeoLontSectionCalc {}
        editable: true
    }
    GeoTimeZoneBox {
        id: timeZoneBox
        visible: showDetails
        geoNameLatt: geoLatt.arcDegree
        geoNameLont: geoLont.arcDegree
        currentUnixTime: dateTimeParams.unixTime
        onQueryFinished: busyPopup.close()
        onQueryAborted: {
            busyPopup.close()
            errorPopup.show(qsTr("Getting time zone information has been failed!"))
        }
    }
    GeoNameDialog {
        id: geoNameDialog
        width: Math.min(400,metrics.screenWidth)
        height: metrics.screenHeight
        edge: Qt.RightEdge
        geoNameBox: geoName.item
        geoLattBox: geoLatt
        geoLontBox: geoLont
        opacity: 0.875
        onGeoNameChanged: {
            busyPopup.show("Getting time zone information...")
            timeZoneBox.search()
        }
    }
}
