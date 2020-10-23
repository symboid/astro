
import QtQuick 2.12
import QtQuick.Controls 2.5
import Symboid.Sdk.Hosting 1.0

SettingsPane {
    title: qsTr("Horoscope")
    SettingsGroup {
        title: qsTr("Planets")
        CheckBox {
            text: qsTr("Include Uranus, Neptune and Pluto")
            checked: true
        }
        CheckBox {
            text: qsTr("Include Lilith")
        }
        CheckBox {
            id: dragonHeadCheck
            text: qsTr("Include Dragon Head")
        }
        CheckBox {
            enabled: dragonHeadCheck.checked
            onEnabledChanged: {
                if (!enabled)
                {
                    checked = false
                }
            }

            text: qsTr("Include Dragon Tail")
        }
    }
    SettingsGroup {
        title: qsTr("House system")
    }
    SettingsGroup {
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
