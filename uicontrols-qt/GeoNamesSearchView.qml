
import QtQuick 2.12
import QtQuick.Controls 2.5
import Symboid.Astro.Controls 1.0
import Symboid.Sdk.Network 1.0

ListView {
    model: restTableModel

    property string geoName: ""
    property int maxRows: 10
    readonly property string apiUser: "symboid"

    RestTableModel {
        id: restTableModel
        restClient: GeoNamesRestClient
        operation: "searchJSON?q="+geoName+
                   "&lang="+Qt.locale()+
                   "&maxRows="+maxRows+
                   "&username="+apiUser
        columnNames: ["name", "countryName", "adminName1", "population", "lng", "lat"]
    }

    delegate: Flow {
        width: parent.width
        spacing: 10
        padding: 10

        Label {
            height: coordsBox.height
            verticalAlignment: Label.AlignVCenter
            text: name
        }
        Label {
            height: coordsBox.height
            verticalAlignment: Label.AlignVCenter
            text: "("+countryName+"/"+adminName1+")"
        }
        Row {
            id: coordsBox
            spacing: 10
            GeoCoordBox {
                value: lng
            }
            GeoCoordBox {
                isLattitude: true
                value: lat
            }
        }
    }
}
