
import QtQuick 2.12
import QtQuick.Controls 2.5
import Symboid.Sdk.Hosting 1.0
import Symboid.Astro.Hora 1.0

SettingsGroup {
    title: qsTr("Horoscope")
    SettingsGroupLink {
        title: qsTr("Planets")
        settingsPane: SettingsPane {
            title: qsTr("Planets")
            SettingsItem {
                setting: CheckBox {
                    text: qsTr("Include Uranus, Neptune and Pluto")
                    checked: true
                }
            }
            SettingsItem {
                setting: CheckBox {
                    text: qsTr("Include Lilith")
                }
            }
            SettingsItem {
                setting: CheckBox {
                    id: dragonHeadCheck
                    text: qsTr("Include Dragon Head")
                }
            }
            SettingsItem {
                setting: CheckBox {
                    text: qsTr("Include Dragon Tail")
                    enabled: dragonHeadCheck.checked
                    onEnabledChanged: {
                        if (!enabled)
                        {
                            checked = false
                        }
                    }
                }
            }
        }
    }
    SettingsGroupLink {
        title: HoraConfig.aspects.name
        settingsPane: SettingsPane {
            title: HoraConfig.aspects.name
            Repeater {
                model: HoraConfig.aspects
                SettingsItem {
                    setting: CheckBox {
                        text: config_item.name
                        checked: config_item.value
                        onCheckedChanged: {
                            config_item.value = checked
                            checked = Qt.binding(function(){return config_item.value})
                        }
                    }
                }
            }
        }
    }
    SettingsGroupExpanding {
        title: qsTr("House system")
    }
    SettingsGroupExpanding {
        title: qsTr("Fixed stars")

        SettingsItem {
            setting: CheckBox {
                id: fixedStars
                text: qsTr("Include fixed stars")
            }
            hint: qsTr("Fixed stars in conjuction with planets will be denoted.")
        }
    }
    SettingsGroupLink {
        title: HoraConfig.orbis.name

        settingsPane: SettingsPane {
            title: HoraConfig.orbis.name
            id: orbisSettingsPane
            Repeater {
                model: HoraConfig.orbis
                OrbisSettingsGroup {
                    aspectConfig: config_item
                }
            }
        }
    }
}
