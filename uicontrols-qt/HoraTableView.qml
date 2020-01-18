
import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3
import Symboid.Astro.Controls 1.0

Item {

    property HoraView horaModel: null

    TabBar {
        id: tabBar
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        TabButton {
            text: qsTr("Planets")
        }
        TabButton {
            text: qsTr("Houses")
        }
    }

    StackLayout {
        anchors.top: tabBar.bottom
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        currentIndex: tabBar.currentIndex
        ListView {
            model: horaModel !== null ? horaModel.planetsModel : null
            flickableDirection: Flickable.VerticalFlick | Flickable.HorizontalFlick
            delegate: Row {
                spacing: 10
                Text {
                    text: symbol
                    font.family: "Symboid"
                    anchors.verticalCenter: parent.verticalCenter
                    horizontalAlignment: Text.AlignHCenter
                }
                ArcCoordBox {
                    Component.onCompleted: {
                        arcDegree = Qt.binding(function(){return ecl_lont})
                    }
                    sectionCalc: ZodiacSectionCalc {
                    }
                }
                ArcCoordBox {
                    Component.onCompleted: {
                        arcDegree = Qt.binding(function(){return ecl_latt})
                    }
                    sectionCalc: SignumSectionCalc {
                    }
                }
                ArcCoordBox {
                    Component.onCompleted: {
                        arcDegree = Qt.binding(function(){return ecl_speed})
                    }
                    sectionCalc: SignumSectionCalc {
                    }
                }
            }
        }
        Item {

        }
    }
}
