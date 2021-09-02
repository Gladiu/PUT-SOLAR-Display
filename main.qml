import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Extras 1.4




ApplicationWindow {
    width: 1920
    height: 1080
    visible: true
    title: qsTr("Dashboard gauges, PUT Solar Dynamics")

    Image {
        id: background
        anchors.fill: parent
        source: "file:///home/upsquared/Documents/qt-ws/psd-main-computer/dashpngs/backgrounds_195_610.png"
        mipmap: true
        fillMode: Image.Stretch

        Image {
            id: psd_logo_white
            x: 1696
            y: 30
            opacity: 0.5
            source: "file:///home/upsquared/Documents/qt-ws/psd_main_computer/dashpngs/psd_logo_white.png"
            fillMode: Image.PreserveAspectFit
        }


        CircularGauge {
            id: circGaugeSpeedometer
            objectName: "speedometer"
            x: 702
            y: 194
            width: 516
            height: 516
            value: 100
            enabled: true
            stepSize: 0
            maximumValue: 200
            Connections {
                target: serialPort
            }
            Timer
            {
                interval: 200; running: true; repeat: true
                onTriggered: circGaugeSpeedometer.value = serialPort.getVelocity()
            }


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
            value: 15//qsTr(serialPort.battery)
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

        IsoIconsBar {
            id: iso_195_156
            x: 511
            y: 796
        }

        TurnIndicator{
            id: leftIndicator
            x: 474
            y: 360
            width: 142
            height: 152
            flashing: true
            on: true
            direction: 3
            Connections {
                target: serialPort
            }
            /*
            Timer
            {
                interval: 200; running: true; repeat: true
                onTriggered: leftIndicator.on = serialPort.getLeftIndicator()
            }
*/

        }

        TurnIndicator {
            id: rightIndicator
            x: 1308
            y: 360
            width: 142
            height: 152
            on: true
            flashing: true
            direction: 2
            Connections {
                target: serialPort
            }
            Timer
            {
                interval: 200; running: true; repeat: true
                onTriggered:{
                    rightIndicator.on = serialPort.getRightIndicator()
                    leftIndicator.on = serialPort.getLeftIndicator()
                }
            }
        }

    }


}



/*##^##
Designer {
    D{i:0;formeditorZoom:0.5}
}
##^##*/
