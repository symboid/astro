
import QtQuick 2.12
import QtQuick.Controls 2.5
import Symboid.Sdk.Controls 1.0

Row {
    property alias title: objectTitle.text
    property alias orbis: orbisValue.value
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
    }
}
