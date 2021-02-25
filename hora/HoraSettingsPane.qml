
import QtQuick 2.12
import QtQuick.Controls 2.5
import Symboid.Sdk.Controls 1.0
import Symboid.Sdk.Hosting 1.0
import Symboid.Astro.Hora 1.0

FolderGroupFixed {
    title: qsTr("Horoscope")
    property var aspectTitles: [
        qsTr("Conjunction"), qsTr("Opposition"), qsTr("Trigon"), qsTr("Quadrat"),
        qsTr("Quintile"), qsTr("Sextile"), qsTr("Semi-sextile"), qsTr("Quincunx"),
        qsTr("Semi-quadrat"), qsTr("Sesqui-quadrat"), qsTr("Biquintile")
    ]
    FolderGroupLink {
        title: qsTr("Aspect connections")
        folderPane: FolderPane {
            title: qsTr("Aspect connections")
            Repeater {
                model: HoraConfig.aspects
                AspectSettingsGroup {
                    text: aspectTitles[index]
                    aspectConfig: config_item
                    withSeparator: index == 5
                }
            }
        }
    }
    FolderGroupExpanding {
        title: qsTr("Fixed stars")

        SettingsCheckBox {
            configNode: HoraConfig.fixstars.included_node
            text: qsTr("Include fixed stars")
            hint: qsTr("Fixed stars in conjuction with planets will be denoted.")
        }
    }
    FolderGroupLink {
        title: qsTr("Orbis")
visible:false
        folderPane: FolderPane {
            title: qsTr("Orbis")
            FolderGroupLink {
                title: qsTr("Before house cusps")
                folderPane: FolderPane {
                    title: qsTr("Before house cusps")
                    Repeater {
                        model: HoraConfig.house_cusp_orbis
                        OrbisSettingsItem {
                            property var elementNames: [ qsTr("Asc"), qsTr("Dsc"), qsTr("MC"), qsTr("IC"), qsTr("Other") ]
                            text: elementNames[index]
                            orbisConfig: config_item
                        }
                    }
                }
            }
        }
    }
    FolderGroupExpanding {
        title: qsTr("Other horoscope elements")
        SettingsSwitch {
            title: qsTr("Dragon Head")
            configNode: HoraConfig.karma.dragon_head_node
        }
        SettingsSwitch {
            title: qsTr("Dragon Tail")
            configNode: HoraConfig.karma.dragon_tail_node
        }
        SettingsSwitch {
            title: qsTr("Lilith")
            configNode: HoraConfig.karma.lilith_node
        }
        SettingsSwitch {
            title: qsTr("Axis on lunar nodes")
            configNode: HoraConfig.karma.draw_axis_node
            enabled: false
        }
    }
}
