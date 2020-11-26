
import QtQuick 2.12
import QtQuick.Controls 2.5
import Symboid.Sdk.Hosting 1.0

SettingsGroupLink {
    id: orbisGroup

    property ConfigNode aspectConfig: null

    property var planets: [
        qsTr("Sun"), qsTr("Moon"), qsTr("Mercury"), qsTr("Venus"), qsTr("Mars"),
        qsTr("Jupiter"), qsTr("Saturn"), qsTr("Uranus"), qsTr("Neptune"), qsTr("Pluto"),
        qsTr("Asc"), qsTr("MC"), qsTr("Lunar nodes"), qsTr("House cusps")
    ]

    settingsPane: SettingsPane {
        title: orbisGroup.title
        Repeater {
            model: aspectConfig
            OrbisSettingsItem {
                id: orbisSetting
                text: planets[index]
                aspectConfig: config_item
            }
        }
    }
}
