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
  QString getBMSError();
  bool getCurrentWarning();
  QString getBMSVoltage(int index);
  bool getBMSMode();
  QString getBMSTemperature();


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
  bool bmsMode;
  std::map<std::string, std::string> canDict;
  std::vector<std::string> bmsVoltages;
  std::string bmsTemperatures;
};
