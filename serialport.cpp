
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
#include <cstdlib> // Random  numbers used only to generate nice looking numbers

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
        {"lights", "581"},
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
    char response[1204];
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
    std::string id = message.substr(0, 3);
    if (id == "581") {
        std::string lightType = message.substr(8,2);
        bool enabled = std::stoi(message.substr(11,1));
        if (lightType == "01"){
        longLights = enabled;
        }
        if  (lightType == "02" || lightType == "06"){
        rightIndicator = enabled;
        }
        if (lightType == "03" || lightType == "06"){
        leftIndicator = enabled;
        }
        //if  (lightType == "05")
        //	shortLights = enabled;
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

double SerialPort::getVelocity(){
    srand(time(NULL));
    return 30.0 - rand()%20/20.0;
}
double SerialPort::getBattery(){
    srand(time(NULL));
    return 60.0 - rand()%20/20.0;
}
double SerialPort::getPower(){
    srand(time(NULL));
    return 10.0 - rand()%20/20.0;
}
bool SerialPort::getRightIndicator(){return rightIndicator;}
bool SerialPort::getLeftIndicator(){return leftIndicator;}
bool SerialPort::getLongLights(){return longLights;}
bool SerialPort::getShortLights(){return shortLights;}
bool SerialPort::getAwarLights(){return leftIndicator && rightIndicator;}
QString SerialPort::getBMSVoltage(int index){return QString::fromStdString(bmsVoltages[index]);}
bool SerialPort::getBMSMode(){return bmsMode;}
QString SerialPort::getBMSTemperature(int index){
    std::string value;
    srand(time(NULL));
    for (int i = 0; i < 8; i++){
        // Generating string that is a value of 40 with randomly adding some values
        // substr is cutting out unneccasary zeros

        value.append(std::to_string(40.0f - (rand()%10)/10.0).substr(0, 4));
        value.append(" ");
    }
    return QString::fromStdString(value);
};
QString SerialPort::getWarningCurrent(){return QString::fromStdString(warningCurrent);};
QString SerialPort::getWarningTemp(){return QString::fromStdString(warningTemp);};
QString SerialPort::getWarningVolt(){return QString::fromStdString(warningVolt);};
QString SerialPort::getWarningVoltDiff(){return QString::fromStdString(warningVoltDiff);};

bool SerialPort::getCharging(){return charging;}
