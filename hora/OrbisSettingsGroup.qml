
import QtQuick 2.12
import QtQuick.Controls 2.5
import Symboid.Sdk.Hosting 1.0

SettingsGroupExpanding {

    property ConfigNode aspectConfig: null
    title: aspectConfig.name

    Repeater {
        model: aspectConfig
        Column {
            spacing: 10
            width: parent.width
            OrbisSettingsItem {
                id: orbisSetting
                aspectConfig: config_item
            }
        }
    }
}
