
import QtQuick 2.12
import QtQuick.Controls 2.5
import Symboid.Sdk.Controls 1.0

SettingsGroup {

    Repeater {
        model: [ qsTr("Sun"), qsTr("Moon"), qsTr("Mercury"), qsTr("Venus"),
            qsTr("Mars"), qsTr("Jupiter"), qsTr("Saturn") ]

        OrbisSettingsItem {
            title: modelData
        }
    }

    Repeater {
        model: [ qsTr("Uranus"), qsTr("Neptune"), qsTr("Pluto") ]

        OrbisSettingsItem {
            title: modelData
        }
    }
}
