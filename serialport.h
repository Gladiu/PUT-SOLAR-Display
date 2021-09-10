#pragma once

#include <QDebug>
#include <QObject>
#include <QString>
#include <QSerialPort>
#include <QSerialPortInfo>

#include <string>
#include <vector>

class SerialPort : public QObject {
  Q_OBJECT

public:
  explicit SerialPort(QObject *parent = nullptr);
  void connectToSerialPort();

public slots:
  void readData();
  double getVelocity(){return velocity;}
  double getBattery(){return battery;}
  double getPower(){return power;}
  bool getRightIndicator(){return rightIndicator;}
  bool getLeftIndicator(){return leftIndicator;}
  bool getLongLights(){return longLights;}
  bool getShortLights(){return shortLights;}
  bool getAwarLights(){return leftIndicator && rightIndicator;}
  QString getBMSError(){return QString::fromStdString(bmsErrors[bmsErrorIndex]);}
  bool getCurrentWarning(){return currentWarning;}
  	
private:
  QSerialPort serialportDevice;
  QSerialPortInfo serialportInfo;
  double velocity;
  double battery;
  double power;
  bool leftIndicator;
  bool rightIndicator;
  bool shortLights;
  bool longLights;
  int bmsErrorIndex;
  std::vector<std::string> bmsErrors{
      ""
      "ERROR: High Voltage",
      "ERROR: Low Voltage",
      "ERROR: High Temperature",
      "ERROR: Low Temperature",
      "WARNING: High Voltage",
      "WARNING: Low Voltage",
      "WARNING: High Temperature",
      "WARNING: Low Temperature"
      "ERROR: Too High Volt. Diff.",
      "WARNING: Too High Volt. Diff."
  };
  bool currentWarning;
  std::map<std::string, std::string> canDict;

};
