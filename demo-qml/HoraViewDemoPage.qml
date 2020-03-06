
import QtQuick 2.12
import QtQuick.Controls 2.5
import Symboid.Sdk.Controls 1.0
import Symboid.Astro.Controls 1.0

Item {
    Column {
        id: params
        anchors {
            left: parent.left
            top: parent.top
            bottom: parent.bottom
        }
        padding: 20
        spacing: 10

        Text {
            text: "Birth time:"
        }
        DateCoordBox {
            id: dateBox
            editable: true
        }
        TimeCoordBox {
            id: timeBox
            editable: true
            circularLink: dateBox.dayLink
        }
        Item { width: 1; height: 10 }
        Text {
            text: "Geogr. lattitude:"
        }
        ArcCoordBox {
            id: geoLatt
            editable: true
            sectionCalc: GeoLattSectionCalc {}
        }
        Item { width: 1; height: 10 }
        Text {
            text: "Geogr. longitude:"
        }
        ArcCoordBox {
            id: geoLont
            editable: true
            sectionCalc: GeoLontSectionCalc {}
        }
        Button {
            text: "Calculate"
        }
    }
    HoraView {
        anchors {
            top: parent.top
            bottom: parent.bottom
            left: params.right
            right: parent.right
        }
        year: dateBox.year
        month: dateBox.month
        day: dateBox.day
        hour: timeBox.hour
        minute: timeBox.minute
        second: timeBox.second
        geoLatt: geoLatt.arcDegree
        geoLont: geoLont.arcDegree
        tzDiff: 0
    }
}
