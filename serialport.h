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
  double getVelocity();
  double getBattery();
  double getPower();
  bool getRightIndicator();
  bool getLeftIndicator();
  bool getLongLights();
  bool getShortLights();
  bool getAwarLights();
  QString getBMSVoltage(int index);
  bool getBMSMode();
  QString getBMSTemperature(int index);

  QString getWarningCurrent();
  QString getWarningTemp();
  QString getWarningVolt();
  QString getWarningVoltDiff();

private:
  QSerialPort *serialportDevice;
  QSerialPortInfo serialportInfo;
  double velocity;
  double battery;
  double power;
  bool leftIndicator;
  bool rightIndicator;
  bool shortLights;
  bool longLights;
  bool bmsMode;

  std::string message;

  std::string warningCurrent;
  std::string warningTemp;
  std::string warningVolt;
  std::string warningVoltDiff;

  std::map<std::string, std::string> canDict;
  std::vector<std::string> bmsVoltages;
  std::vector<std::string> bmsTemperatures;
};
