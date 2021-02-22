
import QtQuick 2.12
import QtQuick.Controls 2.5
import Symboid.Sdk.Hosting 1.0

SettingsItem {
    property alias text: label.text
    property ConfigNode aspectConfig: null
    property ConfigNode configNode: aspectConfig.enabled_node

    setting: Label {
        id: label
    }
    rightItem: Row {
        Switch {
            id: switchButton
            anchors.verticalCenter: parent.verticalCenter
            checked: configNode.value
            onCheckedChanged: {
                configNode.value = checked
                checked = Qt.binding(function(){return configNode.value})
            }
        }
        RoundButton {
            id: expandButton
            anchors.verticalCenter: parent.verticalCenter
            icon.source: "/icons/br_next_icon&24.png"
            visible: switchButton.checked
            onClicked: settingsView.loadPane(settingsPane)
        }
        Item {
            width: expandButton.width
            height: expandButton.height
            visible: !expandButton.visible
        }
    }

    property var elementNames: [
        qsTr("Sun"), qsTr("Moon"), qsTr("Mercury"), qsTr("Venus"), qsTr("Mars"),
        qsTr("Jupiter"), qsTr("Saturn"), qsTr("Uranus"), qsTr("Neptune"), qsTr("Pluto"),
        qsTr("Asc/Dsc"), qsTr("MC/IC"), qsTr("House cusps"), qsTr("Lunar nodes"), qsTr("Lilith")
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
