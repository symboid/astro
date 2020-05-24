
import QtQuick 2.12
import QtQuick.Controls 2.5
import Symboid.Sdk.Hosting 1.0

SettingsPane {
    minimumColumnWidth: 300
    title: qsTr("Orbis")

    Repeater {
        model: [ qsTr("Conjunction"), qsTr("Opposition"), qsTr("Trigon"), qsTr("Quadrat"),
            qsTr("Quintil"), qsTr("Sextil"), qsTr("Quincunx"), qsTr("Semi-sextil") ]
        OrbisSettingsGroup {
            title: modelData
            collapsed: true
        }
    }

    SettingsGroup {
        title: qsTr("Fixed stars")
        collapsed: true
        OrbisSettingsItem {
            title: qsTr("Alpha star")
            orbis: 20
        }
        OrbisSettingsItem {
            title: qsTr("Other star")
            orbis: 10
        }
    }
}
