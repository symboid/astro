
import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Window 2.12
import Symboid.Sdk.Controls 1.0
import Symboid.Astro.Controls 1.0
import Symboid.Astro.Hora 1.0

ApplicationWindow {
    id: mainWindow
    visible: true
    visibility: Window.Maximized
    width: 900
    height: 600

    DemoScreen {
        anchors.fill: parent
    }
}
