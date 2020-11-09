
import QtQuick 2.12
import QtQuick.Controls 2.5
import Symboid.Sdk.Hosting 1.0
import Symboid.Astro.Hora 1.0

SettingsGroup {
    title: OrbisConfig.name

    Repeater {
        model: OrbisConfig
        OrbisSettingsGroup {
            aspectConfig: config_item
            expanded: false
        }
    }
}
