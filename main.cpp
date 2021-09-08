#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickView>

#include <iostream>

/* Private includes */
#include "serialport.h"
#include <QQmlContext>
#include <QQmlComponent>

int main(int argc, char *argv[]) {
  QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
  QGuiApplication app(argc, argv);
  QQmlApplicationEngine engine;

  /* Connect C++ with QML engine */
  SerialPort serialPort;
  /* Start reading from COM port */
  serialPort.connectToSerialPort();

  QQmlContext *context = engine.rootContext();
  context->setContextProperty("serialPort", &serialPort);
  const QUrl url(QStringLiteral("qrc:/main.qml"));
      engine.rootContext()->setContextProperty("appPath", QGuiApplication::applicationDirPath());
  QObject::connect(
      &engine, &QQmlApplicationEngine::objectCreated, &app,
      [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
          QCoreApplication::exit(-1);
      }, Qt::QueuedConnection);
  engine.load(url);
  return app.exec();
}
