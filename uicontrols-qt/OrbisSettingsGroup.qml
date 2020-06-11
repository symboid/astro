
import QtQuick 2.12
import QtQuick.Controls 2.5
import Symboid.Sdk.Hosting 1.0
import Symboid.Astro.Controls 1.0

SettingsGroup {

    property ConfigNode aspectConfig: null
    title: aspectConfig.name

    Repeater {
        model: aspectConfig
        OrbisSettingsItem {
            title: config_name
            orbis: config_value
        }
    }
}
