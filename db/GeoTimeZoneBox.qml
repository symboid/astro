
import QtQuick 2.12
import QtQuick.Controls 2.5
import Symboid.Sdk.Controls 1.0
import Symboid.Sdk.Network 1.0
import Symboid.Sdk.Hosting 1.0

Row {

    spacing: 10
    Label {
        anchors.verticalCenter: parent.verticalCenter
        text: qsTr("Time zone diff:")
    }

    readonly property string displaySuffix: qsTr("h")
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
    property double diffHours: tzDiffBox.box(0).value / 2
    onDiffHoursChanged: {
        setHour(diffHours)
        diffHours = Qt.binding(function(){return tzDiffBox.box(0).value / 2})
    }
    function setHour(hour)
    {
        tzDiffBox.box(0).value = hour * 2
    }

    signal queryFinished
    signal queryAborted

    property double geoNameLatt: 0.0
    property double geoNameLont: 0.0
    property int currentUnixTime: 0
    property double prevLatt: 0
    property double prevLont: 0
    RestObjectModel {
        id: timeZoneModel
        restClient: RestClient {
            apiAddress: "api.timezonedb.com"
            secure: AppConfig.software.ssl_supported
            authUser: "symboid"
        }
        operation: "v2.1/get-time-zone"+
                   "?key=7JKCP2G245UG"+
                   "&format=json"+
                   "&by=position"+
                   "&time="+currentUnixTime+
                   "&lat="+geoNameLatt+
                   "&lng="+geoNameLont
        onSuccessfullyFinished: {
            setHour(restObject.gmtOffset / 3600)
            prevLatt = geoNameLatt
            prevLont = geoNameLont
            queryFinished()
        }
        onNetworkError: {
            setHour(0)
            queryAborted()
        }
    }
    function search()
    {
        if (prevLatt !== geoNameLatt || prevLont !== geoNameLont)
        {
            timeZoneModel.runOperation()
        }
        else
        {
            setHour(timeZoneModel.restObject.gmtOffset / 3600)
        }
    }

    Component.onCompleted: {
        setHour(0)
    }

}
