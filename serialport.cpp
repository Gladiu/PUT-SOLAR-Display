
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


    velocity = 0;
    battery = 0;
    leftIndicator = false;
    rightIndicator = false;
    longLights = false;
    shortLights = false;
    currentWarning = false;
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
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        qDebug() << "Name : " << info.portName();
        qDebug() << "Description : " << info.description();
        qDebug() << "Manufacturer: " << info.manufacturer();
        if (info.description() == QString::fromStdString("PsdEcmComPort")){
            serialportDevice.setPort(info);
            serialportInfo = info;
        }
    }

    /* Establishing connection */
    serialportDevice.setPortName(serialportInfo.portName());
    if (serialportDevice.open(QSerialPort::ReadOnly)) {
        qDebug() << "Port " + serialportDevice.portName() + " opened.";
        serialportDevice.setBaudRate(QSerialPort::Baud115200);
        serialportDevice.setDataBits(QSerialPort::Data8);
        serialportDevice.setParity(QSerialPort::NoParity);
        serialportDevice.setStopBits(QSerialPort::OneStop);
        serialportDevice.setFlowControl(QSerialPort::NoFlowControl);
    } else {
        qDebug() << "Port could not be opened. Are you running program as sudo?";
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
        std::string message;
        auto data = serialportDevice.readLine();
        for (int i = 0; i < data.size(); ++i)
        {
            message.push_back(data.at(i));
        }
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
