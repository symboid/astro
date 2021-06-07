
import QtQuick 2.12
import QtQuick.Controls 2.5
import Symboid.Astro.Controls 1.0

Item {
    property bool showSeconds: false
    property var headerModel: null
    property var tableModel: null

}

/*
HoraTableView {
    anchors.margins: 20
    headerModel: tableModel !== null ? tableModel.headerModel : null
    property bool showSeconds: false
    columnComponents: [
        Component {
            Pane {
                Label {
                    text: cellData
                    anchors.horizontalCenter: parent.horizontalCenter
                }
            }
        },
        Component {
            ArcCoordLabel {
                arcDegree: cellData
                sectionCalc: ZodiacSectionCalc {}
                sectionFont.family: "Symboid"
                showSecond: showSeconds
            }
        },
        Component {
            ArcCoordLabel {
                arcDegree: cellData
                sectionCalc: SignumSectionCalc {}
                showSecond: showSeconds
            }
        }
    ]
}
*/
