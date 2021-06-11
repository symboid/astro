
import QtQuick 2.12
import QtQuick.Controls 2.5
import Symboid.Astro.Controls 1.0

HoraTableView {
    property bool showSeconds: false
    columns: [
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
