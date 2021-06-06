
import QtQuick 2.12
import QtQuick.Controls 2.5
import Symboid.Astro.Controls 1.0

HoraTableView {
    anchors.margins: 20
    headerModel: tableModel !== null ? tableModel.headerModel : null
    property bool showSeconds: false
    columnComponents: [
        Component {
            Pane {
                Label {
                    text: cellData
                    font.family: "Symboid"
                    width: 40
                }
            }
        },
        Component {
            Pane {
                ArcCoordLabel {
                    arcDegree: cellData
                    sectionCalc: ZodiacSectionCalc {}
                    sectionFont.family: "Symboid"
                    showSecond: showSeconds
                }
            }
        },
        Component {
            Pane {
                ArcCoordLabel {
                    arcDegree: cellData
                    sectionCalc: SignumSectionCalc {}
                    showSecond: showSeconds
                }
            }
        },
        Component {
            Pane {
                ArcCoordLabel {
                    arcDegree: cellData
                    sectionCalc: SignumSectionCalc {}
                    showSecond: showSeconds
                }
            }
        }
    ]
}
