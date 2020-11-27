
import QtQuick 2.12
import QtQuick.Controls 2.5
import Symboid.Sdk.Hosting 1.0

SettingsGroupLink {
    id: orbisGroup

    property ConfigNode aspectConfig: null

    property var elementNames: []

    settingsPane: SettingsPane {
        title: orbisGroup.title
        Repeater {
            model: aspectConfig
            OrbisSettingsItem {
                id: orbisSetting
                text: elementNames[index]
                aspectConfig: config_item
            }
        }
    }
}
