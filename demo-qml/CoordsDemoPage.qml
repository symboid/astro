
import QtQuick 2.12
import QtQuick.Controls 2.5
import Symboid.Astro.Controls 1.0

Grid {
    padding: 20
    columns: 4
    spacing: 20
    verticalItemAlignment: Grid.AlignVCenter

    //--------------

    Text {
        text: "ArcCoordBox -> value"
    }
    ArcCoordBox {
        id: arcCoordBox
        editable: true
    }
    Text {
        text: arcCoordBox.arcDegree
    }
    Item { width:1;height:1 }

    //--------------

    Text {
        text: "value -> ArcCoordBox"
    }
    TextField {
        id: arcDegreeInput
        text: "0.0"
        validator: RegExpValidator {
            regExp: new RegExp("\-?[0-9]+\.[0-9]+")
        }
    }
    ArcCoordBox {
        id: arcCoordOutput
        editable: true
        arcDegree: Number(arcDegreeInput.text)
    }

    Text {
        text: arcCoordOutput.arcDegree
    }

    //--------------

    Text {
        text: "GeoCoords - Longitude"
    }
    TextField {
        id: geoLontInput
        text: "0.0"
        validator: RegExpValidator {
            regExp: new RegExp("\-?[0-9]*\.[0-9]+")
        }
        onTextChanged: geoLontOutput.arcDegree = Number(text)
    }
    ArcCoordBox {
        id: geoLontOutput
        editable: true
        sectionCalc: GeoLontSectionCalc {}
    }
    Text {
        text: geoLontOutput.arcDegree
    }


    //--------------

    Text {
        text: "GeoCoords - Lattitude"
    }
    TextField {
        id: geoLattInput
        text: "0.0"
        validator: RegExpValidator {
            regExp: new RegExp("\-?[0-9]*\.[0-9]+")
        }
        onTextChanged: geoLattOutput.arcDegree = Number(text)
    }
    ArcCoordBox {
        id: geoLattOutput
        //isLattitude: true
        editable: true
        sectionCalc: GeoLattSectionCalc {}
    }
    Text {
        text: geoLattOutput.arcDegree
    }
    Item { width:1;height:1 }
}
