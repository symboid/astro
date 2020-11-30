
import QtQuick 2.12
import QtQuick.Controls 2.5
import Symboid.Sdk.Hosting 1.0

SettingsSwitch {
    id: group
    property ConfigNode aspectConfig: null
    configNode: aspectConfig.enabled_node

    rightItem: Item {
        width: defaultItemHeight
        height: defaultItemHeight
        RoundButton {
            id: expandButton
            anchors.centerIn: parent
            icon.source: "/icons/br_next_icon&24.png"
            visible: group.checked
            onClicked: {
                if (settingsPane !== null) {
                    settingsView.push(settingsPane.createObject(settingsView,{}))
                }
            }
        }
    }

    property var elementNames: [
        qsTr("Sun"), qsTr("Moon"), qsTr("Mercury"), qsTr("Venus"), qsTr("Mars"),
        qsTr("Jupiter"), qsTr("Saturn"), qsTr("Uranus"), qsTr("Neptune"), qsTr("Pluto"),
        qsTr("Asc"), qsTr("MC"), qsTr("House cusps"), qsTr("Lunar nodes"), qsTr("Lilith")
    ]

    property Component settingsPane: SettingsPane {
        title: text
        Repeater {
            model: aspectConfig.orbis
            OrbisSettingsItem {
                id: orbisSetting
                text: elementNames[index]
                orbisConfig: config_item
                withSeparator: index === 9 || index === 12
            }
        }
    }
}
