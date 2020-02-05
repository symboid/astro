
import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3
import Symboid.Astro.Controls 1.0
import QtQuick.Controls.Material 2.3

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

        HoraTableView {
            headerModel: tableModel !== null ? tableModel.headerModel : null
            tableModel: horaModel !== null ? horaModel.planetsModel : null
            columnComponents: [
                Component {
                    Text {
                        text: cellData
                        font.family: "Symboid"
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                },
                Component {
                    ArcCoordBox {
                        Component.onCompleted: {
                            arcDegree = Qt.binding(function(){return cellData})
                        }
                        sectionCalc: ZodiacSectionCalc {
                        }
                    }
                },
                Component {
                    ArcCoordBox {
                        Component.onCompleted: {
                            arcDegree = Qt.binding(function(){return cellData})
                        }
                        sectionCalc: SignumSectionCalc {
                        }
                    }
                },
                Component {
                    ArcCoordBox {
                        Component.onCompleted: {
                            arcDegree = Qt.binding(function(){return cellData})
                        }
                        sectionCalc: SignumSectionCalc {
                        }
                    }
                }
            ]
        }
        HoraTableView {
            headerModel: tableModel !== null ? tableModel.headerModel : null
            tableModel: horaModel !== null ? horaModel.housesModel : null
            columnComponents: [
                Component {
                    Label {
                        text: cellData
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                },
                Component {
                    ArcCoordBox {
                        Component.onCompleted: {
                            arcDegree = Qt.binding(function(){return cellData})
                        }
                        sectionCalc: ZodiacSectionCalc {
                        }
                    }
                },
                Component {
                    ArcCoordBox {
                        Component.onCompleted: {
                            arcDegree = Qt.binding(function(){return cellData})
                        }
                        sectionCalc: SignumSectionCalc {
                        }
                    }
                }
            ]
        }
    }
}
