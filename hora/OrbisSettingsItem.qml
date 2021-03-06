
import QtQuick 2.12
import QtQuick.Controls 2.5
import Symboid.Sdk.Controls 1.0
import Symboid.Sdk.Hosting 1.0

FolderItem {
    property alias text: objectTitle.text
    property ConfigNode orbisConfig: null
    mainItem: Row {

        leftPadding: 20
        rightPadding: 20
        Label {
            id: objectTitle
            width: parent.parent.width - orbisValue.width - parent.leftPadding - parent.rightPadding
            height: orbisValue.height
            verticalAlignment: Label.AlignVCenter
        }
        SpinBox {
            id: orbisValue
            from: 0
            to: 100
            value: orbisConfig.value * 10
            onValueChanged: {
                orbisConfig.value = Number(value)/10
                value = Qt.binding(function(){return orbisConfig.value*10})
            }

            textFromValue: function(value, locale) {
                return Number(value / 10).toLocaleString(locale, 'f', 1)
            }
            valueFromText: function(text, locale) {
                return Number.fromLocaleString(locale, text) * 10;
            }
            MouseArea {
                anchors {
                    left: parent.down.indicator.right
                    right: parent.up.indicator.left
                    top: parent.top
                    bottom: parent.bottom
                }
                onWheel: {
                    if (wheel.angleDelta.y > 0)
                    {
                        orbisValue.increase()
                    }
                    else
                    {
                        orbisValue.decrease()
                    }
                }
            }
        }
    }
}
