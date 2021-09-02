#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickView>

#include <iostream>

/* Private includes */
#include <QQmlContext>
#include <QQmlComponent>
//#include <canopen_object_dict.h>
#include <serialport.h>

int main(int argc, char *argv[]) {
  QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
  QGuiApplication app(argc, argv);


  /* Connect C++ with QML engine */
  SerialPort serialPort;
  /* Start reading from COM port */
  serialPort.connectToSerialPort();
  QQmlApplicationEngine engine;

  QQmlContext *context = engine.rootContext();
  context->setContextProperty("serialPort", &serialPort);
  const QUrl url(QStringLiteral("qrc:/main.qml"));
  QObject::connect(
      &engine, &QQmlApplicationEngine::objectCreated, &app,
      [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
          QCoreApplication::exit(-1);
      }, Qt::QueuedConnection);
  engine.load(url);
  return app.exec();
}
