
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
        TableView {
            model: horaModel !== null ? horaModel.planetsModel : null
            flickableDirection: Flickable.VerticalFlick | Flickable.HorizontalFlick
            columnSpacing: 30
            delegate: Loader {
                Component {
                    id: planetSymbol
                    Text {
                        text: symbol
                        font.family: "Symboid"
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                }
                Component {
                    id: planetLont
                    ArcCoordBox {
                        Component.onCompleted: {
                            arcDegree = Qt.binding(function(){return ecl_lont})
                        }
                        sectionCalc: ZodiacSectionCalc {
                        }
                    }
                }
                Component {
                    id: planetLatt
                    ArcCoordBox {
                        Component.onCompleted: {
                            arcDegree = Qt.binding(function(){return ecl_latt})
                        }
                        sectionCalc: SignumSectionCalc {
                        }
                    }
                }
                Component {
                    id: planetSpeed
                    ArcCoordBox {
                        Component.onCompleted: {
                            arcDegree = Qt.binding(function(){return ecl_speed})
                        }
                        sectionCalc: SignumSectionCalc {
                        }
                    }
                }
                sourceComponent: switch(column)
                {
                case 0: return planetSymbol
                case 1: return planetLont
                case 2: return planetLatt
                case 3: return planetSpeed
                default: return null
                }
            }
        }
        Item {

        }
    }
}
