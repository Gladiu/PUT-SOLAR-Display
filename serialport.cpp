
#include "serialport.h"

#include <QString>
#include <QDebug>
#include <QTimer>

#include <string>
#include <iomanip>
#include <sstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <map>

// C library headers
#include <stdio.h>
#include <string.h>

// https://blog.mbedded.ninja/programming/operating-systems/linux/linux-serial-ports-using-c-cpp/

// Linux headers
#include <fcntl.h> // Contains file controls like O_RDWR
#include <errno.h> // Error integer and strerror() function
#include <termios.h> // Contains POSIX terminal control definitions
#include <unistd.h> // write(), read(), close()

#include <iostream>

SerialPort::SerialPort(QObject *parent) : QObject(parent) {
    serialportDevice = new QSerialPort("/dev/ACM0");
    power = 0;
    velocity = 0;
    battery = 0;
    leftIndicator = false;
    rightIndicator = false;
    longLights = false;
    shortLights = false;
    bmsMode	= false;
    charging = false;
    std::map<std::string, std::string> tempMap{
        {"inverter", "290" },
        {"lights", "581"},
        {"battery", "18B"},
        {"current", " 55"},
        {"voltage0", "185"},
        {"voltage1", "186"},
        {"voltage2", "187"},
        {"voltage3", "188"},
        {"bmsGeneral", "189"},
        {"bmsWarning", " 87"},
        {"bmsTemp","18A"},
        {"charging", " 1C"}
  };
    canDict.insert(tempMap.begin(),tempMap.end());
    bmsVoltages = std::vector<std::string>(4,"");
    bmsTemperatures = std::vector<std::string>(2,"");
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
            serialportDevice->setPort(info);
            serialportInfo = info;
        }
    }

    /* Establishing connection */
    //serialportDevice->setPortName(serialportInfo.portName());

    if (serialportDevice->open(QSerialPort::ReadOnly)) {
        qDebug() << "Port " + serialportDevice->portName() + " opened.";
        serialportDevice->setBaudRate(QSerialPort::Baud115200);
        serialportDevice->setDataBits(QSerialPort::Data8);
        serialportDevice->setParity(QSerialPort::NoParity);
        serialportDevice->setStopBits(QSerialPort::OneStop);
        serialportDevice->setFlowControl(QSerialPort::NoFlowControl);
    }
    else{
        qDebug() << "Port Could not be opened";
    }
    serialportDevice->clear(QSerialPort::AllDirections);
    if(serialportDevice->isOpen())
        serialportDevice->close();
}

void SerialPort::readData()
{
    // Stop iterating when we cant find a false
    std::map<std::string, bool> recievedMessage;
    if (charging){
        std::map<std::string, bool> temp{
            {"current", false},
            {"battery", false},
            {"power", false},
            {"voltage0", false},
            {"voltage1", false},
            {"voltage2", false},
            {"voltage3", false},
            {"bmsGeneral", false},
            {"bmsWarning", false},
            {"bmsTemp", false},
            };
        recievedMessage.insert(temp.begin(), temp.end());
    }
    else{
        std::map<std::string, bool> temp{
            {"inverter", false},
            {"lightLong", false},
            {"lightLeft", false},
            {"lightRight", false},
            {"current", false},
            {"battery", false},
            {"power", false},
            {"voltage0", false},
            {"voltage1", false},
            {"voltage2", false},
            {"voltage3", false},
            {"bmsGeneral", false},
            {"bmsWarning", false},
            {"bmsTemp", false},
            };
        recievedMessage.insert(temp.begin(), temp.end());
    }

    auto havntRead = [](std::pair<std::string, bool> element){return !element.second;};
    while(std::find_if(recievedMessage.begin(), recievedMessage.end(), havntRead) != std::end(recievedMessage)){

        // Code from stackoverflow
        int USB = open( "/dev/ttyACM0", O_RDWR| O_NOCTTY );
        struct termios tty;
memset (&tty, 0, sizeof tty);

/* Error Handling */
if ( tcgetattr ( USB, &tty ) != 0 ) {
   std::cout << "Error " << errno << " from tcgetattr: " << strerror(errno) << std::endl;
}

/* Save old tty parameters */
//tty_old = tty;

/* Set Baud Rate */
cfsetospeed (&tty, (speed_t)B115200);
cfsetispeed (&tty, (speed_t)B115200);

/* Setting other Port Stuff */
tty.c_cflag     &=  ~PARENB;            // Make 8n1
tty.c_cflag     &=  ~CSTOPB;
tty.c_cflag     &=  ~CSIZE;
tty.c_cflag     |=  CS8;

tty.c_cflag     &=  ~CRTSCTS;           // no flow control
tty.c_cc[VMIN]   =  1;                  // read doesn't block
tty.c_cc[VTIME]  =  5;                  // 0.5 seconds read timeout
tty.c_cflag     |=  CREAD | CLOCAL;     // turn on READ & ignore ctrl lines

/* Make raw */
cfmakeraw(&tty);

/* Flush Port, then applies attributes */
tcflush( USB, TCIFLUSH );
if ( tcsetattr ( USB, TCSANOW, &tty ) != 0) {
   std::cout << "Error " << errno << " from tcsetattr" << std::endl;
}

int n = 0,
        spot = 0;
    char buf = '\0';

    /* Whole response*/
    char response[1024];
    memset(response, '\0', sizeof response);

    do {
        n = read( USB, &buf, 1 );
        sprintf( &response[spot], "%c", buf );
        spot += n;
    } while( buf != '\r' && n > 0);

    if (n < 0) {
        std::cout << "Error reading: " << strerror(errno) << std::endl;
    }
    else if (n == 0) {
        std::cout << "Read nothing!" << std::endl;
    }
    else {
        std::cout << "Response: " << response << std::endl;
    }

    message.assign(response, response + 1024);
    response[0] = 0;
    close(USB);
        //auto data = serialportDevice->readAll();
        std::string id = message.substr(0, 3);
        if (id == canDict["charging"]){
            charging = std::stoi(message.substr(7,1));
        }
        if (id == canDict["inverter"]) {
            velocity = strtoul(message.substr(12,2).c_str(), NULL, 16);
            recievedMessage["inverter"] = true;
        }
        if (id == canDict["lights"]) {
            std::string lightType = message.substr(6,2);
            bool enabled = std::stoi(message.substr(9,1));
            if (lightType == "01"){
                longLights = enabled;
                recievedMessage["lightLong"] = true;
            }
            if  (lightType == "02" || lightType == "06"){
                rightIndicator = enabled;
                recievedMessage["lightRight"] = true;
            }
            if (lightType == "03" || lightType == "06"){
                    leftIndicator = enabled;
                    recievedMessage["lightLeft"] = true;
            }
            //if  (lightType == "05")
            //	shortLights = enabled;
        }
        if (id == canDict["current"]){
            recievedMessage["current"] = true;
            warningCurrent = message.substr(6,2) == "00" ? "" :"Over-Current" ;
        }
        if (id == canDict["battery"]){
            recievedMessage["battery"] = true;
            recievedMessage["power"] = true;
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
        if (id == canDict["bmsGeneral"]){
            recievedMessage["bmsGeneral"] = true;

            bmsTemperatures[0] = "";
            for (int j = 4; j < 8; j++)
            {
                double temperature = strtoul(message.substr(6+(j*2),2).c_str(), NULL, 16);
                temperature = temperature + 30;
                if (j != 4)
                    bmsTemperatures[0].append(std::string(" "));
                // It should work without this if but it doesnt
                bmsTemperatures[0].append(std::to_string(temperature).substr(0,4));
            }
        }
        if (id == canDict["bmsTemp"]){
            recievedMessage["bmsTemp"] = true;

            bmsTemperatures[1] = "";
            for (int j = 0; j < 8; j++)
            {
                double temperature = strtoul(message.substr(6+(j*2),2).c_str(), NULL, 16);
                temperature = temperature + 30;
                if (j != 0)
                    bmsTemperatures[1].append(std::string(" "));
                // It should work without this if but it doesnt
                bmsTemperatures[1].append(std::to_string(temperature).substr(0,4));
            }
        }

        if (id == canDict["bmsWarning"]){
            recievedMessage["bmsWarning"] = true;
            if (message.substr(6,2)  != message.substr(8,2)){
                warningVolt = (message.substr(6,2) == "00") ? warningVolt : "Over-Voltage";
                warningVolt = (message.substr(8,2) == "00") ? warningVolt : "Under-Voltage";
            }
            else
                warningVolt = "";
            if (message.substr(10,2)  != message.substr(12,2)){
                warningTemp = (message.substr(10,2) == "00") ? warningTemp : "High-Temperature";
                warningTemp = (message.substr(12,2) == "00") ? warningTemp : "Low-Temperature";
            }
            else
                warningVolt = "";
            warningVoltDiff = (message.substr(14,2) == "00") ? "" : "Voltage-Difference";
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
QString SerialPort::getBMSVoltage(int index){return QString::fromStdString(bmsVoltages[index]);}
bool SerialPort::getBMSMode(){return bmsMode;}
QString SerialPort::getBMSTemperature(int index){return QString::fromStdString(bmsTemperatures[index]);};

QString SerialPort::getWarningCurrent(){return QString::fromStdString(warningCurrent);};
QString SerialPort::getWarningTemp(){return QString::fromStdString(warningTemp);};
QString SerialPort::getWarningVolt(){return QString::fromStdString(warningVolt);};
QString SerialPort::getWarningVoltDiff(){return QString::fromStdString(warningVoltDiff);};

bool SerialPort::getCharging(){return charging;}
