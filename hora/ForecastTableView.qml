
import QtQuick 2.12
import QtQuick.Controls 2.5
import Symboid.Astro.Controls 1.0
import Symboid.Astro.Hora 1.0

HoraTableView {
    anchors.margins: 20
    property HoraView horaView: null
    tableModel: ForecastModel {
        hora: horaView !== null ? horaView.hora : null
    }
    headerModel: tableModel.headerModel
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