
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
    battery = 0;
    leftIndicator = false;
    rightIndicator = false;
    longLights = false;
    shortLights = false;
    currentWarning = false;
    bmsVoltages.assign(29,0);
    std::map<std::string, std::string> temp{
        {"inverter", "290" },
        {"lights", "581"},
        {"bms", "085"},
        {"battery", "18B"},
        {"current", "056"}
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

void SerialPort::readData()
{

    // Stop iterating when we cant find a false
    std::map<std::string, bool> recievedMessage{
        {"inverter", true},
        {"lights", true},
        {"bms", false},
        {"current", false},
        {"battery", false}
    };

    auto isFalse = [](std::pair<std::string, bool> element){return element.second;};
    while(std::find_if(recievedMessage.begin(), recievedMessage.end(), isFalse) != std::end(recievedMessage)){
        serialport_message = this->serialport_device->readLine();
        std::string message(serialport_message.toStdString());
        std::string id = message.substr(0, 3);

        if (id == canDict["inverter"]) {
            velocity = strtoul(message.substr(12,2).c_str(), NULL, 16);
            recievedMessage["inverter"] = true;
        }
        if (id == canDict["lights"]) {
            std::string lightType = message.substr(8,2);
            longLights = lightType == "01";
            rightIndicator = (lightType == "02" || lightType == "06");
            leftIndicator = (lightType == "03" || lightType == "06");
            shortLights = lightType == "05";
            recievedMessage["lights"] = true;
        }
        if (id == canDict["bms"]){
            recievedMessage["bms"] = true;
            bmsErrorIndex =  strtoul(message.substr(9,1).c_str(), NULL, 16);
            }
        if (id == canDict["current"]){
            recievedMessage["current"] = true;
            currentWarning =  static_cast<bool>(message.substr(9,1).c_str());

        }
        if (id == canDict["battery"]){
            recievedMessage["battery"] = true;
            battery =  strtoul(message.substr(10,2).c_str(), NULL, 16);
        }
        }


    }
/*
    1 długie
    2 prawy
    3 lewy
    4 przeciwmgielne
    5 krótkie
    6 awaryjne
    9 postojowe
*/
