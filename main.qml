import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Extras 1.4
import QtQuick.Window 2.12




ApplicationWindow {
    id: app
    width: 1920
    height: 1080
    visible: true
    title: qsTr("Dashboard gauges, PUT Solar Dynamics")
    property bool mode: true
    MouseArea{
        anchors.fill: parent
        onClicked:{
//            normalMode.toggle()
//            bmsMode.toggle()
        }
    }

    Image {
        id: background
        anchors.fill: parent
        source: "file:///" + appPath + "/dashpngs/backgrounds_195_610.png"
        mipmap: true
        fillMode: Image.Stretch
    }
    Rectangle{
        id: normalMode
        visible: true
        function toggle()
        {
            visible = !visible
        }
        Connections {
            target: serialPort
        }
        Timer
        {
            interval: 200; running: true; repeat: true
            onTriggered:{
                //serialPort.readData();
                rightIndicator.on = serialPort.getRightIndicator();
                leftIndicator.on = serialPort.getLeftIndicator();
                longLights.visible = serialPort.getLongLights();
                shortLights.visible = serialPort.getShortLights();
                awarLights.visible = serialPort.getAwarLights();
                circGaugeSpeedometer.value = serialPort.getVelocity();
                bmsError.text = serialPort.getBMSError();
                currentWarning.visible = true;//serialPort.getCurrentWarning();
                circGaugeBattery.value = serialPort.getBattery();
            }
        }
        CircularGauge {
            id: circGaugeSpeedometer
            objectName: "speedometer"
            x: 702
            y: 230
            width: 516
            height: 516
            value: 100
            stepSize: 0
            maximumValue: 100
        }
        Text{
            id: bmsError
            color: "red"
            text:  qsTr("TEST")
            font.pixelSize: 100
        }
        Text{
            id: currentWarning
            color: "red"
            text: qsTr("Overcurrent Warning!")
            y: 100
            font.pixelSize: 100
        }
        CircularGauge {
            id: circGaugePower
            x: 1514
            y: 370
            width: 335
            height: 728
            value: 7
            stepSize: 1
            maximumValue: 12
            style: IconGaugeStyle {
                id: tempGaugeStyle

                //                icon: "qrc:/images/temperature-icon.png"
                maxWarningColor: Qt.rgba(0.5, 0, 0, 1)

                tickmarkLabel: Text {
                    color: "white"
                    visible: styleData.value === 0 || styleData.value === 12
                    text: styleData.value === 0 ? "0" : (styleData.value === 12 ? "12" : "12")
                }
            }

            Text {
                id: text1
                x: 126
                y: 388
                width: 149
                height: 32
                color: "#f7f6f6"
                text: qsTr("POWER")
                font.pixelSize: 30
            }

            Text {
                id: txtPower
                x: 126
                y: 426
                width: 39
                height: 32
                color: "#f7f6f6"
                //circGaugePower.value
                font.pixelSize: 30
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignTop
            }

            Text {
                id: text3
                x: 171
                y: 426
                width: 149
                height: 32
                color: "#f7f6f6"
                text: qsTr("kW")
                font.pixelSize: 30
            }


        }

        CircularGauge {
            id: circGaugeBattery
            x: 72
            y: 556
            width: 408
            height: 312
            value: 0
            maximumValue: 100
            stepSize: 1
            style: IconGaugeStyle {
                id: tempGaugeStyle1

                //                icon: "qrc:/images/temperature-icon.png"
                maxWarningColor: Qt.rgba(0.5, 0, 0, 1)

                tickmarkLabel: Text {
                    color: "white"
                    visible: styleData.value === 0 || styleData.value === 100
                    font.pixelSize: tempGaugeStyle1.toPixels(0.225)
                    text: styleData.value === 0 ? "0" : (styleData.value === 100 ? "100" : "100")
                    //                    text: "POWER"
                }

            }

            Text {
                id: text5
                x: 130
                y: 175
                width: 149
                height: 32
                color: "#f7f6f6"
                text: qsTr("BATTERY")
                font.pixelSize: 30
            }

            Text {
                id: text6
                x: 207
                y: 213
                width: 149
                height: 32
                color: "#f7f6f6"
                text: qsTr("%")
                font.pixelSize: 30
            }

            Text {
                id: txtBattery
                x: 152
                y: 213
                width: 49
                height: 32
                color: "#f7f6f6"
                text: circGaugeBattery.value
                font.pixelSize: 30
                horizontalAlignment: Text.AlignHCenter
            }
        }
        TurnIndicator{
            id: leftIndicator
            x: 474
            y: 360
            width: 142
            height: 152
            flashing: true
            on: false
            direction: 3
        }

        TurnIndicator {
            id: rightIndicator
            x: 1308
            y: 360
            width: 142
            height: 152
            on: false
            flashing: true
            direction: 2
        }
        Rectangle{
            x: 811
            y: 796
            Image {
                id: shortLights
                width: 100
                height: 90
                source: "file:///" + appPath + "/dashpngs/krotkie.png"
                visible: false
            }
            Image {
                id: longLights
                x: 110
                width: 100
                height: 90
                source: "file:///" + appPath + "/dashpngs/dlugie.png"
                visible: false
            }
            Image {
                id: awarLights
                x: 220
                width: 100
                height: 90
                source: "file:///" + appPath + "/dashpngs/awar.png"
                visible: false
            }

        }
    }
/*
    Rectangle{
        id: bmsMode
        visible: false
        function toggle()
        {
            visible = !visible
        }

        Timer
        {
            interval: 200; running: true; repeat: true
            onTriggered:
            {

            }
        }
        Text {
            anchors.fill:parent
            color: "white"
            id: myText
            font.family: "Helvetica"
            font.pointSize: 50
            text:  qsTr("No BMS Voltages avalible!")
        }
    } */
}





/*##^##
Designer {
    D{i:0;formeditorZoom:0.5}
}
##^##*/
