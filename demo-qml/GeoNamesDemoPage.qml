
import QtQuick 2.12
import QtQuick.Controls 2.5
import Symboid.Sdk.Network 1.0
import Symboid.Astro.Controls 1.0

Column {

    padding: 20

    Row {
        spacing: 20
        Text {
            anchors.verticalCenter: parent.verticalCenter
            text: "GeoNames API address:"
            font.bold: true
        }
        Text {
            anchors.verticalCenter: parent.verticalCenter
            text: GeoNamesRestClient.apiAddress
        }
    }

    RestObjectModel {
        id: restObjectModel
        restClient: GeoNamesRestClient
        operation: "countrySubdivisionJSON?lat=47.5&lng=19.2&username=symboid"

    }

    RestTableModel {
        id: restTableModel
        restClient: GeoNamesRestClient
//        operation: "countrySubdivisionJSON?lat=47.5&lng=19.2&username=symboid"
        operation: "postalCodeSearchJSON?postalcode=9011&maxRows=10&username=symboid"

        columnNames: ["countryCode", "placeName", "lng", "lat"]

    }

    Rectangle {
        width: 200
        height: 400
        border.width: 1
        ListView {
            anchors.fill: parent
            model: restObjectModel
            delegate: Row {
                property int rowIndex: index
                Text {
                    text: field
                }
                Text {
                    text: ":"
                }

                Text {
                    text: field === "codes" ? "[]" : display
                }
            }
        }
    }
}
