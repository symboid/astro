
import QtQuick 2.12
import QtQuick.Controls 2.5
import Symboid.Sdk.Hosting 1.0
import Symboid.Astro.Hora 1.0

SettingsGroupFixed {
    title: qsTr("Horoscope")
    property var aspectTitles: [
        qsTr("Conjunction"), qsTr("Opposition"), qsTr("Trigon"), qsTr("Quadrat"),
        qsTr("Quintile"), qsTr("Sextile"), qsTr("Semi-sextile"), qsTr("Quincunx"),
        qsTr("Semi-quadrat"), qsTr("Sesqui-quadrat"), qsTr("Biquintile")
    ]
    SettingsGroupLink {
        title: qsTr("Aspect connections")
        settingsPane: SettingsPane {
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
    SettingsGroupExpanding {
        title: qsTr("Fixed stars")

        SettingsCheckBox {
            configNode: HoraConfig.fixstars.enabled_node
            text: qsTr("Include fixed stars")
            hint: qsTr("Fixed stars in conjuction with planets will be denoted.")
        }
    }
    SettingsGroupLink {
        title: qsTr("Orbis")

        settingsPane: SettingsPane {
            title: qsTr("Orbis")
            SettingsGroupLink {
                title: qsTr("Before house cusps")
                settingsPane: SettingsPane {
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
    SettingsGroupExpanding {
        title: qsTr("Other horoscope elements")
        SettingsSwitch {
            text: qsTr("Dragon Head")
            configNode: HoraConfig.karma.dragon_head_node
        }
        SettingsSwitch {
            text: qsTr("Dragon Tail")
            configNode: HoraConfig.karma.dragon_tail_node
        }
        SettingsSwitch {
            text: qsTr("Lilith")
            configNode: HoraConfig.karma.lilith_node
        }
        SettingsSwitch {
            text: qsTr("Axis on lunar nodes")
            configNode: HoraConfig.karma.draw_axis_node
            enabled: false
        }
    }
}
