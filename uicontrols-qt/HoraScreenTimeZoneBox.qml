
import QtQuick 2.12
import QtQuick.Controls 2.5
import Symboid.Sdk.Controls 1.0

Row {
    readonly property string displaySuffix: qsTr("h")

    spacing: 10
    Label {
        anchors.verticalCenter: parent.verticalCenter
        text: qsTr("Time zone diff:")
    }

    MultiNumberBox {
        id: tzDiffBox
        editable: true
        boxes: Row {
            NumericBox {
                from: -12 * 2
                to: 12 * 2
                textFromValue: function(value, locale)
                {
                    var text = String(value / 2) + displaySuffix
                    return text
                }
                valueFromText: function(text, locale)
                {
                    var suffixPos = String(text).indexOf(displaySuffix)
                    var valueString = (suffixPos !== 0) ? String(text).substring(0, suffixPos) : text
                    return Number(valueString) * 2
                }
                validator: RegExpValidator {
                    regExp: new RegExp("-?1?[0-9](\\" + "." + "[05])?" + displaySuffix)
                }
            }
        }
    }
    property int hour: tzDiffBox.box(0).value / 2
    function setHour(hour)
    {
        tzDiffBox.box(0).value = hour * 2
    }
    Component.onCompleted: {
        setHour(0)
    }
}
