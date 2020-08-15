
import QtQuick 2.12
import QtQuick.Controls 2.5
import Symboid.Astro.Controls 1.0

Flow {
    property string geoName: ""
    property string country: ""
    property string adminName: ""
    property alias lattArcDegree: latt.arcDegree
    property alias lontArcDegree: lont.arcDegree

    spacing: 10
    padding: 10

    Label {
        text: geoName
        font {
            italic: true
            bold: true
        }
    }
    Label {
        text: "("+country+"/"+adminName+")"
    }
    Row {
        spacing: 10
        ArcCoordLabel {
            id: lont
            sectionCalc: GeoLontSectionCalc {}
        }
        ArcCoordLabel {
            id: latt
            sectionCalc: GeoLattSectionCalc {}
        }
    }
}
