
import QtQuick 2.12
import QtQuick.Controls 2.5
import Symboid.Sdk.Hosting 1.0
import Symboid.Astro.Controls 1.0

SettingsGroup {

    property ConfigNode aspectConfig: null
    title: aspectConfig.name

    Repeater {
        model: aspectConfig
        Column {
            spacing: 10
            width: parent.width
            Rectangle {
                width: parent.width
                height: 1
                color: "lightgrey"
                visible: index === 10 || index === 7
            }
            OrbisSettingsItem {
                id: orbisSetting
                aspectConfig: config_item
            }
        }
    }
}
