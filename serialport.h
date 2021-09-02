#pragma once

#include <QDebug>
#include <QObject>
#include <QString>
#include <QSerialPort>
#include <QSerialPortInfo>

#include <canopen_object_dict.h>

#include <string>

class SerialPort : public QObject {
  Q_OBJECT

public:
  explicit SerialPort(QObject *parent = nullptr);
  void connectToSerialPort();

public slots:
  double getVelocity();
  bool getRightIndicator();
  bool getLeftIndicator();


private:
  bool readFromSerialPort();
  bool readVelocity();
  bool readLeftIndicator();
  bool readRightIndicator();

  QString ecm_com_port;
  QSerialPort *serialport_device;
  QString serialport_message;
  double velocity;
  double battery;
  int position;
  bool leftIndicator;
  bool rightIndicator;
};

