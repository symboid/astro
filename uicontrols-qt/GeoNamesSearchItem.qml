
import QtQuick 2.12
import QtQuick.Controls 2.5

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
        GeoCoordBox {
            id: lont
        }
        GeoCoordBox {
            id: latt
            isLattitude: true
        }
    }
}
