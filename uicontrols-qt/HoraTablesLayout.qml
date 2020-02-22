
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

        Item {
            HoraTableView {
                id: planetTableView
                anchors {
                    top: parent.top
                    bottom: showSeconds.top
                    left: parent.left
                    right: parent.right
                }
                headerModel: tableModel !== null ? tableModel.headerModel : null
                tableModel: horaModel !== null ? horaModel.planetsModel : null
                columnComponents: [
                    Component {
                        Pane {
                            Label {
                                text: cellData
                                font.family: "Symboid"
                                anchors.horizontalCenter: parent.horizontalCenter
                            }
                        }
                    },
                    Component {
                        ArcCoordLabel {
                            arcDegree: cellData
                            sectionCalc: ZodiacSectionCalc {}
                            sectionFont.family: "Symboid"
                            showSecond: showSeconds.checked
                        }
                    },
                    Component {
                        ArcCoordLabel {
                            arcDegree: cellData
                            sectionCalc: SignumSectionCalc {}
                            showSecond: showSeconds.checked
                        }
                    },
                    Component {
                        ArcCoordLabel {
                            arcDegree: cellData
                            sectionCalc: SignumSectionCalc {}
                            showSecond: showSeconds.checked
                        }
                    }
                ]
            }
            CheckBox {
                id: showSeconds
                anchors {
                    horizontalCenter: parent.horizontalCenter
                    bottom: parent.bottom
                }
                text: qsTr("Show seconds")
            }
        }
        HoraTableView {
            headerModel: tableModel !== null ? tableModel.headerModel : null
            tableModel: horaModel !== null ? horaModel.housesModel : null
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
                    }
                },
                Component {
                    ArcCoordLabel {
                        arcDegree: cellData
                        sectionCalc: SignumSectionCalc {}
                    }
                }
            ]
        }
    }
}
