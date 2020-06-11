
import QtQuick 2.12
import QtQuick.Controls 2.5
import Symboid.Sdk.Controls 1.0

Row {
    property alias title: objectTitle.text
    property double orbis: Number(orbisValue.value)/10
    onOrbisChanged: {
        orbisValue.value = orbis * 10
        orbis = Qt.binding(function(){return Number(orbisValue.value)/10})
    }

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
        textFromValue: function(value, locale) {
            return Number(value / 10).toLocaleString(locale, 'f', 1)
        }
        valueFromText: function(text, locale) {
            return Number.fromLocaleString(locale, text) * 10;
        }
        MouseArea {
            anchors.fill: parent
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
