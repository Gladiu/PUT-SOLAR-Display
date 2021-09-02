
#include "serialport.h"

#include <QString>
#include <QDebug>

#include <string>
#include <iomanip>
#include <sstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <map>

SerialPort::SerialPort(QObject *parent) : QObject(parent) {
    /* Creating constructor and freeing memory */
    this->serialport_device = new QSerialPort(this);
    velocity = 0;
    leftIndicator = false;
    rightIndicator = false;
    bmsVoltages.assign(29,0);
    bmsError = false;
    std::map<std::string, std::string> temp{
        {"inverter", "290" },
        {"lights", "581"},
        {"bmsError", "085"},
        // Could be a container but its way easier this way and i dont care enought
        {"bms1", "185"},
        {"bms2", "186"},
        {"bms3", "187"},
        {"bms4", "188"},
        {"bms5", "189"}
  };
    canDict.insert(temp.begin(),temp.end());
}

void SerialPort::connectToSerialPort() {
    /* Private variables */
    qDebug() << "Looking for ECM COM port...";
    QList<QSerialPortInfo> serialport_device_list;
    serialport_device_list = QSerialPortInfo::availablePorts();

    /* Looking for ECM COM Port*/
    for (int i = 0; i < serialport_device_list.count(); i++) {
        qDebug() << "Found device name: " +
                    serialport_device_list.at(i).portName() +
                    " and description: " +
                    serialport_device_list.at(i).description();
        if (serialport_device_list.at(i).description() == "PsdEcmComPort") {
            ecm_com_port = serialport_device_list.at(i).portName();
            qDebug() << "Found ECM COM Port under description:" +
                        serialport_device_list.at(i).portName() + " " +
                        serialport_device_list.at(i).description();
            break;
        } else {
            qDebug() << "Could not find ECM COM Port. Checking other ports...";
        }
    }

    /* Establishing connection */
    this->serialport_device->setPortName(ecm_com_port);
    if (serialport_device->open(QSerialPort::ReadWrite)) {
        qDebug() << "Port " + serialport_device->portName() + " opened.";
        this->serialport_device->setBaudRate(QSerialPort::Baud115200);
        this->serialport_device->setDataBits(QSerialPort::Data8);
        this->serialport_device->setParity(QSerialPort::NoParity);
        this->serialport_device->setStopBits(QSerialPort::OneStop);
        this->serialport_device->setFlowControl(QSerialPort::NoFlowControl);
        qDebug() << "Connection with " + serialport_device->portName() +
                    " established.";

    } else {
        qDebug() << "Port could not be opened.";
    }
}
/*
void SerialPort::readData()
{

    // Stop iterating when we cant find a false
    std::map<std::string, bool> recievedMessage{
        {"inverter", false},
        {"lLight", false},
        {"rLight", false},
        {"bmsError", false},
        // Could be a container but its way easier this way and i dont care enought
        {"bms1", false},
        {"bms2", false},
        {"bms3", false},
        {"bms4", false},
        {"bms5", false}
    };


    while(std::find(recievedMessage.begin(), recievedMessage.end(), false) != std::end(recievedMessage)){
        serialport_message = this->serialport_device->readLine();
        std::string message(serialport_message.toStdString());
        std::string id = message.substr(0, 3);

        if (id == canDict["inverter"]) {
            velocity = strtoul( message.substr(12,2).c_str(), NULL, 16);
            recievedMessage["inverter"] = true;
        }
        if (id == canDict["lights"] && (message.substr(8,2) == "02" || message.substr(8,2) == "06")) {
            rightIndicator = true;
            recievedMessage["rLight"] = true;
        }
        if (id == canDict["lights"] && (message.substr(8,2) == "03" || message.substr(8,2) == "06")) {
            leftIndicator = true;
            recievedMessage["lLight"] = true;
        }
        if (id == canDict["bmsError"]){
            bmsError = true;
            recievedMessage["bmsError"] = true;
        }
        if (id == canDict["bms1"]){
            /*
            bmsVoltages[0] =
            bmsVoltages[1] =
            bmsVoltages[2] =
            bmsVoltages[3] =
            bmsVoltages[4] =
            bmsVoltages[5] =
            bmsVoltages[6] =
            bmsVoltages[7] =
            recievedMessage["bms1"] = true;

        }


    }
}
*/

/*
    1 długie
    2 prawy
    3 lewy
    4 przeciwmgielne
    5 krótkie
    6 awaryjne
    9 postojowe
*/
