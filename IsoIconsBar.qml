import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Extras 1.4
import QtQuick.Layouts 1.0


Image {
    id: iso_195_156
    source: "file:///home/upsquared/Documents/qt-ws/psd_main_computer/dashpngs/iso_195_156.png"
    fillMode: Image.PreserveAspectFit

    RowLayout {
        x: 0
        y: -6
        height: 85

        IsoParking {
            id: iconLongLights
            y: 8
            width: 65
            height: 65
            color: "#555753"
            source: "dashicons/iso_grs_7000_4_0240.dat"
            active: false
        }

        IsoIconLights {
            id: iconLights
            y: 8
            active: false
        }

    
    
    
}

/*##^##
Designer {
    D{i:0;height:67;width:899}
}
##^##*/
}
