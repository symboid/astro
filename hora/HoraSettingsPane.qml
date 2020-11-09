
import QtQuick 2.12
import QtQuick.Controls 2.5
import Symboid.Sdk.Hosting 1.0

SettingsGroup {
    title: qsTr("Horoscope")
    SettingsGroupExpanding {
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
}
