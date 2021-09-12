
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

    power = 0;
    velocity = 0;
    battery = 0;
    bmsErrorIndex = 0;
    leftIndicator = false;
    rightIndicator = false;
    longLights = false;
    shortLights = false;
    currentWarning = false;
    bmsMode	= false;
    std::map<std::string, std::string> tempMap{
        {"inverter", "290" },
        {"lights", "581"},
        {"bmsError", "085"},
        {"battery", "18B"},
        {"current", "056"},
        {"voltage0", "185"},
        {"voltage1", "186"},
        {"voltage2", "187"},
        {"voltage3", "188"},
        {"bmsGeneral", "189"},
        {"bmsTemp","18A"}
  };
    canDict.insert(tempMap.begin(),tempMap.end());
    std::vector<std::string> tempVec(4,"");
    bmsVoltages = tempVec;
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
        {"bmsError", true},
        {"current", true},
        {"battery", true},
        {"power", true},
        {"voltage0", false},
        {"voltage1", false},
        {"voltage2", false},
        {"voltage3", false},
        {"bmsGeneral", true},
        {"bmsTemp", true}
    };


    auto havntRead = [](std::pair<std::string, bool> element){return !element.second;};
    while(std::find_if(recievedMessage.begin(), recievedMessage.end(), havntRead) != std::end(recievedMessage)){

        auto data = serialportDevice.readLine();
        std::string message = data.toStdString();
        // I dont know why and how but without this loop it doesnt work
        for(int i = 0; i < 50000; i++){
            serialportDevice.canReadLine();
        }
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
        if (id == canDict["bmsError"]){
            recievedMessage["bmsError"] = true;
            bmsErrorIndex =  strtoul(message.substr(9,1).c_str(), NULL, 16);
            }
        if (id == canDict["current"]){
            recievedMessage["current"] = true;
            currentWarning =  static_cast<bool>(message.substr(9,1).c_str());

        }
        if (id == canDict["battery"]){
            recievedMessage["battery"] = true;
            battery =  strtoul(message.substr(10,2).c_str(), NULL, 16);
            power = strtoul(message.substr(6,4).c_str(), NULL, 16);
        }

        // Bms voltages
        for (int i = 0; i < 4; i++)
        {
            std::string key = "voltage";
            key.append(std::to_string(i));
            if (id == canDict[key]){
                recievedMessage[key] = true;
                bmsVoltages[i] = "";
                for (int j = 0; j < 8; j++)
                {
                    double voltage = strtoul(message.substr(6+(j*2),2).c_str(), NULL, 16);
                    voltage = voltage/100.0+1.85;
                    if (j != 0)
                        bmsVoltages[i].append(std::string(" "));
                    // It should work without this if but it doesnt
                    if (i < 4)
                        bmsVoltages[i].append(std::to_string(voltage).substr(0,4));
                }
            }
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

double SerialPort::getVelocity(){return velocity;}
double SerialPort::getBattery(){return battery;}
double SerialPort::getPower(){return power;}
bool SerialPort::getRightIndicator(){return rightIndicator;}
bool SerialPort::getLeftIndicator(){return leftIndicator;}
bool SerialPort::getLongLights(){return longLights;}
bool SerialPort::getShortLights(){return shortLights;}
bool SerialPort::getAwarLights(){return leftIndicator && rightIndicator;}
QString SerialPort::getBMSError(){return QString::fromStdString(bmsErrors[bmsErrorIndex]);}
bool SerialPort::getCurrentWarning(){return currentWarning;}
QString SerialPort::getBMSVoltage(int index){return QString::fromStdString(bmsVoltages[index]);}
bool SerialPort::getBMSMode(){return bmsMode;}
QString SerialPort::getBMSTemperature(){return QString::fromStdString(bmsTemperatures);};
