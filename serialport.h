#pragma once

#include <QDebug>
#include <QObject>
#include <QString>
#include <QSerialPort>
#include <QSerialPortInfo>

#include <canopen_object_dict.h>

#include <string>
#include <vector>

class SerialPort : public QObject {
  Q_OBJECT

public:
  explicit SerialPort(QObject *parent = nullptr);
  void connectToSerialPort();

public slots:
  //void readData();
  double getVelocity(){return 69;}
  bool getRightIndicator(){return rightIndicator;}
  bool getLeftIndicator(){return leftIndicator;}
  double getBMSVoltage(int index){return (index > 0 && index <=29 )? bmsVoltages[index] : 0; };
  bool getBMSError(){return bmsError;};

private:
  std::vector<double> bmsVoltages;

  QString ecm_com_port;
  QSerialPort *serialport_device;
  QString serialport_message;
  double velocity;
  bool leftIndicator;
  bool rightIndicator;
  bool bmsError;

  std::map<std::string, std::string> canDict;

};
