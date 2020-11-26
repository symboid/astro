
import QtQuick 2.12
import QtQuick.Controls 2.5
import Symboid.Sdk.Hosting 1.0
import Symboid.Astro.Hora 1.0

SettingsGroupFixed {
    title: qsTr("Horoscope")
    SettingsGroupLink {
        title: HoraConfig.aspects.title
        settingsPane: SettingsPane {
            title: HoraConfig.aspects.title
            Repeater {
                model: HoraConfig.aspects
                SettingsSwitch {
                    configNode: config_item
                }
            }
        }
    }
    SettingsGroupExpanding {
        title: HoraConfig.fixstars.title

        SettingsCheckBox {
            configNode: HoraConfig.fixstars.enabled_node
            hint: qsTr("Fixed stars in conjuction with planets will be denoted.")
        }
    }
    SettingsGroupLink {
        title: HoraConfig.orbis.title

        settingsPane: SettingsPane {
            title: HoraConfig.orbis.title
            id: orbisSettingsPane
            Repeater {
                model: HoraConfig.orbis
                OrbisSettingsGroup {
                    aspectConfig: config_item
                }
            }
        }
    }
    SettingsGroupExpanding {
        title: qsTr("Karmic points")
        SettingsCheckBox {
            id: dragonHeadCheck
            configNode: HoraConfig.karma.dragonHead_node
        }
        SettingsCheckBox {
            id: dragonTailCheck
            configNode: HoraConfig.karma.dragonTail_node
            enabled: dragonHeadCheck.checked
            onEnabledChanged: {
                if (!enabled)
                {
                    checked = false
                }
            }
        }
        SettingsCheckBox {
            configNode: HoraConfig.karma.lilith_node
        }
        SettingsCheckBox {
            configNode: HoraConfig.karma.drawAxis_node
            enabled: dragonTailCheck.checked
            onEnabledChanged: {
                if (!enabled)
                {
                    checked = false
                }
            }
        }
    }
}
