
import QtQuick 2.12
import QtQuick.Controls 2.5
import Symboid.Sdk.Hosting 1.0

SettingsGroupLink {

    property ConfigNode aspectConfig: null
    title: aspectConfig.name

    settingsPane: SettingsPane {
        title: aspectConfig.name
        Repeater {
            model: aspectConfig
            OrbisSettingsItem {
                id: orbisSetting
                aspectConfig: config_item
            }
        }
    }
}
