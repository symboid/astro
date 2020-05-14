
import QtQuick 2.12
import QtQuick.Controls 2.5
import Symboid.Sdk.Controls 1.0

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
            text: qsTr("Include Chiron")
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

        SettingsItem {
            setting: CheckBox {
                text: qsTr("Show constellations")
            }
            hint: qsTr("Constellation structures calculated by the approach of Hungarian Astrosophy")
        }
    }
}
