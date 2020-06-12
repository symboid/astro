
import QtQuick 2.12
import QtQuick.Controls 2.5
import Symboid.Sdk.Hosting 1.0
import Symboid.Astro.Controls 1.0

SettingsPane {
    minimumColumnWidth: 300
    title: OrbisConfig.name

    Repeater {
        model: OrbisConfig
        OrbisSettingsGroup {
            aspectConfig: config_item
            collapsed: true
        }
    }
}
