#include "serialport.h"
#include <string>
#include <iomanip>
#include <sstream>
#include <QString>
#include <QDebug>
#include <iostream>

SerialPort::SerialPort(QObject *parent) : QObject(parent) {
  /* Creating constructor and freeing memory */
  this->serialport_device = new QSerialPort(this);
    velocity = 0;
    leftIndicator = false;
    rightIndicator = false;
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
 * @brief: Read data from serial port
 * @return: Value stored in inbox
 *
 */


bool SerialPort::readVelocity()
{
    serialport_message = this->serialport_device->readLine();
    QString terminator = "\r";
    //qDebug() << "Serial port message: " + serialport_message;
    std::string message(serialport_message.toStdString());
    std::string id = message.substr(0, 3);
    // inverter
    if (id == "290") {
      std::stringstream stream;
      message = message.substr(12,2);
      velocity = strtoul(message.c_str(), NULL, 16);
      return true;
    }
    else
        return false;
}

double SerialPort::getVelocity()
{
    while (readVelocity())
        int a;
    return velocity;
}

bool SerialPort::readRightIndicator()
{
    serialport_message = this->serialport_device->readLine();
    QString terminator = "\r";
    //qDebug() << "Serial port message: " + serialport_message;
    std::string message(serialport_message.toStdString());
    std::string id = message.substr(0, 3);
    std::cout<<message<<std::endl;
    if (id == "581" && (message.substr(8,2) == "02" || message.substr(8,2) == "06")) {
      rightIndicator = true;

      std::cout<<"a"<<std::endl;
      return true;
    }
    else
        return false;
}

bool SerialPort::getRightIndicator()
{
    while (readRightIndicator())
        int a;
    return rightIndicator;
}
//1 długie
//2 prawy
//3 lewy
//4 przeciwmgielne
//5 krótkie
//6 awaryjne
//9 postojowe
bool SerialPort::readLeftIndicator()
{
    serialport_message = this->serialport_device->readLine();
    QString terminator = "\r";
    //qDebug() << "Serial port message: " + serialport_message;
    std::string message(serialport_message.toStdString());
    std::string id = message.substr(0, 3);
    if (id == "581" && (message.substr(8,2) == "03" || message.substr(8,2) == "06")) {
      leftIndicator = true;
      return true;
    }
    else
        return false;
}

bool SerialPort::getLeftIndicator()
{
    while (readLeftIndicator())
        int a;
    return leftIndicator;
}
